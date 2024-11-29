/* Documentation
Name: Yash Thakur (24021_322)
Date: 21/11/2024
Description: The LSB Image Steganography project, implemented in C, involves hiding secret data within 
             an image by modifying the least significant bits of its pixel values. This technique ensures
             the hidden data is invisible to the human eye while preserving the image's appearance, showcasing
             skills in C programming, bitwise operations, and secure data handling.
             -> Using the C programming language, the project includes:
                    -Encoding: Embedding hidden data (e.g., text) into an image by altering its LSBs.
                    -Decoding: Extracting the hidden information from the modified image.
*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    //if Argument count is less than 2 
    if (argc < 2)
    {
        printf("Please pass the proper Arguments\n");
    }
    else if (argc < 3)
    {
        printf("Error: Unable To Perform Operation. Arguments Should be > 3\n");
        printf("For Encoding Arguemnts Should be:\n./a.out -e <sourcefile.bmp> <secretfile.txt> [outputfile.bmp]\n");
        printf("For Decoding Arguments should be:\n./a.out -d <sourcefile.bmp> [outputfile.txt]\n");
    }
    else
    {
        //Based ont the user input sstart operation
        int type = check_operation_type(argv);
        //Encoding(-e)
        if (type == e_encode)
        {
            printf("Encoding started\n");

            EncodeInfo encInfo;
            uint img_size;

            //if Read and validate succesfull return prompt message and GO to next step
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                
                printf("Arugments Verified Successfully\n");

                // Test open_files
                if (open_files(&encInfo) == e_failure)
                {
                    printf("ERROR: %s function failed\n", "open_files");
                    return 1;
                }
                else
                {
                    printf("SUCCESS: %s function completed\n", "open_files");
                    if (do_encoding(&encInfo) == e_success)
                    {
                        printf("Encoding is completed\n");
                    }
                    else
                    {
                        printf("ERROR: Encoding fail\n");
                    }
                }
            }

            // Test get_image_size_for_bmp
            // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
            // printf("INFO: Image size = %u\n", img_size);
        }
        //Decoding (-d)
        else if (type == e_decode)
        {
            printf("Decoding is Started\n");
            DecodeInfo decInfo;

            /*// Fill with sample filenames
              encInfo.src_image_fname = "beautiful.bmp";
              encInfo.secret_fname = "secret.txt";
              encInfo.stego_image_fname = "stego_img.bmp";
             */
            if (read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                // function call for decoding
                if (do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding is Completed\n");
                }
                else
                {
                    printf("Decoding Failed\n");
                }
            }
        }
        else
        {
             printf("Error: Unable To Perform Operation. Arguments Should be > 3\n");
             printf("For Encoding Arguemnts Should be:\n./a.out -e <sourcefile.bmp> <secretfile.txt> [outputfile.bmp]\n");
             printf("For Decoding Arguments should be:\n./a.out -d <sourcefile.bmp> [outputfile.txt]\n");
        }
    }
    return 0;
}
