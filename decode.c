#include <stdio.h>
#include "decode.h"
#include "common.h"
#include "color.h"

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    // check if there are only four argc or not
    if(argc != 4)
    {
        fprintf(stderr, RED "ERROR: Invalid number of arguments.\n" RESET);
        return e_failure;
    }

    // check argv[2] is having .bmp or not
    char *check_bmp = strstr(argv[2], ".bmp");

    if(check_bmp != NULL)
    {
        if(strcmp(check_bmp, ".bmp") == 0)
        {
            decInfo -> stego_image_fname = argv[2];
        }
        else
        {
            fprintf(stderr, RED "ERROR: Invalid stego image file. Expected a .bmp file.\n" RESET);
            return e_failure;
        }
    }
    else
    {
        fprintf(stderr, RED "ERROR: Invalid stego image file. Expected a .bmp file.\n" RESET);
        return e_failure;
    }

    return e_success;
}

/* Get file pointers for input and output files */
Status open_decode_files(DecodeInfo *decInfo)
{

}

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo)
{

}

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{

}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{

}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{

}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{

}

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *image_buffer, char *data)
{

}

/* Decode a size from LSB of image data array */
Status decode_size_from_lsb(char *image_buffer, int *size)
{

}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{

}