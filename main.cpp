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

#include <stdio.h>
#include <inttypes.h>

#include <filesystem>

namespace fs = std::filesystem;

#include <stb_image.h>
#include <stb_image_write.h>

// `x` degrees in radians
constexpr static inline float radians(float x) {
    return x*0.017453292519943295f;
}

// `x` radians in degrees
constexpr static inline float degrees(float x) {
    return x*57.29577951308232f;
}

int main(int argc, char** args) {

    if (argc < 3 || argc > 6) {
        printf("Usage: \n");
        printf("  Convert a fish eye image to a spherical\n");
        printf("  fish2sphere <fish eye> <w> <fov=180> <theta=0> <phi=0>\n");
        return -1;
    }

    int out_w = atoi(args[2]);
    int out_h = out_w / 2;

    float fov = argc >= 4 ? radians(strtof(args[3], nullptr)) : radians(180.f);
    float theta = argc >= 5 ? radians(strtof(args[4], nullptr)) : radians(0.f);
    float phi = argc >= 6 ? radians(strtof(args[5], nullptr)) : radians(0.f);

    printf("Creating spherical image %d:%d\n", out_w, out_h);
    printf("fov = %.2f\n", degrees(fov));
    printf("theta = %.2f\n", degrees(theta));
    printf("phi = %.2f\n", degrees(phi));

    int x, y;
    int ch;
    unsigned char* data = stbi_load(args[1], &x, &y, &ch, 4);
    if (!data) {
        fprintf(stderr, "Unable to load image %s\n", args[1]);
        return -1;
    }

    if (x != y) {
        fprintf(stderr, "fish eye image must be rectangular\n");
        return -1;
    }

    size_t stride_in_bytes = sizeof(uint32_t) * out_w;
    uint32_t* output = (uint32_t*)malloc(stride_in_bytes * out_h);
    if (!output) {
        fprintf(stderr, "Unable to allocate memory for output image\n");
        return -1;
    }

    fish2sphere((uint32_t*)data, x, y, output, out_w, out_h, fov, theta, phi);

    fs::path in_path(args[1]);
    fs::path out_path(in_path.parent_path() / fs::path("spherical_" + in_path.replace_extension(".png").filename().string()));

    int result = stbi_write_png(out_path.string().c_str(), out_w, out_h, 4, output, out_w * sizeof(uint32_t));
    if (!result) {
        fprintf(stderr, "Unable to save spherical.png\n");
        return -1;
    }

    free(output);
    stbi_image_free(data);

    return 0;
}