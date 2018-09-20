#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]){

	int np,th_id, order=0;

	if(argc != 2){
		printf("Introduce solamente el número de procesos a usar\n");
		return 0;
	}
	
	np = atoi(argv[1]);
	omp_set_num_threads(np);

	#pragma omp parallel private(th_id)
	{ 
		th_id = omp_get_thread_num();
		while(1){
			if(th_id == order){
				printf("Hilo: %d Hola mundo\n",th_id);
				order++;
				break;
			}
		}
	}

	return 0;
}