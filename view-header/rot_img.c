#include <malloc.h>
#include <inttypes.h>
#include "rot_img.h"
struct image rotate(struct image const img) {
    const uint64_t w = img.width;
    const uint64_t h = img.height;
    struct image new_img = {.height = w, .width = h};
    new_img.data = (struct pixel*)malloc(sizeof(struct pixel) * h * w);
    for (size_t i = 0; i < h; i++){
        for (size_t j = 0; j < w; j++){
            new_img.data[j * h + i] = img.data[(i + 1) * w - j - 1];
        }
    }
    return new_img;
}
