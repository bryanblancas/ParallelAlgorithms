#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

int fpow(int exponent){
	if(exponent%2 == 0)
		return 1;
	return -1;
}

int main(int argc, char const *argv[]){
	
	int i, n;
	double sum;

	//validación de argumentos
	if(argc != 3){
		printf("calculo_pi.c  lím_suma  número_hilos\n");
		return -1;
	}

	//conversión de argumentos
	n = atoi(argv[1]);
	omp_set_num_threads(atoi(argv[2]));

	sum = 0.0;
	#pragma omp parallel for reduction(+:sum)
	for(i = 0; i < n ; i++){
		sum += (double) fpow(i)/((2*i)+1);
	}

	sum *= 4;
	printf("%0.5lf\n",sum);
	return 0;
}
