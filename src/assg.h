#include "vector.h"
#include <stdlib.h>

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

void output_(World w, Vec3 backgroundColor, Light light, FILE *input, FILE *output, Camera c, int numSpheres);