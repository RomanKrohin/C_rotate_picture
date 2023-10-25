#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>


#pragma pack(push, 1)
struct bmp_header 
{
    uint16_t bfType;
    uint32_t  bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t  biHeight;
    uint16_t  biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t  biClrImportant;
};
#pragma pack(pop)

struct pixel
{
    uint8_t b, r, g;
};


struct image
{
    uint16_t width, height;
    struct pixel* data;
};

struct bmp_header bmp_header;

//------- FROM BMP -------

enum read_status  {
  READ_OK = 0,
  READ_INVALID_SIGNATURE,
  READ_INVALID_BITS,
  READ_INVALID_HEADER
  /* коды других ошибок  */
  };

enum read_status from_bmp( FILE* in, struct image* img ){
    if (in == NULL){
        return READ_INVALID_SIGNATURE;
    }

    size_t bytesRead = fread(&bmp_header, sizeof(bmp_header), 1, in);
    if (bytesRead != 1) {
        return READ_INVALID_HEADER;
    }

    img -> height = (uint16_t) bmp_header.biHeight;
    img -> width = (uint16_t) bmp_header.biWidth;
    
    img -> data = (struct pixel*) malloc (sizeof(struct pixel) * img -> width * img -> height);  

    if (img->data == NULL) {
        return READ_INVALID_BITS;
    }


    fseek(in, bmp_header.bOffBits, SEEK_SET);

    for (uint16_t y=0; y< img -> height; y++){
        for (uint16_t x=0; x< img -> width; x++){

            struct pixel new_pixel;

            fread(&new_pixel.b, sizeof(uint8_t), 1, in);
            fread(&new_pixel.r, sizeof(uint8_t), 1, in);
            fread(&new_pixel.g, sizeof(uint8_t), 1, in);

            img -> data[y * img -> width + x] =  new_pixel;
        }
    }
    
    return READ_OK;
};

//------- FROM BMP -------

struct image rotate(struct image const source){

    struct image rotated_img =  (struct image) {.width = source.width, .height = source.height};

    rotated_img.data = (struct pixel*)malloc(sizeof(struct pixel) * rotated_img.width * rotated_img.height);

    for (uint16_t y = 0; y < source.height; y++){
        for (uint16_t x = 0; x < source.width; x++){

            uint16_t rotated_x = rotated_img.width - y - 1;
            uint16_t rotated_y = x;

            if (rotated_x < rotated_img.width && rotated_y < rotated_img.height) {

                rotated_img.data[rotated_y * rotated_img.width + rotated_x] = source.data[y * source.width + x];
            }        
        }
    }

    return rotated_img;
}


//------- TO BMP -------

enum  write_status  {
  WRITE_OK = 0,
  WRITE_ERROR
  /* коды других ошибок  */
};

enum write_status to_bmp( FILE* out, struct image const* img ){

    if (out == NULL){
        return WRITE_ERROR;
    }
    
    fwrite(&bmp_header, sizeof(struct bmp_header), 1, out);

    for (uint16_t y=0; y< img -> height; y++){
        for (uint16_t x=0; x< img -> width; x++){
            struct pixel pixel = img->data[y * img->width + x];
            fwrite(&pixel.b, sizeof(uint8_t), 1, out);
            fwrite(&pixel.g, sizeof(uint8_t), 1, out);
            fwrite(&pixel.r, sizeof(uint8_t), 1, out);
        }

        unsigned int padding = (4 - (img->width * 3) % 4) % 4;
        uint8_t dummy = 0;
        fwrite(&dummy, sizeof(uint8_t), padding, out);
    }

    free(img -> data);

    return WRITE_OK;
};

//------- TO BMP -------

int main(){
    struct image* img =  (struct image*) malloc(sizeof(struct image));
    FILE* file1 = fopen("image1.bmp", "rb"); 
    FILE* file2 = fopen("image2.bmp", "wb"); 
    if (from_bmp(file1, img)==READ_OK){
        *img = rotate(*img);
       if (to_bmp(file2, img)==WRITE_OK){
            printf("Success");
        }
    };
    fclose(file1);
    fclose(file2);
    free(img);
    return 0;
}
