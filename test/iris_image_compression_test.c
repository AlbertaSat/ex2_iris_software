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
#include "iris_configs.h"
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
  sprintf(testIm, "testImage.png");
  printf("String: %s\n", testIm);

  return 0;
  // MetaData for image
  input_feature_t image_metadata;
  image_metadata.signed_samples = '1';
  image_metadata.signed_samples = '1';
  image_metadata.dyn_range = '1';
  image_metadata.regular_input = '1';
  image_metadata.x_size = 512;
  image_metadata.y_size = 256;
  image_metadata.z_size = 8;
  image_metadata.in_interleaving = BSQ;
  image_metadata.in_interleaving_depth = 1;
  image_metadata.byte_ordering = BIG;

  // Prepare the buffer packet
  buffer_packet_t buffer;
  buffer.image_metadata = image_metadata;
  buffer.heuristic_value = 0;
  buffer.comp_duration = 0;
  buffer.pred_duration = 0;
  buffer.encode_duration = 0;
  buffer.compressed_bytes = 0;
  buffer.compressed_rate = 0;
  strcpy(buffer.image_file, testIm);
  strcpy(buffer.compressed_file, "testOut");

  // Initialize the buffer with demo data
  buf_handle_t handle = buffer_init(&buffer, 10);

  
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
  return 0;
}