#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define azul "\x1B[36m"
#define cerrar "\x1B[00m"
#define N 10

void print_matriz(int *m, int n){
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j)
			printf("%d\t",m[i*N+j]);
		printf("\n");
	}
}

int main(int argc, char const *argv[]){

	int a[N][N], b[N][N], c[N*N], i, j, pid, size, tag=0, ndat, destino;
	srand(time(NULL)); 
	MPI_Status info;

	for(i=0 ; i<N ; i++){
		for(j=0 ; j<N ; j++){
			a[i][j] = rand()%11+10;
			b[i][j] = rand()%11+10;
			c[i*N+j] = 0;	
		}
	}

	MPI_Init(&argc,(char ***) &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		for(i=0 ; i<N ; i++){
			for(j=0 ; j<N ; j++){
				if(i!=j)
					c[i*N+j] = a[i][j] + b[i][j];
				else
					c[i*N+j] = pid;
			}
		}

		if(pid == 0){
			printf(azul" %d mi matriz es: \n"cerrar, pid);
			print_matriz(c, N);
			for(i = 0; i< size-1; i++){
				MPI_Recv(&c[0], N*N, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &info);
				MPI_Get_count(&info, MPI_INT, &ndat);
				printf("\n\n"azul" %d recibe matriz de %d y la matriz es: \n"cerrar, pid, info.MPI_SOURCE);
				print_matriz(c, N);
			}
		}
		else{
			destino = 0; tag = 0;
			MPI_Send(&c[0], N*N, MPI_INT, destino, tag, MPI_COMM_WORLD);
		}

	MPI_Finalize();

	return 0;
}