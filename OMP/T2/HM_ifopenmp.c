#include <stdio.h>

/*
	Generación de varios Threads y modificación del número de threads mediante: 
	export OMP_NUM_THREADS
*/

#ifdef _OPENMP //si se puso -fopenmp en la compilación
	#include <omp.h>
	#define TRUE 1
	#define FALSE 0
#else //si no existe la biblioteca, el programa asignará números a las funciones de omp
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
#endif

#define N 24

int th_id, n_thrs;
int i, A[N];

int main(int argc, char const *argv[]){
	
	for (int i = 0; i < N; i++) A[i]=0;

	//MODIFICACIÓN HECHA
	//Establecer el número de hilos a usar igual a N para que el arreglo quede completamente lleno
	omp_set_num_threads(N);

	#pragma omp parallel private(th_id)
	{
		n_thrs = omp_get_num_threads();
		th_id = omp_get_thread_num();
		printf("Thread %d de %d en marcha \n",th_id, n_thrs);
		A[th_id] = th_id+10;
		printf("EL thread %d ha terminado\n", th_id);
	}

	for(i=0; i<N; i++) printf("A(%d) = %d \n", i, A[i]);

	return 0;
}