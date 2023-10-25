#ifndef DESERIALIZE_FROM_BMP_H
#define DESERIALIZE_FROM_BMP_H

#include "Image.h"
#include <stdio.h>  // Добавлено для использования FILE

enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
    /* коды других ошибок  */
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};

enum write_status to_bmp(FILE* out, const struct image* img);

enum read_status from_bmp(FILE* in, struct image* img);

#endif
