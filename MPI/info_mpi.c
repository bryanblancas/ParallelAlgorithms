#include <mpi.h> 
#include <stdio.h>
int main(int argc, char const *argv[]){

	int rank, size, lnom;
	char nombrepr[MPI_MAX_PROCESSOR_NAME];
	int a = 2;

	printf("%d\n",a);

	MPI_Init(&argc,(char ***) &argv);

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		MPI_Get_processor_name(nombrepr, &lnom);

		a+=1;
		printf("Hola mundo! soy el proceso %d de %d activado en %s, a = %d\n",rank, size, nombrepr, a);

	MPI_Finalize();

	return 0;
}
