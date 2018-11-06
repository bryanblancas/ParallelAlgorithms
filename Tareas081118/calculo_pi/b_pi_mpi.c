#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define a_cuadrado 4.0

int main(int argc, char const *argv[]){
	
	srand48(time(NULL));
	double x, y, pi, time, time1;
	int p_circulo, p_circulo_final, p_totales, i, p_por_proceso, size, rank;

	//validación de argumentos
	if(argc != 2){
		printf("b_pi_mpi.c  número_puntos\n");
		return -1;
	}

	//conversión de argumentos
	p_totales = atoi(argv[1]);

	p_circulo = 0;
	p_circulo_final = 0;
	MPI_Init(NULL,NULL);
		
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		
		p_por_proceso = p_totales/size;

		time -= MPI_Wtime();
		for(i=0 ; i < p_por_proceso; i++){
			x = drand48();
			y = drand48();
			if((x*x+y*y) <= 1)
				p_circulo += 1;
		}
		time += MPI_Wtime();
		
		MPI_Reduce(&p_circulo, &p_circulo_final, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		
		if(rank == 0){
			
			time1 -= MPI_Wtime();
			pi =(double) a_cuadrado*((double) p_circulo_final/p_totales);
			time1 += MPI_Wtime();
			
			printf("%0.5lf\n",pi);	
			printf("Time: %lf\n", time+time1);	
		}
	
	MPI_Finalize();
	
	return 0;
}

//con p_totales = 1000000 -> p_circulo = 785375 : pi = 3.14150