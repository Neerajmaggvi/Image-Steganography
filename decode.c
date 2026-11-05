#include <stdio.h>
#include "decode.h"
#include "common.h"
#include "color.h"

// Read and validate Decode args from argv 
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
            decInfo->output_fname = argv[3];
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

// Get file pointers for input and output files 
Status open_decode_files(DecodeInfo *decInfo)
{
    // Step 1: Open the stego image in read-binary mode
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb"); // Opening the file in read mode

    if(decInfo->fptr_stego_image == NULL) // If file didn't open it's NULL
    {
        perror("fopen");
        return e_failure;
    }

    if(fseek(decInfo->fptr_stego_image, 54, SEEK_SET) != 0) // If it failed to move 54 bytes  
    {
        fclose(decInfo->fptr_stego_image);
        decInfo->fptr_stego_image = NULL;
        return e_failure;
    }

    return e_success;
}

// Decode Magic String 
Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[8];
    char decoded_data;

    for(int i = 0; i < 2; i++)
    {
        if(fread(buffer, sizeof(char), 8, decInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }

        decode_byte_from_lsb(buffer, &decoded_data);

        decInfo -> magic_string[i] = decoded_data;
    }

    decInfo -> magic_string[2] = '\0';

    if(strcmp(decInfo -> magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }

    return e_failure;
}

// Decode secret file extension size 
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int extn_size_temp;

    if(fread(buffer, sizeof(char), 32, decInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }

    if(decode_size_from_lsb(buffer, &extn_size_temp) != e_success)
    {
        return e_failure;
    }

    decInfo -> extn_size = extn_size_temp;

    return e_success;
}

// Decode secret file extension 
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    char decode_data;

    for(int i = 0; i < decInfo->extn_size; i++)
    {
        if(fread(buffer, sizeof(char), 8, decInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }

        if(decode_byte_from_lsb(buffer, &decode_data) != e_success)
        {
            return e_failure;
        }

        decInfo -> extn_secret_file[i] = decode_data;

    }

    decInfo -> extn_secret_file[4] = '\0';

    return e_success;
}

// Decode secret file size 
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int temp_size;

    if(fread(buffer, sizeof(char), 32, decInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }

    if(decode_size_from_lsb(buffer, &temp_size) != e_success)
    {
        return e_failure;
    }

    decInfo -> size_secret_file = temp_size;

    return e_success;
}

// Decode secret file data 
Status decode_secret_file_data(DecodeInfo *decInfo)
{

}

// Decode a byte from LSB of image data array 
Status decode_byte_from_lsb(char *image_buffer, char *data)
{
    unsigned char decode = 0;

    for(int i = 0; i < 8; i++)
    {
        decode = (decode << 1) | (image_buffer[i] & 1);
    }

    *data = decode;

    return e_success;
}

// Decode a size from LSB of image data array 
Status decode_size_from_lsb(char *image_buffer, int *size)
{
    int decode = 0;

    for(int i = 0; i < 32; i++)
    {
        decode = (decode << 1) | (image_buffer[i] & 1);
    }

    *size = decode;

    return e_success;
}

// Perform the decoding 
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\n==========================================================\n");
    printf("             IMAGE STEGANOGRAPHY - DECODING\n");
    printf("==========================================================\n\n");

    // Step 1: Open the stego image and skip the BMP header
    printf("\n[1] FILE OPENING STATUS\n");

    Status check_file_open = open_decode_files(decInfo);

    if(check_file_open == e_failure)
    {
        fprintf(stderr, RED "    ERROR: FILE OPEN FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    SUCCESS: FILE OPENED SUCCESSFULLY.\n" RESET);

    // Step 2: Decode and validate the magic string
    printf("\n[2] MAGIC STRING VERIFICATION STATUS\n");

    Status check_magic_string = decode_magic_string(decInfo);

    if(check_magic_string == e_failure)
    {
        fprintf(stderr, RED "    ERROR: MAGIC STRING NOT MATCHING.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    SUCCESS: MAGIC STRING MATCHING.\n" RESET);   

    // Step 3: Decode the secret file extension size
    printf("\n[3] SECRET FILE EXTENSION SIZE STATUS\n");

    Status check_extn_size = decode_secret_file_extn_size(decInfo);

    if(check_extn_size == e_failure)
    { 
        fprintf(stderr, RED "    ERROR: SECRET FILE SIZE DECODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    SUCCESS: SECRET FILE SIZE DECODING SUCCESSFULL.\n" RESET);     

    // Step 4: Decode the secret file extension
    printf("\n[4] SECRET FILE EXTENSION STATUS\n");

    Status check_secret_extn = decode_secret_file_extn(decInfo);

    if(check_secret_extn == e_failure)
    {
        fprintf(stderr, RED "    ERROR: SECRET FILE EXTENSION DECODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    SUCCESS: SECRET FILE EXTENSION DECODING SUCCESSFULL.\n" RESET);
    
    // Step 5: Decode the secret file size
    printf("\n[5] SECRET FILE SIZE EXTENSION\n");

    Status check_secret_file_size = decode_secret_file_size(decInfo);

    if(check_secret_file_size == e_failure)
    {
        fprintf(stderr, RED "    ERROR: SECRET FILE SIZE DECODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    SUCCESS: SECRET FILE SIZE DECODING SUCCESSFULL.\n" RESET);

    // Step 6: Construct the output filename

    // Step 7: Open the output file

    // Step 8: Decode the secret file data

    // Step 9: Close the opened files

    return e_success;
}