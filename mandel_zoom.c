#include "ppm.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TRSH 2.0
#define ITER 1024ull

#define SIZEX 1500
#define SIZEY 1500

struct col {
    int r;
    int g;
    int b;
};

struct col getcol(int val, int max) {
    double q = (double)val / (double)max;

    struct col c = {0, 0, 0};

    if (q < 0.25) {
        c.r = (q * 4.0) * 255.0;
        c.b = 255;
    } else if (q < 0.5) {
        c.b = 255;
        c.g = 255;
        c.r = (q - 0.25) * 4.0 * 255.0;
    } else if (q < 0.75) {
        c.b = 255;
        c.r = 255;
        c.g = 255.0 - (q - 0.5) * 4.0 * 255.0;
    } else {
        c.b = 255 - (q - 0.75) * 4.0 * 255.0;
        c.g = 0;
        c.r = 255;
    }

    return c;
}

double cx(int x, double centerX, double scaleX) {
    return centerX + (x - SIZEX / 2) * scaleX / SIZEX;
}

double cy(int y, double centerY, double scaleY) {
    return centerY + (y - SIZEY / 2) * scaleY / SIZEY;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <centerX> <centerY> <zoom>\n", argv[0]);
        return 1;
    }

    double centerX = atof(argv[1]);
    double centerY = atof(argv[2]);
    double zoom = atof(argv[3]);

    double scaleX = 3.0 / zoom;
    double scaleY = 2.0 / zoom;

    struct ppm_image im;
    ppm_image_init(&im, SIZEX, SIZEY);

    int i, j;
    double colref = 255.0 / log(ITER);

    for (i = 0; i < SIZEX; ++i) {
        for (j = 0; j < SIZEY; ++j) {
            unsigned long int iter = 0;

            double complex c = cx(i, centerX, scaleX) + cy(j, centerY, scaleY) * I;
            double complex z = 0;

            while (iter < ITER) {
                double mod = cabs(z);

                if (TRSH < mod) {
                    break;
                }

                z = z * z + c;
                iter++;
            }

            int grey = colref * log(iter);
            ppm_image_setpixel(&im, i, j, grey, grey, grey);
        }
    }

    ppm_image_dump(&im, "mandelbrot_zoom.ppm");
    ppm_image_release(&im);

    printf("Generated Mandelbrot image with center (%.2f, %.2f) and zoom %.2f\n", centerX, centerY, zoom);
    return 0;
}
