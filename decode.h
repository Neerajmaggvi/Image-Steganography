#include<stdio.h>

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