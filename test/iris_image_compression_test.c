/*
 *Copyright 2020 University of Alberta
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
 * @file iris_image_compression_test.c
 * @author Meharpreet Singh Nanda
 * @date 2021-08-05
 */

/*Standard Lib Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*File Includes*/
#include "iris_buffer.h"

/*
  - Assign the image file
  - Prepare the buffer for the compresser
  - Pass the compresser
  - Read the compressed file
  
  -- Decompress the image and compare with original
*/

/**
 * @brief
 * 		Tests the image compression script 
 * @param 
 * 	
 * @return
 * 		Returns 1 if not sucessful.
 */
int test_compress_img()
{
  char* testIm = (char *) malloc(128);
  assign_char(testIm, 128);
  printf("String: %s\n", testIm);
  // // MetaData
  // char signed_samples = '1';
  // unsigned char dyn_range = '1';
  // unsigned char regular_input = '1';
  // unsigned int x_size = 64;
  // unsigned int y_size = 64;
  // unsigned int z_size = 1;
  // interleaving_t in_interleaving = BSQ;
  // unsigned int in_interleaving_depth = 1;
  // endianness_t byte_ordering = BIG;

  // // Prepare the buffer packet
  // input_feature_t image_metadata = ;
  // double heuristic_value;
  // double comp_duration;
  // double pred_duration;
  // double encode_duration;
  // double compressed_bytes;
  // double compressed_rate;
  // char image_file[128];
  // char compressed_file[128];

  // Free Memory
  free(testIm);
}

void assign_char(char* str, int size)
{
  char temp = 'a';
  for (int i = 0; i < size; i++)
  {
    str[i] = temp;
    if (temp == 'z')
      temp = 'a';
    temp = temp + 1;
  }
}

int main(){
  test_compress_img();
}