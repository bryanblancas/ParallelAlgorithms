#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define azul "\x1B[36m"
#define cerrar "\x1B[00m"
#define N 10

void print_vector(int *m, int n){
	for(int i = 0; i<n ; i++)
		printf("%d\t",*(m+i));
	printf("\n");
} 

int main(int argc, char const *argv[]){

	int a[N], b[N], c[N], i, pid, size, destino, tag, ndat;
	srand(time(NULL)); 
	MPI_Status info;

	for(i=0 ; i<N ; i++){
		a[i] = rand()%11+10;
		b[i] = rand()%11+10;
		c[i] = 0;
	}

	MPI_Init(&argc,(char ***) &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		for(i=0 ; i<N; i++)
			c[i] = a[i] + b[i];
		c[0] = pid;  //identificador del arreglo con pid en la casilla 0

		if(pid == 0){
			tag = 0;
			printf(azul"\n\n %d mi vector es: \n"cerrar,pid);
			print_vector(c, N);
			for(i = 0; i < size-1; i++){
				MPI_Recv(&c[0], N, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &info);
				MPI_Get_count(&info, MPI_INT, &ndat);
				printf("\n\n"azul" %d recibe vector de %d y el vector es\n"cerrar, pid, info.MPI_SOURCE);
				print_vector(c, N);
			}
		}
		else if(pid != 0){
			destino = 0; tag = 0;
			MPI_Send(&c[0], N, MPI_INT, destino, tag, MPI_COMM_WORLD);
		}

	MPI_Finalize();

	return 0;
}