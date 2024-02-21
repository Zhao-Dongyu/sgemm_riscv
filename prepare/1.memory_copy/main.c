#include <stdio.h>

extern void *memcpy(void* dest, const void* src, size_t n);


int main(int argc, char const *argv[])
{
    int array_a[10] = {0,1,2,3,4,5,6,7,8,9};
    int array_b[10] = {};
    printf("Hello NeZha\n");
    memcpy(array_b, array_a, 10 * sizeof(int));

    for(int i = 0; i < 10; i ++) {
        printf("%d\t", array_b[i]);
    }
    printf("\n");
    return 0;
}