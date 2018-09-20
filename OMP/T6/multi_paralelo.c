#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP 
	#include <omp.h>
#else 
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
#endif

int main(int argc, char const *argv[]){
	
	srand(time(NULL));
	int **a, *x, *y, i, j, tam_matriz, n_td, iam;
	//comentariodeprueba	
	tam_matriz = atoi(argv[1]);
	n_td = atoi(argv[2]);
	x =(int *) malloc(sizeof(int)*tam_matriz);
	y =(int *) malloc(sizeof(int)*tam_matriz);
	a = (int **) malloc(sizeof(int *) * tam_matriz);
	for(i = 0 ; i < tam_matriz ; i++)
		a[i] = (int *) malloc(sizeof(int) * tam_matriz);
	for(i = 0 ; i < tam_matriz ; i++){
		x[i] = rand()%10;
		for(j = 0 ; j < tam_matriz ; j++)
			a[i][j] = rand()%10;
	}

	omp_set_num_threads(n_td);

	
	double empezar = omp_get_wtime();
	#pragma omp parallel for private(j)
		for (i = 0; i < tam_matriz; ++i){
			y[i] = 0;
			for(j = 0; j < tam_matriz; j++){
				y[i] += a[i][j] * x[j]; 
			}
		}
	double terminar = omp_get_wtime();
	printf("Tiempo: %lf\n",terminar-empezar);

	return 0;
}

/*for(i = 0 ; i < tam_matriz ; i++){
	for(j = 0 ; j < tam_matriz ; j++)
		printf("%d ",a[i][j]);
	printf("\n");
}
printf("---------------------------------------\n")f
for(j = 0 ; j < tam_matriz ; j++)
	printf("%d\n",x[j]);
printf("---------------------------------------\n");
for(j = 0 ; j < tam_matriz ; j++)
	printf("%d\n",y[j]);*/	
