#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define a_cuadrado 4.0

int main(int argc, char const *argv[]){
	
	srand48(time(NULL));
	double x, y, pi;
	int p_circulo, p_totales, i, p;

	//validación de argumentos
	if(argc != 2){
		printf("b_pi_mpi.c  número_puntos\n");
		return -1;
	}

	//conversión de argumentos
	p_totales = atoi(argv[1]);

	p_circulo = 0;

	clock_t t; 
    t = clock(); 
    
	for(i=0 ; i < p_totales; i++){
		x = drand48();
		y = drand48();
		if((x*x+y*y) <= 1)
			p_circulo += 1;
	}

	pi =(double) a_cuadrado*((double) p_circulo/p_totales);
    
    t = clock() - t; 
    double time = ((double)t)/CLOCKS_PER_SEC; 

	printf("%0.5lf\n",pi);
	printf("Time: %lf\n", time);	
	return 0;
}

//con p_totales = 1000000 -> p_circulo = 785375 : pi = 3.14150