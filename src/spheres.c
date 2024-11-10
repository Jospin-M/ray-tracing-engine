#include "spheres.h"
#include <stdlib.h>
#include <stdio.h>
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