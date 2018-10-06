#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

void my_scatter(int * data, int numdatos, MPI_Datatype datatype, int root, MPI_Comm communicator){
	int i, *l, world_rank, world_size;
	l = (int *)malloc(sizeof(int) * numdatos);
	MPI_Comm_rank(communicator, &world_rank);
	MPI_Comm_size(communicator, &world_size);
	
	if(world_rank == root){
		for(i=0; i < world_size; i++)
			if(i != world_rank){
				//extracting values to send
				for(int j = 0; j < numdatos; j++) l[j] = data[i*numdatos+j];
				//sending values
				MPI_Send(l, numdatos, datatype, i, 0, communicator);
			}
	}
	else
		MPI_Recv(l, numdatos, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
		/*printf("PR %d me llego: ",world_rank);
		for (int i = 0; i < numdatos; ++i){
			printf("%5d",l[i]);
		}
		printf("\n");*/

}

int main(int argc, char const *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage: compare_bcast num_elements_per_proc num_trials");
		exit(1);
	}

	int num_elements_per_proc = atoi(argv[1]);
	int num_trials = atoi(argv[2]);
	int world_size, world_rank, *l, i;
	l = (int *)malloc(sizeof(int) * num_elements_per_proc);

	MPI_Init(NULL, NULL);

		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		double total_my_scatter_time = 0.0;
		double total_mpi_scatter_time = 0.0;

		int* data = (int *) malloc(sizeof(int) * num_elements_per_proc * world_size);
		assert(data != NULL);
		for(i=0; i< (num_elements_per_proc*world_size); i++) data[i] = i;

		for(i=0; i<num_trials; i++){

			MPI_Barrier(MPI_COMM_WORLD);
			total_my_scatter_time -= MPI_Wtime();
			my_scatter(data, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			total_my_scatter_time += MPI_Wtime();

			MPI_Barrier(MPI_COMM_WORLD);
			total_mpi_scatter_time -= MPI_Wtime();
			MPI_Scatter(data, num_elements_per_proc, MPI_INT, l, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			total_mpi_scatter_time += MPI_Wtime();

		}

		if(world_rank == 0){
			printf("Data size = %d bytes, Trials = %d\n",num_elements_per_proc * (int) sizeof(int), num_trials);
			printf("AVG my_Scatter time = %lf\n", total_my_scatter_time/num_trials);
			printf("AVG MPI_Scatter time = %lf\n", total_mpi_scatter_time/num_trials);
		}

	MPI_Finalize();
	free(data);
	return 0;
}

