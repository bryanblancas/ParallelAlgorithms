#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define azul "\x1B[36m"
#define cerrar "\x1B[00m"

//el tamaño de la matriz debe de ser divisible entre el número de procesos

void crear_matriz(int ***m, int tam);
void llenar_matriz(int **m, int tam);
void imprimir_matriz(int **m, int tam);
void producto_matrices(int **ma, int *columna_resul, int tam, int n_per_proc, int *columna);
void suma_matrices(int **ma, int *columna_resul, int tam, int tam_per_proc, int *columna, int *n_colm);

int main(int argc, char const *argv[]){
	srand(time(NULL));
	
	if(argc != 2){
		printf("./sum_matrices_mpi.c tam_matriz\n");
		return 1;
	}

	//variables de MPI
	int size, rank;
	
	//Variables para las matrices y el manejo del gather and scatter
	int **ma, **mb, **mc, *vec, *vec2, *columna, *columna_resul;
	int i, j, tam = atoi(argv[1]), n_per_proc, tam_per_proc;
	double t;

	MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if(tam % size != 0){
			printf("Por favor que el tam de la matriz sea divisible entre el número de procesos\n");
			return 1;
		}

		n_per_proc = tam/size;
		tam_per_proc = tam*n_per_proc;

		/**
		**
		**
		**	Creado de matrices columna y de matriz a, que todos los procesos ocuparan
		**
		**
		**/

		columna = (int *) malloc(sizeof(int) * tam_per_proc);
		columna_resul = (int *) malloc(sizeof(int) * tam_per_proc);
		crear_matriz(&ma, tam);
		llenar_matriz(ma, tam);

		/**
		**
		**
		**SÓLO EL PROCESO 0 VA A CREAR LA MATRIZ B, AQUELLA QUE SERÁ REPARTIDA ENTRE TODOS LOS PROCESOS
		**
		**
		**/

		if(rank == 0){

			//Creación de matriz b
			crear_matriz(&mb, tam);
			llenar_matriz(mb, tam);
			//Creación de matriz c
			crear_matriz(&mc, tam);
			llenar_matriz(mc, tam);

			printf("\nSoy el proceso %d y la matriz A es: \n", rank);
			//imprimir_matriz(ma, tam);
			printf("\nSoy el proceso %d y la matriz B es: \n", rank);
			//imprimir_matriz(mb, tam);

			//VECTORIZACIÓN DE MATRIZ B
			vec = (int *) malloc(sizeof(int) * tam * tam);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					vec[i*tam+j] = mb[j][i];
			vec2 = (int *) malloc(sizeof(int) * tam * tam);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					vec2[i*tam+j] = mb[j][i];
		}

		/**
		**
		**
		**	Multiplicación de matrices
		**
		**
		**/

		//Repartir las matrices columna entre todos los procesos
		MPI_Scatter(vec, tam_per_proc, MPI_INT, columna, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);


		t -= MPI_Wtime();
		producto_matrices(ma,columna_resul,tam, n_per_proc, columna);
		t += MPI_Wtime();


		//Retornar las matrices columna calculadas al proceso 0
		MPI_Gather(columna_resul, tam_per_proc, MPI_INT, vec, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		if(rank == 0){
			printf(azul"\nRESULTADO DE A*B: \n"cerrar);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					mc[j][i] = vec[i*tam+j];
			//imprimir_matriz(mc, tam);
		}

		/**
		**
		**
		**	Suma de matrices
		**
		**
		**/

		//Repartir las matrices columna entre todos los procesos
		MPI_Scatter(vec2, tam_per_proc, MPI_INT, columna, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		//Operaciones para calcular la suma ma + columna
		int n_colm[n_per_proc];
		for(j = 0; j < n_per_proc; j++)
			n_colm[j] = rank * n_per_proc + j;



		suma_matrices(ma, columna_resul, tam, tam_per_proc, columna, n_colm);



		//Retornar las matrices columa calculadas al proceso 0
		MPI_Gather(columna_resul, tam_per_proc, MPI_INT, vec2, tam_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

		if(rank == 0){
			printf(azul"\nRESULTADO DE A+B: \n"cerrar);
			for(i=0; i<tam; i++)
				for(j=0; j<tam; j++)
					mc[j][i] = vec2[i*tam+j];
			//imprimir_matriz(mc, tam);
		}

	MPI_Finalize();

	return 0;
}

void suma_matrices(int **ma, int *columna_resul, int tam, int tam_per_proc, int *columna, int *n_colm){
	int k = 0, j = 0, i;
	for(i = 0; i < tam_per_proc; i++){
		columna_resul[i] = ma[j++][n_colm[k]] + columna[i];
		if(j == tam){ j = 0; k++;}
	}
}

void producto_matrices(int **ma, int *columna_resul, int tam,int n_per_proc, int *columna){
	int rep = 0, i_m = 0, i, j;
		
	while(rep < n_per_proc){
		for(i = 0; i < tam; i++){
			columna_resul[i_m] = 0;
			for(j=0; j < tam; j++)
				columna_resul[i_m] += ma[i][j] * columna[tam*rep + j];
			i_m++;
		}
		rep++;
	}
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