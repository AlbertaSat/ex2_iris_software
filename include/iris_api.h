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
 * @file iris_api.h
 * @author Scott Chu
 * @date 2020-07-30
 */

#ifndef IRIS_API_H
#define IRIS_API_H

#include "iris_buffer.h"

int heuristic_img(buf_handle_t buffer, int imgnum);

int compress_img(buf_handle_t buffer, int imgnum);

int send_img(buf_handle_t buffer, int imgnum);

int capture_img();

#endif
