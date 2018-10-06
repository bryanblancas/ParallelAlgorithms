#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

void my_gather(int * data, int * l,int num_elements_per_proc, MPI_Datatype datatype, int root, MPI_Comm communicator){
	int world_rank, world_size, ndat, *recv;
	MPI_Status status;
	MPI_Comm_rank(communicator, &world_rank);
	MPI_Comm_size(communicator, &world_size);
	recv = (int *)malloc(sizeof(int) * num_elements_per_proc);

	if(world_rank == root){
		//root put his values into data
		for (int i = 0; i < num_elements_per_proc; ++i) data[world_rank * num_elements_per_proc+i] = l[i];
		//receive values from others process
		for(int i = 0; i < world_size-1; i++){
			MPI_Recv(recv, num_elements_per_proc, datatype, MPI_ANY_SOURCE, 0, communicator, &status);
			MPI_Get_count(&status, MPI_INT, &ndat);
			//put values from any other process into data
			for (int j = 0; j < num_elements_per_proc; ++j) data[status.MPI_SOURCE * num_elements_per_proc + j] = recv[j];
		}
	}
	else
		MPI_Send(l, num_elements_per_proc, datatype, root, 0, communicator);
}

int main(int argc, char const *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage: compare_bcast num_elements_per_proc num_trials");
		exit(1);
	}

	int num_elements_per_proc = atoi(argv[1]);
	int num_trials = atoi(argv[2]);

	int world_size, i, world_rank;

	int *l = (int *)malloc(sizeof(int) * num_elements_per_proc);
	assert(l != NULL);

	MPI_Init(NULL, NULL);

		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		double total_my_gather_time = 0.0;
		double total_mpi_gather_time = 0.0;
		int tam = num_elements_per_proc * world_size;

		int *data = (int *) malloc(sizeof(int) * tam);
		assert(data != NULL);
		for (int i = 0; i < (tam); ++i) data[i] = -1;

		for(i=0; i < num_elements_per_proc; i++) l[i] = world_rank; 


		for(i=0; i<num_trials; i++){
			
			MPI_Barrier(MPI_COMM_WORLD);
			total_my_gather_time -= MPI_Wtime();
			my_gather(data, l, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			total_my_gather_time += MPI_Wtime();

			//print gathering from my_gather
			/*if(world_rank == 0){
				printf("Soy root %d y el arreglo data es : ",world_rank);
				for (int i = 0; i < (num_elements_per_proc * world_size); ++i)printf("%3d", *(data+i));	printf("\n");
			}*/

			MPI_Barrier(MPI_COMM_WORLD);
			total_mpi_gather_time -= MPI_Wtime();
			MPI_Gather(l, num_elements_per_proc, MPI_INT, data, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			total_mpi_gather_time += MPI_Wtime();

			//print gathering from MPI_Gather
			/*if(world_rank == 0){
				printf("Soy root %d y el arreglo data es : ",world_rank);
				for (int i = 0; i < (num_elements_per_proc * world_size); ++i)printf("%3d", *(data+i));	printf("\n");
			}*/
		}


		if(world_rank == 0){
			printf("Data size = %d bytes, Trials = %d\n",num_elements_per_proc * (int) sizeof(int), num_trials);
			printf("AVG my_Scatter time = %lf\n", total_my_gather_time/num_trials);
			printf("AVG MPI_Scatter time = %lf\n", total_mpi_gather_time/num_trials);
		}

	MPI_Finalize();
	free(l);
	free(data);
	return 0;
}

