#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define a_cuadrado 4.0

int main(int argc, char const *argv[]){
	
	srand48(time(NULL));
	double x, y, pi, pi_reduce, time;
	int p_circulo, p_totales, i, size, rank;

	//validación de argumentos
	if(argc != 2){
		printf("b_pi_mpi.c  número_puntos\n");
		return -1;
	}

	//conversión de argumentos
	p_totales = atoi(argv[1]);

	p_circulo = 0;
	MPI_Init(NULL,NULL);
		
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		time -= MPI_Wtime();
		for(i=0 ; i < p_totales; i++){
			x = drand48();
			y = drand48();
			if((x*x+y*y) <= 1)
				p_circulo += 1;
		}
		pi =(double) a_cuadrado*((double) p_circulo/p_totales);
		//printf("Soy el proceso %d y calcule %lf\n", rank, pi);
		time += MPI_Wtime();
		
		MPI_Reduce(&pi, &pi_reduce, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		if(rank == 0){
			
			printf("%0.5lf\n",pi_reduce/4);	
			printf("Time: %lf\n", time);	
		}
	
	MPI_Finalize();
	
	return 0;
}

//con p_totales = 1000000 -> p_circulo = 785375 : pi = 3.14150