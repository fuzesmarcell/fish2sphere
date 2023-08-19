#include "fish2sphere.h"

#include <math.h>

typedef uint32_t u32;

class vec3f {
public:
    float x, y, z;
};

typedef vec3f vec3;

class vec2f {
public:
    float x, y;
};

typedef vec2f vec2;

class vec2i {
public:
    int x, y;
};

void fish2sphere(u32* fe_pixels, int fe_w, int fe_h, float fov, u32* s_pixels, int s_w, int s_h) {

    float width = (float)s_w;
    float height = (float)s_h;

#pragma omp parallel for
    for (int y = 0; y < s_h; y++) {
        for (int x = 0; x < s_w; x++) {

            // coordinates in the spherical image in polar coordinates
            float theta = 2.0f * 3.14159265f * (x / width - 0.5f);
            float phi = 3.14159265f * (y / height - 0.5f);

            // vector in 3D space
            vec3 psph;
            psph.x = cosf(phi) * sinf(theta);
            psph.y = cosf(phi) * cosf(theta);
            psph.z = sinf(phi);

            // calculate fish eye angle and radius
            theta = atan2f(psph.z, psph.x);
            phi = atan2f(sqrtf(psph.x*psph.x+psph.z*psph.z), psph.y);
            float r = fe_w * phi / fov;

            // pixel in fish eye space
            vec2 pfish;
            pfish.x = 0.5f * fe_w + r * cosf(theta);
            pfish.y = 0.5f * fe_w + r * sinf(theta);

            vec2i pfishi;
            pfishi.x = (int)pfish.x;
            pfishi.y = (int)pfish.y;

            if (pfishi.x >= 0   &&
                pfishi.x < fe_w &&
                pfishi.y >= 0   &&
                pfishi.y < fe_h) {

                s_pixels[y*s_w+x] = fe_pixels[pfishi.y * fe_w + pfishi.x];
            }
            else {
                s_pixels[y*s_w+x] = 0xff00ff00;
            }
        }
    }
}
