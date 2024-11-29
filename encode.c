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
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

OperationType check_operation_type(char *argv[])
{
    //Return the Specific Operation based on the input argv[1]
    return strcmp(argv[1], "-e") == 0 ? e_encode : strcmp(argv[1], "-d") == 0 ? e_decode : e_unsupported ;
}

//Read and validate Function
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    printf("Checking Arguments is stated\n");
    
    //check if the source file is .bmp
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("Error : Difficulty in Checking source File extension in .bmp\n");
        return e_failure;
    }

    char *temp = strstr(argv[3], ".");
    strcpy(encInfo->extn_secret_file, temp);
    //Check the Secret FIle extension( .txt/ .sh/ .c)
    if (strcmp(temp, ".txt") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else if (strcmp(temp, ".sh") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else if (strcmp(temp, ".c") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        return e_failure;
    }
    if (argv[4] != NULL)
    {
        //check if output file is .bmp or not
        if ((strcmp(strstr(argv[4], "."), ".bmp")) == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
            return e_failure;
    }
    else
        //if output file is not pass in arguments create one with "stego.bmp" name
        encInfo->stego_image_fname = "stego.bmp";

    return e_success;

}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

//Get Image size of bmp Function
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

//Check capacity Function
Status check_capacity(EncodeInfo *encInfo)
{
    printf("Checking capacity to encode message\n");
    int len = strlen(MAGIC_STRING);

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    uint size_secret_file = get_file_size(encInfo->fptr_secret);
    encInfo->size_secret_file = size_secret_file;

    //make total of (magic string length, extension size, extension file size + secret file extenion + secret file data * 8)
    uint encode_size = 54 + (len + 4 + 4 + strlen(encInfo->extn_secret_file) + size_secret_file) * 8;

    //Check if Image capacity if Greater than Total of mmy allocated
    if (encInfo->image_capacity > encode_size)
    {
        printf("Capacity of source file is more than the secrete file.\n");
        return e_success;
    }
    else
    {        
        return e_failure;
    }
}

//Gwwt file size function
uint get_file_size(FILE *fptr)
{
    //Seek File ptr to Last char of file
    fseek(fptr, 0, SEEK_END);
    
    //Store the Total bytes 
    int file_size = ftell(fptr);

    //Seek file pointer to 0th byte
    fseek(fptr, 0, SEEK_SET);

    return file_size;
}

//Copy Bmp header function
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    fread(buffer, 54, 1, fptr_src_image);

    fwrite(buffer, 54, 1, fptr_dest_image);

    
    return e_success;
}

//Encode magic String Function
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int magicstr_len = strlen(MAGIC_STRING);
    //call encode data to image function
    encode_data_to_image(magic_string, magicstr_len, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    
    return e_success;
}

//Encode data to image function
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{

    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

//Encode byte to lsb Function
Status encode_byte_to_lsb(char data, char image_buffer[])
{
    for (int i = 7; i >= 0; i--)
    {
        int get = (data & 1 << i) >> i;  //get bit
        image_buffer[7 - i] = image_buffer[7 - i] & (~1);  //clear bits
        image_buffer[7 - i] = image_buffer[7 - i] | get;   //Set bits
        
    }
    return e_success;
}

//Encode FIle Extension size function
Status encode_file_extn_size(char *file_extn, EncodeInfo *encInfo)
{
    char image_buffer[32];
    
    //read 32 bytes from src file(.bmp)
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    long size = strlen(file_extn);

    //call encode int to lsb function
    encode_int_to_lsb(size, image_buffer);

    //Write 32 bytes to Output file(.bmp)
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

//Encode int to lsb function
Status encode_int_to_lsb(long size, char image_buffer[])
{
    //logic 
    for (int i = 31; i >= 0; i--)
    {
        int get = (size & 1 << i) >> i; //get bits
        image_buffer[31 - i] = image_buffer[31 - i] & (~1);  //clear bits
        image_buffer[31 - i] = image_buffer[31 - i] | get;   //Set bits

    }
}

//Encode secret file extn Function
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

//Encode Secret file size Function
Status encode_secret_file_size(uint size_secret_file, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    encode_int_to_lsb(size_secret_file, image_buffer);
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

//Encode Secret file Data function
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char sec_file[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);

    //Read the Secret File data from 
    fread(sec_file, encInfo->size_secret_file, 1, encInfo->fptr_secret);

    if (encode_data_to_image(sec_file, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    return e_failure;
}

//Copy the Remaining Image Data
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src_image) != 0)
    {
        fwrite(&ch, 1, 1, fptr_stego_image);
    }
    return e_success;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("Open file process started.\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }
    else
    {
        printf("Opened Source.bmp file\n");
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }
    else
     {
        printf("Opened Secret.txt\n");
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }
    else
    {
        printf("Opened Output.bmp\n");
    }

    // No failure return e_success
    printf("Files Opened Successfully\n");
    return e_success;
}


//Main Do encoding Function
Status do_encoding(EncodeInfo *encInfo)
{
    //Check all the steps and print the prompt message based on the output

    if (check_capacity(encInfo) == e_success)
    {
        
        printf("Header is copying\n");
        if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        {
            printf("Successfully Header is copied\n");
            printf("Encoding Magic String\n");
            if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
            {
                printf("Magic String Encoded Successfully\n");
                printf("Encoding the File Extention size.\n");
                printf("Entension = %s\n",encInfo->extn_secret_file);
                if (encode_file_extn_size(encInfo->extn_secret_file, encInfo) == e_success)
                {
                    printf("File Extension Size Encoded SuccessFully\n");
                    printf("Encoding Secret file extention\n");
                    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                    {
                        printf("File Extension Encoded Successfully (Ex : .txt)\n");
                        printf("Encoding Secret File Size (Data size).\n");
                        if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                        {
                            printf("File Size is Encoded Successfully\n");
                            printf("Encoding the Secret Data..\n");
                            if (encode_secret_file_data(encInfo) == e_success)
                            {
                                printf("Secret Data Encoded Successfully.\n");
                                printf("Copying the Remaining Data.\n");
                                if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                {
                                    return e_success;
                                }
                            }
                            else
                            {
                                printf("ERROR: Failed to Encode Secret File data\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Failed to Encode Secret file size\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Failed to Encode Secret file extention\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Failed to Encode File extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Failed to Encode Magic String\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Failed to Copy the .bmp Header\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Not Sufficient Capacity\n");
        return e_failure;
    }
}
