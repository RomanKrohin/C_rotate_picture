#include "ToFromBMP.h"
#include "RotatePicture.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    struct image* img =  (struct image*) malloc(sizeof(struct image));
    FILE* file1 = fopen("image1.bmp", "rb"); 
    FILE* file2 = fopen("image2.bmp", "wb"); 
    from_bmp(file1, img);
    *img = rotate(*img);
    to_bmp(file2, img);
    fclose(file1);
    fclose(file2);
    free(img);
    return 0;
}