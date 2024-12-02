#include "spheres.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXIT_CODE 12

void worldInit(World *world) {
    world->spheres = malloc(sizeof(Sphere *));

    if(world->spheres == NULL) {
        fprintf(stderr, "Memory allocation failure.\n");
        freeWorld(world);
        exit(EXIT_CODE);
    }

    world->size = 0;
    world->capacity = 1;

    return;
}

void freeWorld(World *world) {
    if(world->spheres != NULL) {
        for(int i = 0; i < world->size; i++) {
            if(world->spheres[i] != NULL) {
                free(world->spheres[i]);
            }
        }

        free(world->spheres);
        world->spheres = NULL;
    }

    return;
}

void addSphere(World *world, Sphere *sphere) {
    if(world->size == world->capacity) {
        world->capacity *= 2;

        Sphere **temp = realloc(world->spheres, (world->capacity) * sizeof(Sphere *));

        if(temp == NULL) {
            fprintf(stderr, "Memory allocation failure.\n");
            freeWorld(world);
            exit(EXIT_CODE);
        }

        world->spheres = temp;
    }

    world->spheres[world->size] = sphere;
    world->size++;

    return;
}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    /*
        Checks for an intersection between a ray and a sphere.
    */
    
    Vec3 v = subtract(rayPos, sphere->pos);
    float a = dot(rayDir, rayDir);
    float b = 2 * dot(rayDir, v);
    float c = dot(v, v) - (sphere->r * sphere->r);
    float discriminant = calculateDiscriminant(a, b, c);


    if(discriminant < 0) {
        return 0;
    } 

    float t1, t2;
    solveQuadratic(a, b, c, &t1, &t2);

    if(t1 > 0 && t2 > 0) {
        *t = (t1 < t2) ? t1 : t2;
    } else if(t1 > 0) {
        *t = t1;
    } else if(t2 > 0) {
        *t = t2;
    } else {
        return 0;
    }

    return 1;
}

float calculateDiscriminant(float a, float b, float c) {
    return (b * b) - (4 * a * c);
}

void solveQuadratic(float a, float b, float c, float *t1, float *t2) {
    float discriminant = calculateDiscriminant(a, b, c);
    float shift = sqrt(discriminant) / (2 * a);
    float val = -b / (2 * a);

    *t1 = val - shift;
    *t2 = val + shift;
}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *s = (Sphere *) malloc(sizeof(Sphere));

    if(s == NULL) {
        fprintf(stderr, "Memory allocation failure.\n");
        
        return NULL;
    }

    s->r = radius;
    s->pos = position;
    s->color = color;

    return s;
}