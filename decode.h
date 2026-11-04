#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>

#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Information */
    char *stego_image_fname;       // Name of the encoded BMP image
    FILE *fptr_stego_image;        // File pointer to read the encoded image

    /* Decoded Secret File Information */
    char magic_string[3];          // Stores decoded magic string (#*)
    int extn_size;                 // Stores the decoded secret file extension size
    char extn_secret_file[5];      // Stores the decoded extension (e.g., .txt)
    int size_secret_file;          // Stores the decoded secret file size in bytes

    /* Output File Information */
    char *output_fname;            // Name given by the user for the output file
    FILE *fptr_output;             // File pointer to write the decoded secret data

} DecodeInfo;

/* Decoding function prototypes */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers for input and output files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *image_buffer, char *data);

/* Decode a size from LSB of image data array */
Status decode_size_from_lsb(char *image_buffer, int *size);

#endif