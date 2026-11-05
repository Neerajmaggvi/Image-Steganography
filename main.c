#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include"color.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    // Check the operation the user wants to perform
    OperationType operation = check_operation_type(argv[1]);

    if(operation == e_encode)
    {

        // Check the command line arguments are correct or not
        Status valid_CLA = read_and_validate_encode_args(argv, &encInfo);

        if(valid_CLA == e_failure)
        {
            return e_failure;
        }

        printf("\n[1] COMMAND-LINE ARGUMENT VALIDATION\n");
        printf(GREEN "    SUCCESS: ARGUMENTS VALIDATED.\n" RESET);

        // Check if all the functions in the encoding are proper or not
        Status encoding_result = do_encoding(&encInfo);

        if(encoding_result == e_failure)
        {
            return e_failure;
        }

        return e_success;
    }

    
    if(operation == e_decode)
    {
        Status valid_CLA = read_and_validate_decode_args(argc, argv, &decInfo);

        if(valid_CLA == e_failure)
        {
            return e_failure;
        }

        printf("\n[2] COMMAND-LINE ARGUMENT VALIDATION\n");
        printf(GREEN "    SUCCESS: ARGUMENTS VALIDATED.\n" RESET);

        Status decoding_result = do_decoding(&decInfo);

        if(decoding_result == e_failure)
        {
            return e_failure;
        }

        return e_success;

    }
    
}

OperationType check_operation_type(char *symbol)
{
    //check symbol is -e or not
    char *check_encode = strstr(symbol, "-e");

    if(check_encode != NULL)
    {
        if(strcmp(check_encode, "-e") == 0)
        {
            return e_encode;
        }
    }
    
    // check symbol is -d or not
    char *check_decode = strstr(symbol, "-d");

    if(check_decode != NULL)
    {
        if(strcmp(check_decode, "-d") == 0)
        {
            return e_decode;
        }
    }

    return e_unsupported;

}
