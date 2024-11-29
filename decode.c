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
#include <string.h>
#include "common.h"
#include "types.h"
#include "decode.h"

//Read and Validate Function
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    printf("Checking Argument Started\n");
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        //store the file name to src
        decInfo->stego_fname = argv[2];
    }
    else
    {
        //print error if the src file is not .bmp
        printf("Error : Difficulty in checking source file extension in .bmp\nDifficulty in checking file extensions.\n");
        return e_failure;
    }

    //create a Output File if user did not mention a Output file name
    if (argv[3] != NULL)
    {
        
        decInfo->decode_fname = argv[3];
    }
    else
    {
        strcpy(decInfo->decode_fname, "decode");
    }

    printf("Arguments Verified Successfully\n");
    return e_success;
}

//Open FIle Function 
Status open_decode_files(DecodeInfo *decInfo)
{
    printf("Opening Source File\n");
    ///open the Src FIle(.bmp) in read only mode 
    decInfo->fptr_stego = fopen(decInfo->stego_fname, "r");
    if (decInfo->fptr_stego == NULL)
    {
        printf("ERROR: Missing image.bmp file\n");
        return e_failure;
    }
    printf("Source File Opened Successfully\n");
    return e_success;
}

//Deocding Lsb to Char Function
char decode_lsb_to_char(DecodeInfo *decInfo)
{
    char buffer[8];
    //read 8 bytes(lsb) from the Src file
    fread(buffer, 8, 1, decInfo->fptr_stego);
    char ch = 0;
    //logic  to decode lsb to char
    for (int i = 0; i  < 8; i++)
    {
        ch = ch | (buffer[i] & 1);
        if(i != 7)
            ch = ch << 1;
    }
    return ch;
}

//Magic String Decoding Fucntion
Status decode_magic_string(DecodeInfo *decInfo)
{
    int magic_len = strlen(MAGIC_STRING);
    fseek(decInfo->fptr_stego, 54, SEEK_SET);
    char magic_str[5];
    int i;
    for (i = 0; i < magic_len; i++)
    {
        //decode magic string 1 char per Iteration
        magic_str[i] = decode_lsb_to_char(decInfo);
    }
    magic_str[i] = '\0';
    if (strcmp(magic_str, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

//Lsb to Int Decoding Function
int decode_lsb_to_int(DecodeInfo *decInfo)
{
    char intbuffer[32];
    int size = 0;
    fread(intbuffer, 32, 1, decInfo->fptr_stego);
    //logic to decode lsb to int(extn size)
    for (int i = 0; i < 32; i++)
    {
        size = size | (intbuffer[i] & 1);
        if (i != 31)
            size = size << 1;
    }
    return size;
}

//Secrete File extension size Decoding Function
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decInfo->extn_size = decode_lsb_to_int(decInfo);
    return e_success;
}

//Secret file extension decoding Function
Status decode_secret_extn(DecodeInfo *decInfo)
{
    char extention[decInfo->extn_size + 1];
    for (int i = 0; i < decInfo->extn_size; i++)
    {
        extention[i] = decode_lsb_to_char(decInfo);
    }
    extention[decInfo->extn_size] = '\0';
    char *temp = strstr(decInfo->decode_fname, ".");
    if (temp != NULL)
    {
        int i = 0;
        char str[100];
        while (decInfo->decode_fname[i] != '.')
        {
            str[i] = decInfo->decode_fname[i];
            i++;
        }
        str[i] = '\0';

        //copy the extension to output file name
        strcpy(decInfo->decode_fname, str);
    }
    strcat(decInfo->decode_fname, extention);

    decInfo->fptr_decode = fopen(decInfo->decode_fname, "w");
    if (decInfo->fptr_decode == NULL)
    {
        return e_failure;
    }

    return e_success;
}

//Secret file size Decoding Function
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->size_secretfile = decode_lsb_to_int(decInfo);
    return e_success;
}

//Secret file data Decoding Function
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secretfile; i++)
    {
        ch = decode_lsb_to_char(decInfo);
        fwrite(&ch, 1, 1, decInfo->fptr_decode);
    }
    return e_success;
}

//Main Function of Do Decoding
Status do_decoding(DecodeInfo *decInfo)
{
    //Check all the Steps and Print prompt message

    printf("Opening file pocess started.\n");
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Files opened Successfully\n");
        printf("Decoding Magic string\n");
        if (decode_magic_string(decInfo) == e_success)
        {
            printf("Magic String Decoded Successfully\n");
            printf("Decoding Secret File Extension size\n");
            if (decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("Secret File extension size decoded SuccessFully\n");
                printf("Decoding secret File Extenstion\n");
                if (decode_secret_extn(decInfo) == e_success)
                {
                    printf("Secret message file :: %s\n", decInfo -> decode_fname);
                    printf("Secret File opened successfully\n");
                    printf("Decoding Secret File Size\n");
                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Secret File size Decoded Successfully\n");
                        printf("Decoding Secret Data\n");
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Secret data Decoded Successfully\n");
                            return e_success;
                        }
                        printf("ERROR: Failed to Decode secret file data\nError : In Decoding\n");
                        return e_failure;
                    }
                    printf("ERROR: Failed to Decode secret file size\nError : In Decoding\n ");
                    return e_failure;
                }
                return e_failure;
            }
            printf("ERROR: Failed to copy Decode file extention\nError : In Decoding\n");
            return e_failure;
        }
        printf("ERROR : Failed to Decode magic string\nError : In Decoding\n");
        return e_failure;
    }
    printf("ERROR: Failed to open Decoding files\nError : In Decoding\n");
    return e_failure;
}
