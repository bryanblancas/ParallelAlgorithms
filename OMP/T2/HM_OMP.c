#include <omp.h>
#include <stdio.h>

int main(int argc, char const *argv[]){

	int p, th_id;
	p = omp_get_num_procs();	//obtener número de procesadores de la computadora actual
	omp_set_num_threads(p);		//establece el número de hilos a trabajar 
	

	#pragma omp parallel private(th_id)
	{
		th_id = omp_get_thread_num();
		printf("Hola mundo desde hilo %d\n",th_id);
	}

	return 0;
}