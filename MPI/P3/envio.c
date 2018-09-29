#include <stdio.h>
#include <mpi.h>
#define N 30
#define azul "\x1B[36m"
#define cerrar "\x1B[00m"

int main(int argc, char const *argv[]){

	int pid, VA[N], destino, tag, origen, ndat, i, resultado;
	MPI_Status info;

	MPI_Init(&argc,(char ***) &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);

		for(i=0; i<N; i++) VA[i] = 0;

		if(pid == 0){
			for(i=0; i<N; i++) VA[i] = i;
			
			destino = 1; tag = 0;
			MPI_Send(&VA[0], N, MPI_INT, destino, tag, MPI_COMM_WORLD);

			origen = 1; tag = 0;
			MPI_Recv(&resultado, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);

			MPI_Get_count(&info, MPI_INT, &ndat);
			printf("\n\n"azul" %d recibe resultado de PR %d: TAG: %d, ndat: %d VALOR: %d\n\n"cerrar, pid, info.MPI_SOURCE, info.MPI_TAG, ndat, resultado);
		
		}
		else if(pid == 1){
			origen = 0; tag = 0;
			MPI_Recv(&VA[0], N, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);
			
			for(i=0; i<N; i++) resultado += VA[i];
			
			destino = 0; tag = 0;
			MPI_Send(&resultado, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
		}
		
	MPI_Finalize();

	return 0;
}