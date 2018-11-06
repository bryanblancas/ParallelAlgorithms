#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

//el tamaño de la matriz es del número de procesos

int ** crear_y_llenar_matriz(int tam);
void imprimir_matriz(int **matriz, int tam);

int main(int argc, char const *argv[]){
	int size, rank, th_id, th_size;
	int **ma, **mb, *vec, i, j, k, *columna, *columna_resul;

	MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		columna = (int *) malloc(sizeof(int) * size);
		columna_resul = (int *) malloc(sizeof(int) * size);
		ma = crear_y_llenar_matriz(size);

		//SÓLO EL PROCESO 0 VA A CREAR LA MATRIZ B, AQUELLA QUE SERÁ REPARTIDA ENTRE TODOS LOS PROCESOS
		if(rank == 0){
			for(i=0; i<1000000000; i++);
			mb = crear_y_llenar_matriz(size);
			printf("Soy el proceso %d y la matriz B es: \n", rank);
			imprimir_matriz(mb, size);
			printf("Soy el proceso %d y la matriz A es: \n", rank);
			imprimir_matriz(ma, size);

			//VECTORIZACIÓN DE MATRIZ B
			vec = (int *) malloc(sizeof(int) * size * size);
			for(i=0; i<size; i++)
				for(j=0; j<size; j++)
					vec[i*size+j] = mb[j][i];
		}

		//REPARTIR MATRICES COLUMNAS ENTRE TODOS LOS PROCESOS
		MPI_Scatter(vec, size, MPI_INT, columna, size, MPI_INT, 0, MPI_COMM_WORLD);

		//LAS OPERACINES PARA CALCULAR EL PRODUCTO ma * columna 

		omp_set_num_threads(size);
		#pragma omp parallel
		{	
			th_id = omp_get_thread_num();
			th_size = omp_get_num_threads();
			#pragma omp for private(j)
				for(i=0; i<size; i++){
					columna_resul[i] = 0;
					for(j=0; j<size; j++)
						columna_resul[i] += ma[i][j] * columna[j];
					//printf("Proceso %d de %d Hilo %d de %d calculé: %d\n",rank, size, th_id, th_size, columna_resul[i]);
				}
		}

		//RETORNAR LAS MATRICES COLUMNA CALCULADAS AL PROCESO 0
		MPI_Gather(columna_resul, size, MPI_INT, vec, size, MPI_INT, 0, MPI_COMM_WORLD);

		if(rank == 0){
			printf("RESULTADO DE A*B: \n");
			for(i=0; i<size; i++)
				for(j=0; j<size; j++)
					ma[j][i] = vec[i*size+j];
			imprimir_matriz(ma, size);
		}

	MPI_Finalize();

	return 0;
}

int ** crear_y_llenar_matriz(int n){
	int **matriz;
	matriz = (int **) malloc(sizeof(int *) * n);
	for(int i=0 ; i<n; i++)
		matriz[i] = (int *) malloc(sizeof(int) * n);

	srand(time(NULL));
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			matriz[i][j] = rand()%2;

	return matriz;
}

void imprimir_matriz(int **matriz, int tam){
	for (int i = 0; i < tam; ++i){
		for (int j = 0; j < tam; ++j)
			printf("%5d", matriz[i][j]);
		printf("\n");
	}
	printf("\n");
}