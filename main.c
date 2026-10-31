#include <stdio.h>
#include "encode.h"
#include "types.h"
#include"color.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[]) // .aout -e <source_file> <secret_data_file> <optional>
{
    //step 1 -> check check_operation_type(argv[1]) is returning e_encode or not
        // yes -> EncodeInfo encInfo;
    int information = check_operation_type(argv[1]);
    if(information == e_encode)
    {
        EncodeInfo encInfo;

        int valid_CLA = read_and_encode_args(argv, &encInfo);

        if(valid_CLA == e_success)
        {
            printf(GREEN "Files are all set!\t ENCODING STARTED\n" RESET);
        }
        else
        {
            return 0;
        }
    }
            // check read_and_validate_encode_args(argv, &encInfo) is returning e_success or e_failure
                // failure -> print error msg and stop
                // success -> print success msg 
                            // check do_encoding(&encInfo) is returning e_success or e_failure
                                    // failure -> print error msg and stop
                                    // success -> print success msg and stop
}

OperationType check_operation_type(char *symbol)
{
    //step 1 -> check symbol is -e or not
        // yes -> return e_encode
    char *check_encode = strstr(symbol, "-e");

    if(check_encode != NULL)
    {
        if(strcmp(check_encode, "-e") == 0)
        {
            return e_encode;
        }
    }
    
    // step 2 -> check symbol is -d or not
        // yes -> return e_decode
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
