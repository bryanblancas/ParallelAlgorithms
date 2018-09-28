#include <stdio.h>
#include <mpi.h>
#define azul "\x1B[36m"
#define cerrar "\x1B[00m"
#define DECBIN(n, i) ((n&(1<<i))?1:0)

int test(int pid, int z){
	int v[16], i, cont = 0;
	for(i=0; i<16; i++) v[i] = DECBIN(z, i);
	if(
		((v[0] || v[1]) && (!v[1] || !v[3])) &&
		((v[2] || v[3]) && (!v[3] || !v[4])) &&
		((v[4] || !v[5]) && (v[5] || !v[6])) &&
		((v[5] || v[6]) && (v[6] || !v[15])) &&
		((v[7] || !v[8]) && (!v[7] || !v[13])) &&
		((v[8] || v[9]) && (v[8] || !v[9])) &&
		((!v[9] || !v[10]) && (v[9] || v[11])) &&
		((v[10] || v[11]) && (v[12] || v[13])) &&
		((v[13] || !v[14]) && (v[14] || v[15]))
	){	
		cont++;
		//printf("\n\n %d) \t", pid);
		//for (i = 15; i >= 0; i--)
		//	printf("%d",v[i]);
		//printf("\t(%d) \n\n", z);
	}
	return cont;
}

int main(int argc, char const *argv[]){

	int i, pid, npr, cont = 0, final = 0, destino, origen, ndat, tag;
	MPI_Status info;

	MPI_Init(&argc,(char***) &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &npr);
		MPI_Comm_rank(MPI_COMM_WORLD, &pid);

		for(i=pid; i<65536; i+= npr) cont += test(pid, i);

		if(pid != 0){
			destino = 0; tag = 0;
			MPI_Send(&cont, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
		}	
		else{
			final += cont;
			printf(azul" %d tiene CONT: %d\n"cerrar, pid, cont);

			origen = 1; tag = 0;
			MPI_Recv(&cont, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);
			MPI_Get_count(&info, MPI_INT, &ndat);
			printf("\n\n"azul" %d recibe resultado de PR %d: TAG: %d, ndat: %d CONT: %d\n\n"cerrar, pid, info.MPI_SOURCE, info.MPI_TAG, ndat, cont);
			final += cont;

			origen = 2; tag = 0;
			MPI_Recv(&cont, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);
			MPI_Get_count(&info, MPI_INT, &ndat);
			printf("\n\n"azul" %d recibe resultado de PR %d: TAG: %d, ndat: %d CONT: %d\n\n"cerrar, pid, info.MPI_SOURCE, info.MPI_TAG, ndat, cont);
			final += cont;

			origen = 3; tag = 0;
			MPI_Recv(&cont, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &info);
			MPI_Get_count(&info, MPI_INT, &ndat);
			printf("\n\n"azul" %d recibe resultado de PR %d: TAG: %d, ndat: %d CONT: %d\n\n"cerrar, pid, info.MPI_SOURCE, info.MPI_TAG, ndat, cont);
			final += cont;

			printf(" Número de soluciones: %d\n", final);
		}
	MPI_Finalize();
	
	return 0;
}