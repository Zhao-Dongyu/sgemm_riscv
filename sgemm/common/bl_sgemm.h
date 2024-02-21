/*
 * --------------------------------------------------------------------------
 * BLISLAB 
 * --------------------------------------------------------------------------
 * Copyright (C) 2016, The University of Texas at Austin
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of The University of Texas nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * bl_sgemm.h
 *
 *
 * Purpose:
 * this header file contains all function prototypes.
 *
 * Todo:
 *
 *
 * Modification:
 *
 * 
 * */


#ifndef BLISLAB_DGEMM_H
#define BLISLAB_DGEMM_H

// Allow C++ users to include this header file in their source code. However,
// we make the extern "C" conditional on whether we're using a C++ compiler,
// since regular C compilers don't understand the extern "C" construct.
#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Determine the target operating system

#if defined(__linux__)
#define BL_OS_LINUX 1
#else
#error "unsupport OS, this only support Linux"
#endif

// gettimeofday() needs this.
#include <sys/time.h>
#include <time.h>

#define GEMM_SIMD_ALIGN_SIZE 32

#define min( i, j ) ( (i)<(j) ? (i): (j) )

// #define A( i, j )     A[ (j)*lda + (i) ]
// #define B( i, j )     B[ (j)*ldb + (i) ]
// #define C( i, j )     C[ (j)*ldc + (i) ]
// #define C_ref( i, j ) C_ref[ (j)*ldc_ref + (i) ]

#define A( i, j )     A[ (i)*lda + (j) ]
#define B( i, j )     B[ (i)*ldb + (j) ]
#define C( i, j )     C[ (i)*ldc + (j) ]
#define C_ref( i, j ) C_ref[ (i)*ldc_ref + (j) ]

void bl_sgemm(
        int    m,
        int    n,
        int    k,
        float *A,
        int    lda,
        float *B,
        int    ldb,
        float *C,
        int    ldc
        );

void bl_sgemm_pack(
        int    m,
        int    mr,
        int    n,
        int    nr,
        int    k,
        float *A,
        float *packA,
        int    lda,
        float *B,
        float *packB,
        int    ldb,
        float *C,
        int    ldc
        );

float *bl_malloc_aligned(
        int    m,
        int    n,
        int    size
        );

void bl_printmatrix(
        float *A,
        int    lda,
        int    m,
        int    n
        );

float bl_clock( void );
float bl_clock_helper();

void bl_sgemm_ref(
    int    m,
    int    n,
    int    k,
    float *XA,
    int    lda,
    float *XB,
    int    ldb,
    float *XC,
    int    ldc
    );

// End extern "C" construct block.
#ifdef __cplusplus
}
#endif

#endif

