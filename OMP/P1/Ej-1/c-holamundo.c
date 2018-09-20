#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){

	int np, th_id;

	if(argc != 2){
		printf("Introduce solamente el número de procesos a usar\n");
		return 0;
	}
	
	np = atoi(argv[1]);
	omp_set_num_threads(np);

	#pragma omp parallel private(th_id)
	{ 
		th_id = omp_get_thread_num();
		printf("Hilo: %d Hola mundo\n",th_id);
	}
	
	return 0;
}