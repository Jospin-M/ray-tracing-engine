#include <stdio.h>
#include "spheres.h"
#include "assg.h"
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

    unsigned int colors[numColors];
    
    for(int i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
        fscanf(input, "%x", &colors[i]);
    }

    int backgroundColorIndex;
    fscanf(input, "%d", &backgroundColorIndex);
    Vec3 backgroundColor = (Vec3) {0.0, 0.0, 0.0};

    int numSpheres;
    fscanf(input, "%d", &numSpheres);

    World w;
    worldInit(&w);

    for(int i = 0; i < numSpheres; i++) {
        float radius;
        Vec3 position;
        Vec3 color = {1.0, 1.0, 1.0};
        int colorIndex;

        fscanf(input, "%f %f %f", &position.x, &position.y, &position.z);
        fscanf(input, "%f", &radius); 
        fscanf(input, "%d", &colorIndex);
        Sphere *s = createSphere(radius, position, color);
        
        if(s == NULL) {
            fprintf(stderr, "Memory allocation failure.\n");
            freeWorld(&w);
            fclose(input);
            fclose(output);

            return 1;
        }

        addSphere(&w, s);
    }

    output_(w, backgroundColor, light, input, output, c, numSpheres);    

    freeWorld(&w);
    fclose(input);
    fclose(output);
    
    return 0;
}

void output_(World w, Vec3 backgroundColor, Light light, FILE *input, FILE *output, Camera c, int numSpheres) {
    Vec3 t = add(backgroundColor, light.position);
    fprintf(output, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = ", backgroundColor.x, backgroundColor.y, backgroundColor.z, light.position.x, light.position.y, light.position.z);
    fprintf(output, "(%.1f, %.1f, %.1f)\n", t.x, t.y, t.z);

    Vec3 u = subtract(backgroundColor, light.position);
    fprintf(output, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = ", backgroundColor.x, backgroundColor.y, backgroundColor.z, light.position.x, light.position.y, light.position.z);
    fprintf(output, "(%.1f, %.1f, %.1f)\n", u.x, u.y, u.z);
    
    fprintf(output, "%.1f * (%.1f, %.1f, %.1f) = ",  c.viewportWidth, light.position.x, light.position.y, light.position.z);
    Vec3 v = scalarMultiply(c.viewportWidth, light.position);
    fprintf(output, "(%.1f, %.1f, %.1f)\n", v.x, v.y, v.z);

    Vec3 x = normalize(light.position);
    fprintf(output, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n\n", light.position.x, light.position.y, light.position.z,
    x.x, x.y, x.z);

    for(int i = 0; i < numSpheres; i++) {
        Vec3 y = scalarDivide(w.spheres[i]->color, w.spheres[i]->r);

        if(y.x == -9999) { 
            fprintf(stderr, "Division by zero.\n");
            freeWorld(&w);
            fclose(input);
            fclose(output);

            exit(1);
        }

        fprintf(output, "(1.0, 1.0, 1.0) / %.1f = ", w.spheres[i]->r);
        fprintf(output, "(%.1f, %.1f, %.1f)\n", y.x, y.y, y.z);

        float z = dot(light.position, w.spheres[i]->pos);
        fprintf(output, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n", light.position.x, light.position.y, light.position.z,
        w.spheres[i]->pos.x, w.spheres[i]->pos.y, w.spheres[i]->pos.z, z);

        float a = distance(light.position, w.spheres[i]->pos);
        fprintf(output, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n", light.position.x, light.position.y, light.position.z,
        w.spheres[i]->pos.x, w.spheres[i]->pos.y, w.spheres[i]->pos.z, a);

        float b = length(w.spheres[i]->pos);
        fprintf(output, "length(%.1f, %.1f, %.1f) = %.1f\n", w.spheres[i]->pos.x, w.spheres[i]->pos.y, w.spheres[i]->pos.z, b);
        
        if(i < numSpheres - 1) {
            fprintf(output, "\n");
        }
    }
}