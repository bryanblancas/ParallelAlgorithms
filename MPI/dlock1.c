//ENVIO DE VARIABLES 

#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[]){
	
	int pid, origen, destino, tag;
	int a,b,c;
	MPI_Status info;

	MPI_Init(&argc, (char***) argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);

		if(pid == 0){
			a = 5;
			
			printf("\n >> recibiendo datos de P1 \n");
			origen = 1; tag = 1;
			MPI_Recv(&b, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);
			
			printf("\n >> enviando datos a P1 \n");
			destino = 1; tag = 0;
			MPI_Send(&a, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
			
			c = a + b;
			printf("\n C es %d en proc 0 \n\n", c); 
		}
		else if(pid == 1){
			b = 10;

			printf("\n >> enviando datos a P0 \n");
			destino = 0; tag = 1;
			MPI_Send(&b, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);

			b = 6;
			printf("\n >> recibiendo datos de P0 \n");
			origen = 0; tag = 0;
			MPI_Recv(&a, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);

			c = a + b;
			printf("\n C es %d en proc 1 \n", c);
		}

	int i = MPI_Finalize();

	printf("%d\n",i);

	return 0;
}