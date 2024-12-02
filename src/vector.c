#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

Vec3 add(Vec3 v1, Vec3 v2) {
    Vec3 v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.z = v1.z + v2.z;

    return v;
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
    Vec3 v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;

    return v;
}

Vec3 scalarMultiply(float s, Vec3 v) {
    v.x = s * v.x;
    v.y = s * v.y;
    v.z = s * v.z;

    return v;
}

Vec3 scalarDivide(Vec3 v, float d) {
    if(d == 0) {
        fprintf(stderr, "Divide by 0 error.\n");
        exit(12);
    }
    
    float inverse = 1 / d;

    return scalarMultiply(inverse, v);
}

Vec3 normalize(Vec3 v) {
    float len = length(v);

    return scalarDivide(v, len);
}

float dot(Vec3 v1, Vec3 v2) {
    float product = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

    return product;
}

float length2(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float length(Vec3 v) {
    float radicand = v.x * v.x + v.y * v.y + v.z * v.z;

    return sqrt(radicand);
}

float distance2(Vec3 v1, Vec3 v2) {
    Vec3 diff = subtract(v1, v2);

    return length2(diff);
}

float distance(Vec3 v1, Vec3 v2) {
    Vec3 diff = subtract(v1, v2);
    float distance = length(diff);

    return distance;
}