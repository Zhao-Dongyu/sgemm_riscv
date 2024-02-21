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
 * bl_sgemm.c
 *
 *
 * Purpose:
 * this is the main file of blislab sgemm.
 *
 * Todo:
 *
 *
 * Modification:
 *
 * 
 * */

#include "bl_sgemm.h"
#include "bl_config.h"

inline void PackInputLayout(float* dst, const float* src, int m, int k, int mr) {
    int    i, j, p;

    for ( j = 0; j < m / mr; j ++ ) {
        for ( i = 0; i < k; i ++ ) {
            for ( p = 0; p < mr; p ++ ) {
                *dst ++ = *(src + p * k + j * mr * k + i);
            }
        }
    }
}


void AddDot_4x4_opt( int k, float *packA, int lda, float *packB, int ldb, float *C, int ldc )
{
   register float C00, C01, C02, C03, C10, C11, C12, C13, C20, C21, C22, C23, C30, C31, C32, C33;
   float *packAp, *packBp;
   int p;

   C00 = 0.0f;
   C01 = 0.0f;
   C02 = 0.0f;
   C03 = 0.0f;
   C10 = 0.0f;
   C11 = 0.0f;
   C12 = 0.0f;
   C13 = 0.0f;
   C20 = 0.0f;
   C21 = 0.0f;
   C22 = 0.0f;
   C23 = 0.0f;
   C30 = 0.0f;
   C31 = 0.0f;
   C32 = 0.0f;
   C33 = 0.0f;
   for (p = 0; p < k; p++) {
     packAp = &packA[p * 4];
     packBp = &packB[p * 4];

     C00 += packAp[0] * packBp[0];
     C01 += packAp[0] * packBp[1];
     C02 += packAp[0] * packBp[2];
     C03 += packAp[0] * packBp[3];
     C10 += packAp[1] * packBp[0];
     C11 += packAp[1] * packBp[1];
     C12 += packAp[1] * packBp[2];
     C13 += packAp[1] * packBp[3];
     C20 += packAp[2] * packBp[0];
     C21 += packAp[2] * packBp[1];
     C22 += packAp[2] * packBp[2];
     C23 += packAp[2] * packBp[3];
     C30 += packAp[3] * packBp[0];
     C31 += packAp[3] * packBp[1];
     C32 += packAp[3] * packBp[2];
     C33 += packAp[3] * packBp[3];

   }
   C(0, 0) += C00;
   C(0, 1) += C01;
   C(0, 2) += C02;
   C(0, 3) += C03;
   C(1, 0) += C10;
   C(1, 1) += C11;
   C(1, 2) += C12;
   C(1, 3) += C13;
   C(2, 0) += C20;
   C(2, 1) += C21;
   C(2, 2) += C22;
   C(2, 3) += C23;
   C(3, 0) += C30;
   C(3, 1) += C31;
   C(3, 2) += C32;
   C(3, 3) += C33;
}


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
    float *C,        // must be aligned
    int    ldc        // ldc must also be aligned
)
{
    int i, j, p;
    int ir, jr;

    // Early return if possible
    if ( m == 0 || n == 0 || k == 0 ) {
        printf( "bl_sgemm(): early return\n" );
        return;
    }

    PackInputLayout(packA, A, m, k, mr);

    // printf("A\n");
    // for(int i = 0; i < m; i++) {
    //   for(int j = 0; j < m; j++) {
    //     printf("%.0f\t", A[i * m + j]);
    //   }
    //   printf("\n");
    // }
    // printf("\n\n");

    // printf("packA\n");
    // for(int i = 0; i < m; i++) {
    //   for(int j = 0; j < m; j++) {
    //     printf("%.0f\t", packA[i * m + j]);
    //   }
    //   printf("\n");
    // }
    // printf("\n\n");

    for ( i = 0; i < m; i += DGEMM_MR ) {          // Start 2-nd loop
      for ( j = 0; j < n; j += DGEMM_NR ) {        // Start 1-st loop
        //    AddDot_4x4_opt( k, &A( i, 0 ), lda, &packB[j * k], ldb, &C( i, j ), ldc );
           AddDot_4x4_opt( k, &packA[i * k], lda, &packB[j * k], ldb, &C( i, j ), ldc );
        }                                          // End   1-st loop
    }                                              // End   2-nd loop
}


