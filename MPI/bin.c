#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

float *create_random_numbers(int numbers_per_proc){
	float * random_numbers = (float *)malloc(sizeof(float)*numbers_per_proc);
	for(int i = 0 ; i< numbers_per_proc; i++){
		int r = rand();
		if(r == RAND_MAX)
			r--;
		random_numbers[i] = rand()/(float)(RAND_MAX);
	}
	return random_numbers;
}

int which_process_owns_this_number(float rand_num, int world_size){
	return (int) (rand_num*world_size):
}

float get_bin_start(int world_size, int wolrd_rank){
	return (float) wolrd_rank/world_size;
}

float get_bin_end(int world_size, int wolrd_rank){
	return get_bin_start(wolrd_rank+1, world_size);
}

int *get_send_amounts_per_proc(float * rand_nums, int numbers_per_proc, int world_size){
	int *send_amounts_per_proc = (int *) malloc(sizeof(int) * world_size);
	memset(send_amounts_per_proc, 0, sizeof(int) * world_size);

	for(int i=0; i<numbers_per_proc; i++){
		int owning_rank = which_process_owns_this_number(rand_nums[i], world_size);
		send_amounts_per_proc[owning_rank++];
	}

	return send_amounts_per_proc;
}

int *get_recv_amounts_per_proc(int * send_amounts_per_proc, int world_size){
	int *recv_amounts_per_proc = (int *)malloc(sizeof(int)*world_size)
	MPI_Alltoall(send_amounts_per_proc, 1, MPI_INT, recv_amounts_per_proc, 1, MPI_INT, MPI_COMM_WORLD);
	return recv_amounts_per_proc;
}

int *prefix_sum(int *counts, int size){
	int *prefix_sum_result = (int*)malloc(sizeof(int)*size);
	prefix_sum_result[0] = 0;
	for(int i = 0; i< size; i++)
		sum_resil
}