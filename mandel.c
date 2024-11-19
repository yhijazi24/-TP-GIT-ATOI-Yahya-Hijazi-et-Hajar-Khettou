#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "ppm.h"

#define TRSH 2.0
#define ITER 1024ull

#define SIZEX 1500
#define SIZEY 1500

double cx(int x)
{
    /* -2 ---> 1 */
    static const double qx = 3.0 / (double)SIZEX;
    return -2.0 + x * qx;
}

double cy(int y)
{
    /* -1 ---> 1 */
    static const double qy = 2.0 / (double)SIZEY;
    return -1.0 + y * qy;
}

int main(int argc, char *argv[])
{
    struct ppm_image im;
    /* Nous avons supprimé 'amp;' avant 'im' */
    ppm_image_init(&im, SIZEX, SIZEY);

    int i, j;
    double colref = 255.0 / log(ITER);

    for (i = 0; i < SIZEX; ++i) {
        for (j = 0; j < SIZEY; ++j) {
            unsigned long int iter = 0;

            double complex c = cx(i) + cy(j) * I;
            double complex z = 0;

            while (iter < ITER) {
                double mod = cabs(z);
                /* Nous avons supprimé '&lt;' car son équivalent est '<' */

                if (TRSH < mod) {
                    break;
                }

                z = z * z + c;
                iter++;
            }

            int grey = colref * log(iter);
            /* Nous avons supprimé 'amp;' avant 'im' */

            ppm_image_setpixel(&im, i, j, grey, grey, grey);
        }
    }
    /* Nous avons supprimé 'amp;' avant 'im' */
    ppm_image_dump(&im, "m.ppm");
    /* Nous avons supprimé 'amp;' avant 'im' */

    ppm_image_release(&im);

    return 0;
}
