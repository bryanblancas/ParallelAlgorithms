#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#define azul "\x1B[36m"
#define cerrar "\x1B[00m"

//el tamaño de la matriz es del número de procesos

void crear_matriz(int ***m, int tam);
void llenar_matriz(int **m, int tam);
void imprimir_matriz(int **m, int tam);

int main(int argc, char const *argv[]){
	srand(time(NULL));
	
	if(argc != 3){
		printf("./sum_matrices_mpi.c tam_matriz n_th_omp\n");
		return 1;
	}

	//variables de MPI
	int size, rank;
	
	//Variables para las matrices y el manejo del gather and scatter
	int **ma, **mb, *vec, *columna, *columna_resul;
	int i, j, tam = atoi(argv[1]), n_per_proc, tam_per_proc, n_th = atoi(argv[2]);

	MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if(tam % size != 0){
			printf("Por favor que el tam de la matriz sea divisible entre el número de procesos\n");
			return 1;
		}

		n_per_proc = tam/size;
		tam_per_proc = tam*n_per_proc;

		//Creado de matrices columna y de matriz a, que todos los procesos ocuparan
		columna = (int *) malloc(sizeof(int) * tam_per_proc);
		columna_resul = (int *) malloc(sizeof(int) * tam_per_proc);
		crear_matriz(&ma, tam);
		llenar_matriz(ma, tam);

		//SÓLO EL PROCESO 0 VA A CREAR LA MATRIZ B, AQUELLA QUE SERÁ REPARTIDA ENTRE TODOS LOS PROCESOS
		if(rank == 0){
			//Creación de matriz b
			crear_matriz(&mb, tam);
			llenar_matriz(mb, tam);

			printf("\nSoy el proceso %d y la matriz A es: \n", rank);
			//imprimir_matriz(ma, tam);
			printf("\nSoy el proceso %d y la matriz B es: \n", rank);
			//imprimir_matriz(mb, tam);

			//VECTORIZACIÓN DE MATRIZ B
			vec = (int *) malloc(sizeof(int) * tam * tam);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					vec[i*tam+j] = mb[j][i];
		}

		//REPARTIR MATRICES COLUMNAS ENTRE TODOS LOS PROCESOS
		MPI_Scatter(vec, tam_per_proc, MPI_INT, columna, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		//LAS OPERACINES PARA CALCULAR LA SUMA ma + columna
		
		int n_colm[n_per_proc];
		for(j = 0; j < n_per_proc; j++)
			n_colm[j] = rank * n_per_proc + j;

		omp_set_num_threads(n_th);

		#pragma omp parallel for private(i)
		for(j = 0; j < n_per_proc; j++){
			for (i = 0; i < tam; ++i)
				columna_resul[tam*j+i] = ma[i][n_colm[j]] + columna[tam*j+i];
		}

		
		//RETORNAR LAS MATRICES COLUMNA CALCULADAS AL PROCESO 0
		MPI_Gather(columna_resul, tam_per_proc, MPI_INT, vec, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		if(rank == 0){
			printf(azul"\nRESULTADO DE A+B: \n"cerrar);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					ma[j][i] = vec[i*tam+j];
			//imprimir_matriz(ma, tam);
		}

	MPI_Finalize();

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