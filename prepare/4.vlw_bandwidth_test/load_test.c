#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

extern void load(size_t mc,         // nr <= 16, a0
                 size_t nc,         // mr <= 4,  a1
                 const float* a,    // mr * k,   a3
                 float* c          // mr * nr,  a5
                );


int main(int argc, char const *argv[])
{
    int warmup_times = 10;
    int test_times = 1000;
    int nc = 1024;
    int mc = 1024;

    float* a = (float*)malloc(mc * nc * sizeof(float));
    float* c = (float*)malloc(mc * nc * sizeof(float));

    for(int i = 0; i < nc * mc; i++) a[i] = i;


    float time_use=0;
    struct timeval start;
    struct timeval end;

    //warmup
    for (int i = 0; i < warmup_times; i++){
        load(mc, nc, a, c);
    }


    gettimeofday(&start,NULL);
    for (int i = 0; i < test_times; i++){
        load(mc, nc, a, c);
    }
    gettimeofday(&end,NULL);
    time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
    printf("time_use is %.3fms\n", time_use/1000);


    free(a);
    free(c);
}
