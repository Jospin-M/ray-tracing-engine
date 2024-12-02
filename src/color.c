#include "color.h"

void writeColour(FILE *ppmFile, Vec3 color) {
    /*
        Write the RGB values of a color to output PPM file.
    */

    fprintf(ppmFile, "%f %f %f ", color.x * 255, color.y * 255, color.z  * 255);
}