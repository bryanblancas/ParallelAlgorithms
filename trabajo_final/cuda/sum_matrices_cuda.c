#include <stdio.h>
#include <stdlib.h>
const int N = 3;

__global__
void suma(int *a, int *b, int *c, int tam){
	printf("%d*%d+%d\n", blockIdx, blockDim , threadIdx);
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < tam)
		c[id] = a[id] + b[id];
}


void imprimir_matriz(int *a, int tam);

int main(int argc, char const *argv[]){
	
	int i, th_size, bl_size, size = N * N;
	size_t size_mem = N * N * sizeof(int);
	//matriz en HOST
	int *a_h, *b_h, *c_h;
	//matriz en DEVICE
	int *a_d, *b_d, *c_d;

	//MATRICES VECTORIZADAS DIRECTAMENTE
	a_h = (int *) malloc(sizeof(int) * size_mem);
	b_h = (int *) malloc(sizeof(int) * size_mem);
	c_h = (int *) malloc(sizeof(int) * size_mem);
	for(i = 0; i < size; i++){
		a_h[i] = i;
		b_h[i] = i * 2;
	} 

	printf("MATRIZ A: ");
	imprimir_matriz(a_h, size);
	printf("MATRIZ B: ");
	imprimir_matriz(b_h, size);

	//MATRICES EN EL DEVICE
	cudaMalloc((void**) &a_d, size_mem);
	cudaMalloc((void**) &b_d, size_mem);
	cudaMalloc((void**) &c_d, size_mem);

	//MANDAR MATRICES AL DEVICE
	cudaMemcpy(a_d, a_h, size_mem, cudaMemcpyHostToDevice);
	cudaMemcpy(b_d, b_h, size_mem, cudaMemcpyHostToDevice);
	cudaMemcpy(c_d, c_h, size_mem, cudaMemcpyHostToDevice);

	//CREAR TOPOLOGÍA
	th_size = 5;
	bl_size = size / th_size + (size % th_size == 0? 0:1);

	printf("%d , %d\n", bl_size, th_size);

	suma <<< bl_size, th_size >>> (a_d, b_d, c_d, size);

	//REGRESAR MATRICES
	cudaMemcpy(c_h, c_d, size_mem, cudaMemcpyDeviceToHost);

	printf("MATRIZ C: ");
	imprimir_matriz(c_h, size);

	free(a_h);
	free(b_h);
	free(c_h);
	cudaFree(a_d);
	cudaFree(b_d);
	cudaFree(c_d);

	return 0;
}

void imprimir_matriz(int *a, int tam){
	for(int i = 0; i < tam; i++)
		printf("%5d", a[i]);	
	printf("\n");
}