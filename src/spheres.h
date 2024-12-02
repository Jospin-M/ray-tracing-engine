#ifndef SPHERES_H
#define SPHERES_H

#include "vector.h"

typedef struct {
    float r;
    Vec3 pos;
    Vec3 color;
} Sphere;

typedef struct {
    Sphere **spheres;
    int size;
    int capacity;
} World;

void worldInit(World *world);
void freeWorld(World *world);
void addSphere(World *world, Sphere *sphere);
Sphere *createSphere(float radius, Vec3 position, Vec3 color);
int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t);
float calculateDiscriminant(float a, float b, float c);
void solveQuadratic(float a, float b, float c, float *t1, float *t2);

#endif
