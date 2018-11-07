#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
	int rank, size;
	double t;

	MPI_Init(NULL,NULL);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		if(rank == 0) t -= MPI_Wtime();
		sleep(5);
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == 0) t += MPI_Wtime();

	MPI_Finalize();

	if(rank == 0){
		FILE *a;
		a = fopen("tiempos.txt", "a");
		fprintf(a,"\n%5d\t%lf", size, t);
		fclose(a);
	}

	return 0;
}