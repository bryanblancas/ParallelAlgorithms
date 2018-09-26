#include <mpi.h> 
#include <stdio.h>
int main(int argc, char const *argv[]){
	
	MPI_Init(&argc,(char ***) &argv);

	printf("Hola mundo mpi\n");

	MPI_Finalize();

	return 0;
}
