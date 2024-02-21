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
    float *C,         // must be aligned
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

    // printf("[A]\n");
    // for(int i = 0; i < m; i++) {
    //   for(int j = 0; j < k; j++) {
    //     printf("%.0f\t", A[i * k + j]);
    //   }
    //   printf("\n");
    // }
    // printf("[packA]\n");
    // for(int i = 0; i < m; i++) {
    //   for(int j = 0; j < k; j++) {
    //     printf("%.0f\t", packA[i * k + j]);
    //   }
    //   printf("\n");
    // }

    float bias[800] = {0};
    for(int i = 0; i < 800; i++) {bias[i] = 0;}

    for ( j = 0; j < n; j += DGEMM_NR ) {       // Start 2-st loop
        int nb = DGEMM_NR;
        if((n - j) < DGEMM_NR) nb = n - j; 

        for ( i = 0; i < m; i += DGEMM_MR ) {   // Start 1-nd loop
            int mb = DGEMM_MR;
            if((m - i) < DGEMM_MR) mb = m - i; 

            RvvSgemm4x16(   nb,                 // nr <= 16, a0
                            mb,                 // mr <= 4,  a1
                            k,                  // astride = k*sizeof(float), a2
                            &packA[i * k],      // mr * k,   a3
                            &packB[j * k],      // k * 16,   a4
                            &C( i, j ),         // mr * nr,  a5
                            n * sizeof(float),  // Len(N) * sizeof(float), a6
                            bias
                            );
        }                                       // End   1-st loop
    }                                           // End   2-nd loop
}
