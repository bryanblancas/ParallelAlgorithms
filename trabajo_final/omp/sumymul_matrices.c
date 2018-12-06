#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Verificación de que se haya compilado con openmp
#ifdef _OPENMP 
	#include <omp.h>
#else 
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
#endif
#define azul "\x1B[36m"
#define cerrar "\x1B[00m"

void crear_matriz(int ***m, int tam);
void llenar_matriz(int **m, int tam);
void imprimir_matriz(int **m, int tam);
void suma_matrices(int **a, int **b, int **c, int tam);
void mul_matrices(int **a, int **b, int **c, int tam);

int main(int argc, char const *argv[]){
	srand(time(NULL));

	//validación de argumentos de ejecución
	if(argc != 3){
		printf("./sumymul_matrices.c num_hilos tam_matriz\n");
		return 1;
	}

	//declaración y creación de matrices
	int **a, **b, **c, tam = atoi(argv[2]);
	double t;

	crear_matriz(&a, tam);
	llenar_matriz(a, tam);
	crear_matriz(&b, tam);
	llenar_matriz(b, tam);
	crear_matriz(&c, tam);

	printf("Matriz A: \n");
	//imprimir_matriz(a, tam);
	printf("\nMatriz B: \n");
	//imprimir_matriz(b, tam);
	printf("\n");

	//asignación de número de hilos
	int n_th = atoi(argv[1]);
	omp_set_num_threads(n_th);

	//suma de matrices
	t -= omp_get_wtime();
	suma_matrices(a, b, c, tam);
	t += omp_get_wtime();
	printf(azul"Suma A+B time: %lf\n"cerrar, t);
	//imprimir_matriz(c, tam);

	//multiplicación de matrices
	t-= omp_get_wtime();
	mul_matrices(a, b, c, tam);
	t+= omp_get_wtime();
	printf(azul"\nProducto A+B time: %lf\n"cerrar, t);
	//imprimir_matriz(c, tam);

	free(a);
	free(b);
	free(c);
	return 0;
}

void crear_matriz(int ***m, int tam){
	*m = (int **) malloc(sizeof(int*) * tam);
	for(int i = 0; i < tam; i++)
		(*m)[i] = (int *) malloc(sizeof(int) * tam);
}
void llenar_matriz(int **m, int tam){
	for (int i = 0; i < tam; ++i)
		for (int j = 0; j < tam; ++j)
			m[i][j] = rand() % 10;	
}
void imprimir_matriz(int **m, int tam){
	for (int i = 0; i < tam; ++i){
		for (int j = 0; j < tam; ++j)
			printf("%6d", m[i][j]);
		printf("\n");
	}
}

//Funciones de operaciones de matrices
void suma_matrices(int **a, int **b, int **c, int tam){
	int i, j;
	//se paraleliza el bucle exterior
	#pragma omp parallel for private(j)
	for (i = 0; i < tam; ++i)
		for (j = 0; j < tam; ++j)
			c[i][j] = a[i][j] + b[i][j]; 
}

void mul_matrices(int **a, int **b, int **c, int tam){
	int i, j, k;
	//se paraleliza el bucle exterior
	#pragma omp parallel for private(j,k)
	for(i = 0; i < tam; i++)
		for(j = 0; j < tam; j++){
			c[i][j] = 0;
			for(k = 0; k < tam; k++)
				c[i][j] += a[i][k] * b[k][j];
		}
}