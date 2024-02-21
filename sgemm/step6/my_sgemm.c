#include "bl_sgemm.h"
#include "bl_config.h"

extern void RvvSgemm4x16( size_t nr,         // nr <= 16
                          size_t mr,         // mr <= 4
                          size_t k,          // astride = k*sizeof(float)
                          const float* a,    // mr * k
                          const float* b,    // k * 16
                          float* c,          // mr * nr
                          size_t cn_stride,  // Len(N) * sizeof(float)
                          const float* bias  // bias 
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
    float *C,           // must be aligned
    int    ldc          // ldc must also be aligned
)
{
    int i, j, p;
    int ir, jr;

    // Early return if possible
    if ( m == 0 || n == 0 || k == 0 ) {
        printf( "bl_sgemm(): early return\n" );
        return;
    }

    float bias[800];
    for(int i = 0; i < 800; i++) {bias[i] = 0;}

    for ( i = 0; i < m; i += DGEMM_MR ) {       // Start 2-nd loop
        int mb = DGEMM_MR;
        if((m - i) < DGEMM_MR) mb = m - i; 

        for ( j = 0; j < n; j += DGEMM_NR ) {   // Start 1-st loop
            int nb = DGEMM_NR;
            if((n - j) < DGEMM_NR) nb = n - j; 

            RvvSgemm4x16(   nb,                 // nr <= 16, a0
                            mb,                 // mr <= 4,  a1
                            k,                  // astride = k*sizeof(float), a2
                            &A[i * k],          // mr * k,   a3
                            &packB[j * k],      // k * 16,   a4
                            &C( i, j ),         // mr * nr,  a5
                            n * sizeof(float),  // Len(N) * sizeof(float), a6
                            bias
                            );
        }                                       // End   1-st loop
    }                                           // End   2-nd loop
}


