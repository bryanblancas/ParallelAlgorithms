#include <stdio.h>

const int N = 16;
const int blocksize = 16;

__global__
void hello(char *a, int *b){
	a[threadIdx.x] += b[threadIdx.x];
}

int main(int argc, char const *argv[]){
	
	char a[N] = "hello \0\0\0\0\0";
	int b[N] = {15,10,6,0,-11,1,0,0,0,0,0,0,0,0,0,0};
	
	char *ad;	//la d se usa para decir que es una variable que será usada dentro del dispositivo
	int *bd;

	const int csize = N*sizeof(char);
	const int isize = N*sizeof(int);

	printf("%s\n", a);

	cudaMalloc((void **) &ad, csize);
	cudaMalloc((void **) &bd, isize);
	cudaMemcpy(ad, a, csize, cudaMemcpyHostToDevice);  //donde , de donde, tamaño, ram a gpu
	cudaMemcpy(bd, b, isize, cudaMemcpyHostToDevice);

	dim3 dimBlock(blocksize, 1);	//vector de 3 datos
	dim3 dimGrid(1, 1);
	hello<<<dimGrid, dimBlock>>>(ad, bd);	//llamada a un función de cuda, se le manda la arquitectura <<< >>>
	cudaMemcpy(a, ad, csize, cudaMemcpyDeviceToHost);
	cudaFree(ad);
	cudaFree(bd);

	printf("%s\n", a);
	//sleep(1);
	return EXIT_SUCCESS;
}