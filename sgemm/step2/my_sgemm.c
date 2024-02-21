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

void AddDot( int k, float *A, int lda, float *B, int ldb, float *result ) {
  int p;
  for ( p = 0; p < k; p++ ) {
    *result += A( 0, p ) * B( p, 0 );
  }
}

void AddDot_MRxNR( int k, float *A, int lda, float *B, int ldb, float *C, int ldc )
{
  int ir, jr;
  int p;
  for ( jr = 0; jr < DGEMM_NR; jr++ ) {
    for ( ir = 0; ir < DGEMM_MR; ir++ ) {
      AddDot( k, &A( ir, 0 ), lda, &B( 0, jr ), ldb, &C( ir, jr ) );
    }
  }
}


void bl_sgemm(
    int    m,
    int    n,
    int    k,
    float *A,
    int    lda,
    float *B,
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

    for ( i = 0; i < m; i += DGEMM_MR ) {          // Start 2-nd loop
      for ( j = 0; j < n; j += DGEMM_NR ) {        // Start 1-st loop
           AddDot_MRxNR( k, &A( i, 0 ), lda, &B( 0, j ), ldb, &C( i, j ), ldc );
        }                                          // End   1-st loop
    }                                              // End   2-nd loop
}


