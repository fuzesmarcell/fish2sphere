/*
MIT License

Copyright (c) [2023] [Marcel Fuzes]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

// convert `x` degrees to radians
constexpr static inline float radians(float x) {
    return x*0.017453292519943295f;
}

// implementing different projections from https://paulbourke.net/dome/fish2/
void fish2sphere(u32* fe_pixels, int fe_w, int fe_h, u32* s_pixels, int s_w, int s_h, float fov, float theta_off, float phi_off) {

    float width = (float)s_w;
    float height = (float)s_h;

#pragma omp parallel for
    for (int y = 0; y < s_h; y++) {
        for (int x = 0; x < s_w; x++) {

            // coordinates in the spherical image in polar coordinates
            float theta = 2.0f * 3.14159265f * (x / width - 0.5f);
            float phi = 3.14159265f * (y / height - 0.5f);

            theta += theta_off;
            phi += phi_off;

            // vector in 3D space
            vec3 psph;
            psph.x = cosf(phi) * sinf(theta);
            psph.y = cosf(phi) * cosf(theta);
            psph.z = sinf(phi);

            // calculate fish eye angle and radius
        #if 1
            theta = atan2f(psph.z, psph.x);
            phi = atan2f(sqrtf(psph.x*psph.x+psph.z*psph.z), psph.y);
        #else
            theta = atan2f(psph.y, psph.x);
            phi = atan2f(psph.z, sqrtf(psph.x*psph.x + psph.y*psph.y));
        #endif
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

                // TODO: bilinear sample from fish eye image
                s_pixels[y*s_w+x] = fe_pixels[pfishi.y * fe_w + pfishi.x];
            }
            else {
                s_pixels[y*s_w+x] = 0x00000000;
            }
        }
    }
}
