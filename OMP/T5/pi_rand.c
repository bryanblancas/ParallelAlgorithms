#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define a_cuadrado 4.0

int main(int argc, char const *argv[]){
	
	srand48(time(NULL));
	double x, y, pi;
	int p_circulo, p_totales, i, p;

	//validación de argumentos
	if(argc != 3){
		printf("calculo_pi.c  número_puntos  número_hilos\n");
		return -1;
	}

	//conversión de argumentos
	p_totales = atoi(argv[1]);
	omp_set_num_threads(atoi(argv[2]));

	p_circulo = 0;
	#pragma omp parallel for private(x,y) reduction(+:p_circulo)
		for(i=0 ; i < p_totales/4; i++){
			x = drand48();
			y = drand48();
			if((x*x+y*y) <= 1)
				p_circulo += 1;
		}
	
	p_circulo *= 4;
	printf("%d\n",p_circulo);

	pi =(double) a_cuadrado*((double) p_circulo/p_totales);
	printf("%0.5lf\n",pi);

	return 0;
}

//con p_totales = 1000000 -> p_circulo = 785375 : pi = 3.14150