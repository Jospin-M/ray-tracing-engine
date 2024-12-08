#include "vector.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    const Vec3 position;
    float viewportHeight;
    float viewportWidth;
    float focalLength;
    Vec3 viewportPosition;

} Camera;

typedef struct {
    Vec3 position;
    float brightness;
} Light;

void createImage(World world, Camera camera, Light light, FILE *outputFile, int imageWidth, int imageHeight, Vec3 backgroundColor);
Vec3 renderPixel(Vec3 rayOrigin, Vec3 rayDir, World world, Light light, Vec3 backgroundColor);
bool inShadow(Vec3 intersectionPoint, Vec3 normal, Vec3 lightPosition, World world);
Vec3 getAntiAliasedColor(World world, Light light, Camera camera, int x, int y, int imageWidth, int imageHeight, Vec3 backgroundColor);