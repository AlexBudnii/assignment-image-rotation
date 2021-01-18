#ifndef _BMP_H_
#define _BMP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct pixel { uint8_t b,g,r; };
struct image {
    uint64_t width, height;
    struct pixel* data;
};
enum read_status {
    READ_OK = 0,
    READ_ERROR,
    READ_INVALID_HEADER,
    READ_INVALID_BITS
};
enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
};
enum read_status from_bmp(FILE* in, struct image* pic);
enum write_status to_bmp(FILE* out, struct image const* pic);

#endif
