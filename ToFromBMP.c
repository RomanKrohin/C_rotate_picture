#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Image.h"
#include "BMPHeader.h"
#include "ToFromBMP.h"

enum read_status from_bmp(FILE* in, struct image* img)
{
    if (in == NULL)
    {
        return READ_INVALID_SIGNATURE;
    }

    size_t bytesRead = fread(&bmp_header_local, sizeof(bmp_header_local), 1, in);
    if (bytesRead != 1)
    {
        return READ_INVALID_HEADER;
    }

    img->height = abs((int32_t)bmp_header_local.biHeight);
    img->width = bmp_header_local.biWidth;

    uint32_t rowSize = (img->width * sizeof(struct pixel));
    uint32_t paddingSize = rowSize % 4;
    img->data = (struct pixel*)malloc(sizeof(struct pixel) * img->width * img->height);

    if (img->data == NULL)
    {
        return READ_INVALID_BITS;
    }

    int64_t buf;

    buf = bmp_header_local.biWidth;
    bmp_header_local.biWidth = bmp_header_local.biHeight;
    bmp_header_local.biHeight = buf;

    buf = bmp_header_local.biXPelsPerMeter;
    bmp_header_local.biXPelsPerMeter = bmp_header_local.biYPelsPerMeter;
    bmp_header_local.biYPelsPerMeter = buf;

    fseek(in, bmp_header_local.bOffBits, SEEK_SET);

    buffer = bmp_header_local.bOffBits - 54;

    for (uint16_t y = 0; y < img->height; y++)
    {
        for (uint16_t x = 0; x < img->width; x++)
        {
            struct pixel new_pixel;
            fread(&(new_pixel.b), sizeof(uint8_t), 1, in);
            fread(&(new_pixel.g), sizeof(uint8_t), 1, in);
            fread(&(new_pixel.r), sizeof(uint8_t), 1, in);
            img->data[y * img->width + x] = new_pixel;
        }

        fseek(in, paddingSize, SEEK_CUR);
    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img)
{
    if (out == NULL)
    {
        return WRITE_ERROR;
    }
    
    fwrite(&bmp_header_local, sizeof(struct bmp_header)+buffer, 1, out);
    uint32_t rowSize = (img->width * sizeof(struct pixel));
    uint32_t paddingSize = rowSize % 4;
    
    for (uint16_t y = 0; y < img->height; y++){
        for (uint16_t x = 0; x < img->width; x++){
            struct pixel pixel = img->data[y * img->width + x];
            fwrite(&(pixel.b), sizeof(uint8_t), 1, out);
            fwrite(&(pixel.g), sizeof(uint8_t), 1, out);
            fwrite(&(pixel.r), sizeof(uint8_t), 1, out);
        }

        for (uint32_t i = 0; i < paddingSize; i++)
        {
            uint8_t dummy = 8;
            fwrite(&dummy, sizeof(uint8_t), 1, out);
        }
    }

    free(img->data);

    return WRITE_OK;
}
