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

#ifndef COMMON_H
#define COMMON_H

/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*"

#endif
