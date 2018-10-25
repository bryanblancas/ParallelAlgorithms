#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char const *argv[]){

  	//variables del programa
	double area, pi, x;		
	int i, n, thrs; 				//n número de rectángulos

	//validación de argumentos
	if(argc != 3){
		printf("calculo_pi.c  número_divisiones  número_hilos\n");
		return -1;
	}

	//conversión de argumentos
	n = atoi(argv[1]);
	omp_set_num_threads(atoi(argv[2]));

	area = 0.0;
	#pragma omp parallel 
	{
		if(omp_get_thread_num()==0)
			printf("There are %d threads!\n",omp_get_num_threads());

		#pragma omp for private(x) reduction(+:area) 
		for(i=0 ; i<n ; i++){
			x = (i + 0.5)/n;			//encontrar el x actual (rectángulo central)
			area += 4.0/(1.0 + (x*x)); 	//área en x
		}
	}
	pi = area/n;	//pi = área entre todos las divisiones
	printf("Pi: %.5lf\n",pi);
 	
 	return 0;
} 