#include "RotatePicture.h"
#include <stdlib.h> 

struct image rotate(struct image source){
    struct image rotated_img = (struct image){.width = source.height, .height = source.width};
    rotated_img.data = (struct pixel*)malloc(sizeof(struct pixel) * rotated_img.width * rotated_img.height);

    for (uint16_t y = 0; y < source.height; y++){
        for (uint16_t x = 0; x < source.width; x++){
            uint16_t rotated_x = source.height - y - 1;
            uint16_t rotated_y = x;

                rotated_img.data[rotated_y * rotated_img.width + rotated_x] = source.data[y * source.width + x];
        }
    }

    free (source.data);
    return rotated_img;
}
