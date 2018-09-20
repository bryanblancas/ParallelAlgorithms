#include <omp.h>
#include <stdio.h>

int main(int argc, char const *argv[]){
	
	int iam = 0, np = 1, i = 0;

	//MODIFICACIÓN HECHA
	//iam debe de ser privada porque si no el th_id se repetiría, np  también 
	#pragma omp parallel private(i, iam, np)
	{
		#if defined (_OPENMP)
			np = omp_get_num_threads();
			iam = omp_get_thread_num();
		#endif
		printf("Hello from thread %d out of %d\n", iam,np);
		
		#pragma omp for
			for(i = 0; i < (np*2) ; i++){
				printf("Thread %d, contador %d \n", iam, i);
			}

	}

	return 0;
}

//las variables del pragma son privadas porque sólo dentro de ese hilo existirán esas variables
//para que las variables no se confundan