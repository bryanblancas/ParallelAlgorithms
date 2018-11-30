#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define azul "\x1B[36m"
#define cerrar "\x1B[00m"

//el tamaño de la matriz es del número de procesos

void crear_matriz(int ***m, int tam);
void llenar_matriz(int **m, int tam);
void imprimir_matriz(int **m, int tam);

int main(int argc, char const *argv[]){
	srand(time(NULL));

	//VARIABLES DE MPI
	int size, rank;
	//VARIABLES DE LAS MATRICES Y EL MANEJO DE GATHER Y SCATTER
	int **ma, **mb, *vec, i, j, *columna, *columna_resul;

	MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		//CREACIÓN DE LAS MATRICES PARA TODOS LOS PROCESOS
		columna = (int *) malloc(sizeof(int) * size);
		columna_resul = (int *) malloc(sizeof(int) * size);
		crear_matriz(&ma, size);
		llenar_matriz(ma, size);

		//SÓLO EL PROCESO 0 VA A CREAR LA MATRIZ B, AQUELLA QUE SERÁ REPARTIDA ENTRE TODOS LOS PROCESOS
		if(rank == 0){
			//CREACIÓN DE LA MATRIZ B
			crear_matriz(&mb, size);
			llenar_matriz(mb, size);

			printf("\nSoy el proceso %d y la matriz A es: \n", rank);
			imprimir_matriz(ma, size);
			printf("\nSoy el proceso %d y la matriz B es: \n", rank);
			imprimir_matriz(mb, size);

			//VECTORIZACIÓN DE MATRIZ B
			vec = (int *) malloc(sizeof(int) * size * size);
			for(i=0; i<size; i++)
				for(j=0; j<size; j++)
					vec[i*size+j] = mb[j][i];
		}

		//REPARTIR MATRICES COLUMNAS ENTRE TODOS LOS PROCESOS
		MPI_Scatter(vec, size, MPI_INT, columna, size, MPI_INT, 0, MPI_COMM_WORLD);

		//LAS OPERACINES PARA CALCULAR EL PRODUCTO ma * columna 
		for(i=0; i < size; i++){
			columna_resul[i] = 0;
			for(j=0; j < size; j++)
				columna_resul[i] += ma[i][j] * columna[j];
		}

		//RETORNAR LAS MATRICES COLUMNA CALCULADAS AL PROCESO 0
		MPI_Gather(columna_resul, size, MPI_INT, vec, size, MPI_INT, 0, MPI_COMM_WORLD);

		if(rank == 0){
			printf(azul"\nRESULTADO DE A*B: \n"cerrar);
			for(i=0; i<size; i++)
				for(j=0; j<size; j++)
					ma[j][i] = vec[i*size+j];
			imprimir_matriz(ma, size);
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