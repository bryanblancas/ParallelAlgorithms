//el programa se bloquea cuando el número de enteros a recibir, rebasa el tamaño del arreglo

#include <stdio.h>
#include <mpi.h>

#define n 200

int main(int argc, char const *argv[]){
	
	int pid, kont;
	int a[n], b[n], c[n], d[n];
	MPI_Status status;

	MPI_Init(&argc, (char***) argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);
		for (kont=100; kont<=n; kont+=100){
			if (pid == 0){
				MPI_Send(&a[0], kont, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&b[0], kont, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
				printf("emisor %d \n", kont);
			}
			else{
				MPI_Send(&c[0], kont, MPI_INT, 0, 0, MPI_COMM_WORLD);
				MPI_Recv(&d[0], kont, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				printf(" receptor %d \n", kont);
			}
		} 
	MPI_Finalize();

	return 0;
}