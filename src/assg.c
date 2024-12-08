#include <stdio.h>
#include "spheres.h"
#include "assg.h"
#include "color.h"
#include <math.h>

Light light;

int main(int argc, char *argv[]) {
    FILE *input;
    FILE *output;
    
    if(argc >= 3) {
        input = fopen(argv[1], "r");
        
        if(input == NULL) {
            fprintf(stderr, "Error opening input stream: %s\n", argv[1]);
            
            return 1;
        }

        output = fopen(argv[2], "w");

        if(output == NULL) {

            fprintf(stderr, "Error opening output stream: %s\n", argv[2]);
            fclose(input);
            
            return 1;
        }
    }

    int imageWidth;
    int imageHeight;

    fscanf(input, "%d %d", &imageWidth, &imageHeight);

    Camera c = {.position = {0, 0, 0}};
    fscanf(input, "%f", &c.viewportHeight);
    fscanf(input, "%f", &c.focalLength);
    
    c.viewportWidth = c.viewportHeight * ((float) imageWidth / (float) imageHeight);
    c.viewportPosition = (Vec3) {c.viewportWidth, c.viewportHeight, -(c.focalLength)}; 

    fscanf(input, "%f %f %f", &light.position.x, &light.position.y, &light.position.z);
    fscanf(input, "%f", &light.brightness);

    int numColors;
    fscanf(input, "%d", &numColors);

    int colors[numColors];

    for(int i = 0; i < numColors; i++) {
        fscanf(input, "%x", &colors[i]);
    }

    // sort colors
    qsort(colors, numColors, sizeof(int), compareColor);

    Vec3 *rgbColors = malloc(numColors * sizeof(Vec3));

    for(int i = 0; i < numColors; i++) {
        rgbColors[i] = unpackRGB(colors[i]);
    }
    
    int backgroundColorIndex;
    fscanf(input, "%d", &backgroundColorIndex);
    Vec3 backgroundColor = rgbColors[backgroundColorIndex];

    int numSpheres;
    fscanf(input, "%d", &numSpheres);

    World w;
    worldInit(&w);

    for(int i = 0; i < numSpheres; i++) {
        float radius;
        int colorIndex;
        Vec3 position;

        fscanf(input, "%f %f %f", &position.x, &position.y, &position.z);
        fscanf(input, "%f", &radius); 
        fscanf(input, "%d", &colorIndex);
        
        Sphere *s = createSphere(radius, position, rgbColors[colorIndex]);
        
        if(s == NULL) {
            fprintf(stderr, "Memory allocation failure.\n");
            freeWorld(&w);
            free(rgbColors);
            fclose(input);
            fclose(output);

            return 1;
        }

        addSphere(&w, s);
    } 

    createImage(w, c, light, output, imageWidth, imageHeight, backgroundColor);

    freeWorld(&w);
    free(rgbColors);
    fclose(input);
    fclose(output);
    
    return 0;
}

void createImage(World world, Camera camera, Light light, FILE *outputFile, int imageWidth, int imageHeight, Vec3 backgroundColor) {
    /*
        Renders the scene using ray traicing.
    */

    // write the PPM image header to the file
    
    fprintf(outputFile, "P3\n%d %d\n255\n", imageWidth, imageHeight);

    // iterate through the each pixel

    for(int y = 0; y < imageHeight; y++) {
        for(int x = 0; x < imageWidth; x++) {
            Vec3 pixelColor = getAntiAliasedColor(world, light, camera, x, y, imageWidth, imageHeight, backgroundColor);
            
            fprintf(outputFile, "%d %d %d ", (int) pixelColor.x, (int) pixelColor.y, (int) pixelColor.z);
        }

        fprintf(outputFile, "\n"); // move to a new row
    }
}

Vec3 getAntiAliasedColor(World world, Light light, Camera camera, int x, int y, int imageWidth, int imageHeight, Vec3 backgroundColor) {
    /*
        Computes the final color of a pixel using anti-aliasing.
    */
    
    Vec3 initialColor = (Vec3) {0, 0, 0};
    int gridSize = 2;

    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            float offsetX = (i + 0.5) / gridSize;
            float offsetY = (j + 0.5) / gridSize;
            float u = (-(camera.viewportWidth) / 2) + ((x + offsetX) * (camera.viewportWidth / imageWidth));
            float v = (camera.viewportHeight / 2) - ((y + offsetY) * (camera.viewportHeight / imageHeight));

            Vec3 rayDir = normalize((Vec3){u, v, -camera.focalLength});
            Vec3 subPixelColor = renderPixel((Vec3) {0, 0, 0}, rayDir, world, light, backgroundColor);
            initialColor = add(initialColor, subPixelColor);
        }
    }

    return scalarDivide(initialColor, gridSize * gridSize);
}

Vec3 renderPixel(Vec3 rayOrigin, Vec3 rayDir, World world, Light light, Vec3 backgroundColor) {
    /*
        Determines the color of a pixel.
    */

    Sphere *closestSphere = NULL;
    float closestT = 1e9; // arbitrary large value for initial comparison

    // find the closest sphere to the ray along with the distance

    for(int i = 0; i < world.size; i++) {
        float t;

        if(doesIntersect(world.spheres[i], rayOrigin, rayDir, &t)) {
            if(t < closestT) {
                closestT = t;
                closestSphere = world.spheres[i];
            }
        }
    }

    if(closestSphere == NULL) {
        return backgroundColor;
    }

    // calculating final pixel color 

    Vec3 intersectionPoint = add(rayOrigin, scalarMultiply(closestT, rayDir));
    Vec3 normal = normalize(subtract(intersectionPoint, closestSphere->pos));
    Vec3 lightDir = normalize(subtract(light.position, intersectionPoint));

    float dotDN = fmaxf(0.0, dot(normal, lightDir));
    float distanceToLight = length(subtract(light.position, intersectionPoint));
    float intensity = light.brightness * (dotDN / (distanceToLight * distanceToLight));
    intensity = fminf(1.0, intensity);

    if(inShadow(intersectionPoint, normal, light.position, world)) {
        intensity *= 0.1; // reduce intensity for shadowed areas
    }

    return scalarMultiply(intensity, closestSphere->color);
}

bool inShadow(Vec3 intersectionPoint, Vec3 normal, Vec3 lightPosition, World world) {
    /*
        Determines if a point is in the shadow.
    */

    float epsilon = 0.001; // offset to avoid precision errors
    float distanceToLight = length(subtract(lightPosition, intersectionPoint));

    // offset the origin of the shadow ray to avoid self-intersection

    Vec3 shadowRayOrigin = add(intersectionPoint, scalarMultiply(epsilon, normal));
    Vec3 shadowRayDir = normalize(subtract(lightPosition, intersectionPoint));
    
    // if an object intersects and is closer than the light source, the point is in the shadow

    for(int i = 0; i < world.size; i++) {
        float t;

        if(doesIntersect(world.spheres[i], shadowRayOrigin, shadowRayDir, &t)) {
            if(t > 0 && t < distanceToLight) {
                return true;
            }
        }
    }

    return false;
}