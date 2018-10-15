#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[]){

	MPI_Init(NULL,NULL);

		int world_rank, world_size;
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		MPI_Group world_group;
		MPI_Comm_group(MPI_COMM_WORLD, &world_group);

		int n = 2;
		const int ranks[2] = {1,3};

		MPI_Group odd_group;
		MPI_Group_incl(world_group, n, ranks, &odd_group);

		MPI_Comm odd_comm;
		MPI_Comm_create_group(MPI_COMM_WORLD, odd_group, 0, &odd_comm);

		int odd_rank = -1, odd_size = -1;
		if(MPI_COMM_NULL != odd_comm){
			MPI_Comm_rank(odd_comm, &odd_rank);
			MPI_Comm_size(odd_comm, &odd_size);
		}

		printf("WORLD RANK/SIZE: %d/%d --------> PRIME RANK/SIZE %d/%d\n",world_rank, world_size, odd_rank, odd_size);

		MPI_Group_free(&world_group);
		MPI_Group_free(&odd_group);

		if(MPI_COMM_NULL != odd_comm)
			MPI_Comm_free(&odd_comm);

	MPI_Finalize();

	return 0;
}