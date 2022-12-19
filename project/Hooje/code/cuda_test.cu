// cuda_test.cu
#include<cuda_runtime.h>
#include<stdio.h>

__global__ void kernel(){
    printf("inside in kernel\n");
}

int my_cuda(int a, int b){

   kernel<<<1,10>>>();
   cudaDeviceSynchronize();

   return 0;
}