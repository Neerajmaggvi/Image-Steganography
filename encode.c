#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "color.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("    IMAGE WIDTH  : %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("    IMAGE HEIGHT : %u\n", height);

    // Return image capacity
    return width * height * 3;
}

long get_file_size(FILE *fptr)
{
    // Find the size of secret file data

    // fseek is used to treverse the file pointer to the end. 
    fseek(fptr, 0, SEEK_END);

    // ftell is used to return the number of bytes till the file pointer current position.
    long secret_file_size = ftell(fptr);

    rewind(fptr); // Get the file pointer back

    // return secret file size
    return secret_file_size;

}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // step 1 -> check argv[2] is having .bmp or not
    char *check_bmp = strstr(argv[2], ".bmp");

    if(check_bmp != NULL)
    {
        if(strcmp(check_bmp, ".bmp") == 0)
        {
            encInfo -> src_image_fname = argv[2];
        }
        else
        {
            printf(RED "ERROR: Invalid source image extension. Expected .bmp\n" RESET);
            return e_failure;
        }
    }
    else
    {
        printf(RED "ERROR: Source image extension is missing. Expected .bmp\n" RESET);
        return e_failure;
    }

    // step 2 -> check argv[3] is having extn there or not 
    char *check_extn = strstr(argv[3], ".txt");

    if(check_extn != NULL)
    {
        if(strcmp(check_extn, ".txt") == 0)
        {
            encInfo -> secret_fname = argv[3];
            strcpy(encInfo->extn_secret_file, check_extn);
        }
        else
        {
            printf(RED "ERROR: Invalid secret file extension. Expected .txt\n" RESET);
            return e_failure;
        }
    }
    else
    {
        printf(RED "ERROR: Secret file extension is missing. Expected .txt\n" RESET);
        return e_failure;
    }

    // step 3 -> check argv[4] is given is the CLA or default name has to be given.
    if(argv[4] != NULL)
    {
        char *check_dst_extn = strstr(argv[4], ".bmp");

        if(check_dst_extn != NULL)
        {
            if(strcmp(check_dst_extn, ".bmp") == 0)
            {
                encInfo -> stego_image_fname = argv[4];
            }
            else
            {
                printf(RED "ERROR: Destination image extension is missing. Expected .bmp\n" RESET);
                return e_failure;   
            }
        }
        else
        {
            printf(RED "ERROR: Destination image extension is missing. Expected .bmp\n" RESET);
            return e_failure;
        }
    }
    else
    {
        printf(BLUE "Default name added\n" RESET);
        encInfo -> stego_image_fname = "stego.bmp";
    }

    // After all the validation with no errors.
    return e_success;

}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,RED "ERROR: Unable to open file %s\n" RESET, encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // Magic string     = 2 × 8 = 16 bits
    // Stores the predefined identifier (#*) to identify encoded data.

    // Extension size   = 4 × 8 = 32 bits
    // Stores the length of the secret file extension (.txt).
    // Example: 4 characters.

    // Extension        = 4 × 8 = 32 bits
    // Stores the actual secret file extension (.txt).

    // Secret size      = 4 × 8 = 32 bits (We have to store 9 that is a integer that's why we write 4 x 8. Int needs 4 bytes as storage.)
    // Stores the total size of the secret file contents in bytes.
    // Example: 9 bytes for "HELLO1234".

    // Secret data      = 9 × 8 = 72 bits
    // Stores the actual contents of the secret file.
    // Example: "HELLO1234".

    // Find the capacity of the source file 
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

    // Find the size of the secret data that has to encode into source file
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);


    // Checking if the image capacity is greater that required bits to encode.
    int required_bits = 16 + 32 + (strlen(encInfo->extn_secret_file) * 8) + 32 + (encInfo->size_secret_file * 8);
    
    if(encInfo -> image_capacity >= required_bits)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
    
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // step 1 -> rewind source file pointer
    rewind(fptr_src_image);

    // declare one character buffer of size 54
    char buffer[54];

    // step 2 -> read 54 bytes from source file fread()
    if(fread(buffer, sizeof(char), 54, fptr_src_image) != 54)
    {
        return e_failure;
    }

    // step 3 -> write 54 bytes to dest file fwrite()
    if(fwrite(buffer, sizeof(char), 54, fptr_dest_image) != 54)
    {
        return e_failure;
    }

    return e_success;

}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int magic_str_len = strlen(magic_string);

    for(int i = 0; i < magic_str_len; i++)
    {
        char buffer[8];

        //step 1 -> read 8 bytes from source file
        if(fread(buffer, sizeof(char), 8, encInfo -> fptr_src_image) != 8)
        {
            return e_failure;
        }

        //step 2 -> call encode_byte_to_lsb(magic_string[i], buffer)
        Status check_encoding = encode_byte_to_lsb(magic_string[i], buffer);

       if(check_encoding == e_success)
       {
            //step 3 -> write the buffer into dest file
            if(fwrite(buffer, sizeof(char), 8, encInfo -> fptr_stego_image) != 8)
            {
                return e_failure;
            }
       }
       else
       {
            return e_failure;
       }
    }

    return e_success;
}

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];

    //step 1 -> read 32 bytes from source file
    if(fread(buffer, sizeof(char), 32, encInfo -> fptr_src_image) != 32)
    {
        return e_failure;
    }

    //step 2 -> call encode_size_to_lsb(size, buffer)
    encode_size_to_lsb(size, buffer);

    //step 3 -> write the buffer into dest file
    if(fwrite(buffer, sizeof(char), 32, encInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];

    for(int i = 0; i < strlen(file_extn); i++)
    {
        //step 1 -> read 8 bytes from source file
        if(fread(buffer, sizeof(char), 8, encInfo -> fptr_src_image) != 8)
        {
            return e_failure;
        }

        //step 2 -> call encode_byte_to_lsb(file_extn[i], buffer)
        encode_byte_to_lsb(file_extn[i], buffer);

        //step 3 -> write the buffer into dest file
        if(fwrite(buffer, sizeof(char), 8, encInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }

    return e_success;  
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    //step 1 -> read 32 bytes from source file
    if(fread(buffer, sizeof(char), 32, encInfo -> fptr_src_image) != 32)
    {
        return e_failure;
    }

    //step 2 -> call encode_size_to_lsb(file_size, buffer)
    encode_size_to_lsb(file_size, buffer);

    //step 3 -> write the buffer into dest file
    if(fwrite(buffer, sizeof(char), 32, encInfo -> fptr_stego_image) != 32)
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Move pointer to beginning of secret file
    rewind(encInfo -> fptr_secret);

    //step 1 -> read secret data from file and store it into encInfo -> secret_data
    fread(encInfo -> secret_data, sizeof(char), encInfo -> size_secret_file, encInfo -> fptr_secret);

    char buffer[8];

    for(int i = 0; i < encInfo -> size_secret_file; i++)
    {
        //step 2 -> read 8 bytes from source file
        if(fread(buffer, sizeof(char), 8, encInfo -> fptr_src_image) != 8)
        {
            return e_failure;
        }

        //step 3 -> call encode_byte_to_lsb(encInfo -> secret_data[i], buffer)
        encode_byte_to_lsb(encInfo -> secret_data[i], buffer);

        //step 4 -> write the buffer into dest file
        if(fwrite(buffer, sizeof(char), 8, encInfo -> fptr_stego_image) != 8)
        {
            return e_failure;
        }

    }

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    while(fread(&ch, 1, 1, fptr_src) == 1)   // Read remaining bytes
    {
        fwrite(&ch, 1, 1, fptr_dest);       // Write to destination
    }

    if(ftell(fptr_src) == ftell(fptr_dest))  // Check if both pointers match
    {
        return e_success;
    }
    return e_failure;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int j = 0;

    for(int i = 7; i >= 0; i--)
    {
        uint get = (data & (1 << i)) >> i;
        image_buffer[j] = image_buffer[j] & (~1);
        image_buffer[j] = image_buffer[j] | get;

        j++;
    }

    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    // write logic to encode the int
    encode_byte_to_lsb((size >> 24) & 0xFF, imageBuffer);
    encode_byte_to_lsb((size >> 16) & 0xFF, imageBuffer + 8);
    encode_byte_to_lsb((size >> 8) & 0xFF, imageBuffer + 16);
    encode_byte_to_lsb((size) & 0xFF, imageBuffer + 24);

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("\n==========================================================\n");
    printf("             IMAGE STEGANOGRAPHY - ENCODING\n");
    printf("==========================================================\n\n");


    //step 1 -> check open_files(encInfo) is returning e_success or not
    Status for_open_file = open_files(encInfo);

    printf("\n[1] FILE OPENING STATUS\n");
    if(for_open_file == e_failure)
    {
        fprintf(stderr, RED "    ERROR: FILE OPEN FAILED.\n" RESET);
        return e_failure;
    }
    else
    {
        printf(GREEN "    SUCCESS: ALL FILES OPENED.\n" RESET);
    }

    //step 2 -> check check_capacity(encInfo) is returning e_success or not
    printf("\n[2] IMAGE CAPACITY CHECK\n");

    Status for_checking_capacity = check_capacity(encInfo);

    if(for_checking_capacity == e_failure)
    {
        fprintf(stderr, RED "    ERROR: INSUFFICIENT IMAGE CAPACITY.\n" RESET);
        return e_failure;
    }
    else
    {
        printf(GREEN "    STATUS: SUFFICIENT CAPACITY.\n" RESET);
    }


    //step 3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
    printf("\n[3] COPY HEADER CHECK\n");

    Status for_checking_header_copy = copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    if(for_checking_header_copy == e_failure)
    {
        return e_failure;
    }
    else
    {
        printf(GREEN "    STATUS: HEADER COPIED SUCCESSFULLY.\n" RESET);  
    }

    //step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)
    printf("\n[4] MAGIC STRING ENCODING\n");

    Status check_magic_str_encoding = encode_magic_string(MAGIC_STRING, encInfo);

    if(check_magic_str_encoding == e_failure)
    {
        fprintf(stderr, RED "    ERROR: MAGIC STRING ENCODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    STATUS: MAGIC STRING ENCODED SUCCESSFULLY.\n" RESET);

    //step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)
    printf("\n[5] ENCODING SECRET FILE EXTN SIZE\n");

    Status check_extn_size_encode = encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo);

    if(check_extn_size_encode == e_failure)
    {
        fprintf(stderr, RED "    ERROR: FILE EXTENSION SIZE ENCODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    STATUS: FILE EXTENSION SIZE ENCODED SUCCESSFULLY.\n" RESET);

    //step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)
    printf("\n[6] ENCODING SECRET FILE EXTENSION\n");

    Status check_extn_encode = encode_secret_file_extn(encInfo -> extn_secret_file, encInfo);

    if(check_extn_encode == e_failure)
    {
        fprintf(stderr, RED "    ERROR: FILE EXTENSION ENCODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    STATUS: FILE EXTENSION ENCODED SUCCESSFULLY.\n" RESET);


    //step 7 -> call encode_secret_file_size(encInfo ->  size_secret_file, encInfo)
    printf("\n[7] ENCODING SECRET FILE SIZE\n");

    Status check_file_size_encode = encode_secret_file_size(encInfo ->  size_secret_file, encInfo);

    if(check_file_size_encode == e_failure)
    {
        fprintf(stderr, RED "    ERROR: SECRET FILE SIZE ENCODING FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    STATUS: SECRET FILE SIZE ENCODED SUCCESSFULLY.\n" RESET);

    //step 8 -> call encode_secret_file_data(encInfo)
    printf("\n[8] ENCODING SECRET FILE DATA\n");

    Status check_encode_secret_file = encode_secret_file_data(encInfo);

    if(check_encode_secret_file ==  e_failure)
    {
        fprintf(stderr, RED "    ERROR: SECRET FILE DATA ENCODING FAILED.\n" RESET);
        return e_failure;   
    }

    printf(GREEN "    STATUS: SECRET FILE DATA ENCODED SUCCESSFULLY.\n" RESET);

    //step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
    printf("\n[9] COPYING THE REMAINING DATA\n");

    Status check_final_copy = copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    if(check_final_copy == e_failure)
    {
        fprintf(stderr, RED "    ERROR:  COPYING THE REMAINING DATA FAILED.\n" RESET);
        return e_failure;
    }

    printf(GREEN "    STATUS: COPYING REMAINING DATA FROM SOURCE FILE COMPLETED!.\n" RESET);

    return e_success;

}