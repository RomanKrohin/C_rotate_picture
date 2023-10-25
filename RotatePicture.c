#include "RotatePicture.h"
#include <stdlib.h> 

struct image rotate(struct image const source){
    struct image rotated_img = (struct image){.width = source.width, .height = source.height};
    rotated_img.data = (struct pixel*)malloc(sizeof(struct pixel) * rotated_img.width * rotated_img.height);

    for (uint16_t y = 0; y < source.height; y++)
    {
        for (uint16_t x = 0; x < source.width; x++)
        {
            uint16_t rotated_x = rotated_img.width - y - 1;
            uint16_t rotated_y = x;

            if (rotated_x < rotated_img.width && rotated_y < rotated_img.height)
            {
                rotated_img.data[rotated_y * rotated_img.width + rotated_x] = source.data[y * source.width + x];
            }
        }
    }

    return rotated_img;
}