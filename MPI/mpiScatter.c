#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

int main(int argc, char const *argv[]){
	
	MPI_Init(NULL, NULL);
		
		if(world_rank == 0)
			rand_nums = create_rand_nums(elements_per_proc * world_size);
		int elements_per_proc = 10;
		float *sub_rand_nums = (float *) malloc(sizeof(float) * elements_per_proc);

		MPI_Scatter(rand_nums, elements_per_proc, MPI_FLOAT, sub_rand_nums, elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

		float sub_avg = compute_avg(sub_rand, elements_per_proc);
		float *sub_avgs = NULL;

		if(world_rank == 0)
			sub_avgs = malloc(sizeof(float)*world_size);

		MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

		if(world_rank == 0)
			float avg = compute_avg(sub_avgs, world_size);

	MPI_Finalize();

	return 0;
}