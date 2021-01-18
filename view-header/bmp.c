#include "bmp.h"
#include "bmp_struct.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


bool read_header( FILE* f, struct bmp_header* header ) {
    return fread( header, sizeof( struct bmp_header ), 1, f );
}

enum read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header h = { 0 };
    uint8_t pixel_size = sizeof (struct pixel);
    uint32_t pad;
    if (!read_header(in, &h) || h.biBitCount != 24 || h.bfType != 19778) return READ_INVALID_HEADER;
    if (fseek(in, h.bOffBits, SEEK_SET) != 0) return READ_INVALID_BITS;
    img->width = h.biWidth;
    img->height = h.biHeight;
    img->data = malloc(h.biWidth * h.biHeight * sizeof (struct pixel));
    if (((img->width * sizeof(struct pixel)) % 4) != 0) {
        pad = 4 - ((img->width * sizeof(struct pixel)) % 4);
    }else {
        pad = 0;
    }
    for (size_t i = 0; i < img->height; ++i) {
        if (fread(img->data + i * img->width, sizeof (struct pixel), img->width, in) < pixel_size) return READ_INVALID_BITS;
        if (fseek(in, pad, SEEK_CUR) != 0) return READ_INVALID_BITS;
    }
    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img) {
    uint32_t header_size = sizeof (struct bmp_header);
    uint32_t pixel_size = sizeof (struct pixel);
    uint32_t pad;
    struct bmp_header h =
            {
                    .bfType = 19778,
                    .biBitCount = 24,
                    .biHeight = img->height,
                    .biWidth = img->width,
                    .bOffBits = sizeof(struct bmp_header),
                    .bfileSize = sizeof(struct bmp_header) + (sizeof(struct pixel) * img->width + img->width % 4) * img->height,
                    .biSizeImage = img->width * img->height * sizeof(struct pixel),
                    .biSize = 40,
                    .biPlanes = 1
            };
    if (!fwrite(&h, header_size, 1, out)) return WRITE_ERROR;
    char paddingBytes[3] = { 0 };
    if (((img->width * sizeof(struct pixel)) % 4) != 0) {
        pad = 4 - ((img->width * sizeof(struct pixel)) % 4);
    }else {
        pad = 0;
    }
    for (size_t i = 0; i < img->height; ++i) {
        if (fwrite(img->data + i * img->width, pixel_size, img->width, out) != img->width) return WRITE_ERROR;
        if (fwrite(paddingBytes, pad, 1, out) != 1 && pad != 0) return WRITE_ERROR;
    }
    return WRITE_OK;

}
