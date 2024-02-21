#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include<inttypes.h>

extern void saxpy(size_t n, const float a, const float *x, float *y);
// # void 
// # saxpy(size_t n, const float a, const float *x, float *y) 
// # { 
// #   size_t i; 
// #   for (i=0; i<n; i++) 
// #     y[i] = a * x[i] + y[i]; 
// # } 

int main(int argc, char const *argv[])
{
    size_t n = 11;
    float a = 2.0;
    float x[11] = {1,2,3,4,5,6,7,8,9,10,11};
    float y[11] = {1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,1.95,1.97};

    saxpy(n, a, x, y);

    for(int i = 0; i < 11 ; i++) {
        printf("%f\t", y[i]);
    }
    printf("\n");
}
