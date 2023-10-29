#include "ToFromBMP.h"
#include "RotatePicture.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    struct image* img =  (struct image*) malloc(sizeof(struct image));
    FILE* file1 = fopen(argv[1], "rb"); 
    FILE* file2 = fopen(argv[2], "wb"); 

    if (from_bmp(file1, img) == READ_OK){
        *img = rotate(*img);
        if (to_bmp(file2, img) == WRITE_OK){
            printf("Success");
        }
    }
    fclose(file1);
    fclose(file2);
    free(img);
    return 0;
}