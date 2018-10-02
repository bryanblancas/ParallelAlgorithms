//el programa se bloquea cuando el número de enteros a recibir, rebasa el tamaño del arreglo

#include <stdio.h>
#include <mpi.h>

#define n 1000

int main(int argc, char const *argv[]){
	
	int pid, kont;
	int a[n], b[n], c[n], d[n];
	MPI_Status status;

	MPI_Init(&argc, (char***) argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);
		for (kont=100; kont<=n; kont+=100){
			if (pid == 0){
				int z = MPI_Send(&a[0], kont, MPI_INT, 1, 0, MPI_COMM_WORLD);
				printf("send 1 %d\n",z);
				z = MPI_Recv(&b[0], kont, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
				printf("emisor %d --- %d\n", kont, z);
			}
			else{
				int z = MPI_Recv(&d[0], kont, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				printf(" receptor %d --- %d\n", kont, z);
				z = MPI_Send(&c[0], kont, MPI_INT, 0, 0, MPI_COMM_WORLD);
				printf("sen 1 --- %d\n",z);
			}
		} 
	int i = MPI_Finalize();
	printf("%d\n", i);
	return 0;
}