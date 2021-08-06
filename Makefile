#****************************************************************************
#
# Copyright 2013-2020 Altera Corporation. All Rights Reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
#****************************************************************************

#
# $Id$
#

COMPILER := GNU
#COMPILER := ARMCC

MEMORY ?= ddr
#MEMORY ?= ocr

SEMIHOSTED ?= 1
#SEMIHOSTED ?= 0

HWLIBS_ROOT := ../../../armv7a/hwlib
NEWLIB_ROOT := ../../../tools/newlib/usr/local/arm-eabi

ADD_CFLAGS_GNU :=
ADD_CFLAGS_ARMCC :=

EXAMPLE_SRC := source/main.c source/init_seq.c source/iris_api.c source/iris_buffer.c source/iris_cloud_detection.c \
	source/iris_compression.c source/iris_entropy_encoder.c source/iris_heuristic.c source/iris_predictor.c source/iris_utils.c

ELF := application.axf

# These parameters can be overriden
# LINKER_SCRIPT
HWLIBS_SRC := alt_clock_manager.c alt_globaltmr.c alt_interrupt.c alt_timers.c alt_watchdog.c alt_spi.c alt_generalpurpose_io.c alt_16550_uart.c

include Makefile.inc


out/compression.o: source/iris_compression.c 
	gcc -c $^ -o $@ -I include

out/compressionTest.o: test/iris_image_compression_test.c
	gcc -c test/iris_image_compression_test.c -o out/compTest.o -I include

out/seq.o: source/init_seq.c
	gcc -c $^ -o $@ -I include

out/api.o: source/iris_api.c include/iris_api.h
	gcc -c $^ -o $@ -I include

out/buffer.o: source/iris_buffer.c include iris_buffer.h

missc.o: 
	cd source && gcc -c init_seq.c iris_buffer.c iris_compression.c iris_heuristic.c  iris_utils.c iris_api.c iris_cloud_detection.c iris_entropy_encoder.c iris_predictor.c -I include



clean:
	rm -f out/*.o
