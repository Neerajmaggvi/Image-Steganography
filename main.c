#include <stdio.h>
#include "encode.h"
#include "types.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[]) // .aout -e <source_file> <secret_data_file> <optional>
{
    //step 1 -> check check_operation_type(argv[1]) is returning e_encode or not
        // yes -> EncodeInfo encInfo;
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
    
    // step 2 -> check symbol is -d or not
        // yes -> return e_decode

    // return e_unsupported

}
