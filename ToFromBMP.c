#include "Image.h"
#include "BMPHeader.h"
#include "ToFromBMP.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum read_status from_bmp(FILE* in, struct image* img)
{
    if (in == NULL){
        return READ_INVALID_SIGNATURE;
    }

    size_t bytesRead = fread(&bmp_header_local, sizeof(bmp_header_local), 1, in);
    if (bytesRead != 1){
        return READ_INVALID_HEADER;
    }

    img->height = bmp_header_local.biHeight;
    img->width = bmp_header_local.biWidth;

    uint32_t rowSize = (img->width * sizeof(struct pixel));
    uint32_t paddingSize = (4 - (rowSize % 4)) % 4;

    img->data = (struct pixel*)malloc(sizeof(struct pixel) * img->width * img->height);

    if (img->data == NULL){
        return READ_INVALID_BITS;
    }

    fseek(in, bmp_header_local.bOffBits, SEEK_SET);
    
    for (uint16_t y = 0; y < img->height; y++){
        fread(&(img -> data[y * img -> width]),sizeof(struct pixel),img -> width,in);
        fseek(in, paddingSize, SEEK_CUR);
    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image* img){
    if (out == NULL){
        return WRITE_ERROR;
    }

    bmp_header_local.biWidth = img -> width;
    bmp_header_local.biHeight = img -> height;
    
    fwrite(&bmp_header_local, sizeof(struct bmp_header), 1, out);

    uint32_t rowSize = (img->width * sizeof(struct pixel));
    uint32_t paddingSize = (4 - (rowSize % 4)) % 4;

    for (uint16_t y = 0; y < img->height; y++){
        fwrite(&(img -> data[y * img -> width]), sizeof(struct pixel), img -> width, out);

        for (uint32_t i = 0; i < paddingSize; i++){
            uint8_t padding = 8;
            fwrite(&padding, sizeof(uint8_t), 1, out);
        }
    }

    free(img -> data);
    return WRITE_OK;
}
