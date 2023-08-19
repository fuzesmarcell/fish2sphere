#include "fish2sphere.h"

#include <stdio.h>
#include <inttypes.h>

#include <filesystem>

namespace fs = std::filesystem;

#include <stb_image.h>
#include <stb_image_write.h>

int main(int argc, char** args) {

    if (argc != 3) {
        printf("Usage: \n");
        printf("  Convert a fish eye image to a spherical\n");
        printf("  fish2sphere fish eye w\n");
        return -1;
    }

    int out_w = atoi(args[2]);
    int out_h = out_w / 2;

    float fov = 220.f * 0.017453292519943295f;

    printf("Creating spherical image %d:%d\n", out_w, out_h);

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

    fish2sphere((uint32_t*)data, x, y, fov, output, out_w, out_h);

    fs::path in_path(args[1]);
    fs::path out_path(in_path.parent_path() / "spherical.png");

    int result = stbi_write_png(out_path.string().c_str(), out_w, out_h, 4, output, out_w * sizeof(uint32_t));
    if (!result) {
        fprintf(stderr, "Unable to save spherical.png\n");
        return -1;
    }

    free(output);
    stbi_image_free(data);

    return 0;
}