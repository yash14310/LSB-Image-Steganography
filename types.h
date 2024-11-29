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

#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
