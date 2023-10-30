#include "ToFromBMP.h"
#include "RotatePicture.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
        struct image img =(struct image){ .width=0, .height=0};
        FILE* file1 = (FILE*)fopen(argv[1], "rb"); 
        FILE* file2 = (FILE*)fopen(argv[2], "wb"); 

        if (from_bmp(file1, &img) == READ_OK){
            struct image imgRotated = rotate(img);
            if (to_bmp(file2, &imgRotated) == WRITE_OK){
                printf("Success");
            }
        }
        fclose(file1);
        fclose(file2);
    return 0;
}
