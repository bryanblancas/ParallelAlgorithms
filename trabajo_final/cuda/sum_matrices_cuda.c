#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int N = 3;

__global__
void suma(int *a, int *b, int *c, int tam){
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < tam)
		c[id] = a[id] + b[id];
}

__global__
void producto(int *a, int *b, int *c, int tam){
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	int idy = blockIdx.y * blockDim.y + threadIdx.y;
	int index = idy * tam + idx;

	if(idx < tam && idy < tam){
		c[index] = 0;
		for(int i = 0; i < tam; i++)
			c[index] += a[idy*tam+i] * b[i*tam+idx];
	}
}

void imprimir_matriz(int *a, int tam);
int n(int a, int b){
	return (int) (a/b)  + (a % b == 0? 0:1);
}

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
	imprimir_matriz(a_h, N);
	printf("MATRIZ B: ");
	imprimir_matriz(b_h, N);

	//MATRICES EN EL DEVICE
	cudaMalloc((void**) &a_d, size_mem);
	cudaMalloc((void**) &b_d, size_mem);
	cudaMalloc((void**) &c_d, size_mem);

	//MANDAR MATRICES AL DEVICE
	cudaMemcpy(a_d, a_h, size_mem, cudaMemcpyHostToDevice);
	cudaMemcpy(b_d, b_h, size_mem, cudaMemcpyHostToDevice);
	cudaMemcpy(c_d, c_h, size_mem, cudaMemcpyHostToDevice);

	/**
	**
	**	Producto de matrices
	**
	**/

	//CREAR TOPOLOGÍA
	dim3 th_size3(10,10);
	dim3 bl_size3(n(N, th_size3.x), n(N, th_size3.y));

	printf("\n TOPOLOGÍA PARA MULTIPLICACIÓN: <<< (%d,%d) , (%d, %d) >>>\n\n", bl_size3.x, bl_size3.y, th_size3.x, th_size3.y);

	producto <<< bl_size3, th_size3 >>> (a_d, b_d, c_d, N);

	//REGRESAR MATRICES
	cudaMemcpy(c_h, c_d, size_mem, cudaMemcpyDeviceToHost);

	printf("MATRIZ C: ");
	imprimir_matriz(c_h, N);

	/**
	**
	**	Suma de matrices
	**
	**/

	//CREAR TOPOLOGÍA
	th_size = 5;
	bl_size = size / th_size + (size % th_size == 0? 0:1);

	printf("\n TOPOLOGÍA PARA SUMA: <<< %d , %d >>>\n\n", bl_size, th_size);

	suma <<< bl_size, th_size >>> (a_d, b_d, c_d, size);

	//REGRESAR MATRICES
	cudaMemcpy(c_h, c_d, size_mem, cudaMemcpyDeviceToHost);

	printf("MATRIZ C: ");
	imprimir_matriz(c_h, N);
	

	free(a_h);
	free(b_h);
	free(c_h);
	cudaFree(a_d);
	cudaFree(b_d);
	cudaFree(c_d);

	return 0;
}

void imprimir_matriz(int *a, int tam){
	printf("\n");
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++)
			printf("%5d", a[i*N+j]);	
		printf("\n");
	}
}