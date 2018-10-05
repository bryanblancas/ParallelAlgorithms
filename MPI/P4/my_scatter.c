#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char const *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage: compare_bcast num_elements num_trials");
		exit(1);
	}

	int num_elements = atoi(argv[1]);
	int num_trials = atoi(argv[2]);

	MPI_Init(NULL, NULL);

		int world_rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		double total_my_scatter_time = 0.0;
		double total_mpi_scatter_time = 0.0;

	MPI_Finalize();

	return 0;
}

