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
	int *a,*b,*c,*d, *x,*y,*z, *w,i, tam_vector;
	double t0,t1,t2,t3,t4;

	//validación de argumentos
	if(argc != 3){
		printf("calculo_pi.c  tam_vector  número_hilos\n");
		return -1;
	}

	tam_vector = atoi(argv[1]);
	printf("%d\n",tam_vector );
	a =(int *) malloc(sizeof(int)*tam_vector);
	b =(int *) malloc(sizeof(int)*tam_vector);
	c =(int *) malloc(sizeof(int)*tam_vector);
	d =(int *) malloc(sizeof(int)*tam_vector);
	x =(int *) malloc(sizeof(int)*tam_vector);
	y =(int *) malloc(sizeof(int)*tam_vector);
	z =(int *) malloc(sizeof(int)*tam_vector);
	w =(int *) malloc(sizeof(int)*tam_vector);

	for (i = 0; i < tam_vector; i++){
		a[i] = rand()%3;
		b[i] = rand()%3;
		c[i] = rand()%3;
		d[i] = rand()%3;
	}
	
	t0 = omp_get_wtime();
	#pragma omp parallel shared(a,b,c,d, tam_vector)
	{
		t1 = omp_get_wtime();
		#pragma omp for
			for(i = 0; i<tam_vector; i++){
				x[i] = a[i] - b[i];
				y[i] = b[i] + c[i];
			}
		t2 = omp_get_wtime();
		#pragma omp for
			for(i = 0; i<tam_vector; i++){
				w[i] = x[i] + y[i];
				z[i] = c[i] - d[i];
			}
		t3 = omp_get_wtime();
		#pragma omp for
			for(i = 0; i<tam_vector; i++)
				a[i] = w[i] * z[i];
	}
	t4 = omp_get_wtime();

	for(i = 0; i < tam_vector; i++){
		if(a[i]<0)
			printf("%d ",-a[i]);
		else
			printf("%d ", a[i]);
	}

	printf("\n Antes de parellel: %lf\n",t1-t0);
	printf("Primer for: %lf\n",t2-t1);
	printf("Segundo for: %lf\n",t3-t2);
	printf("Tercer for: %lf\n",t4-t3);
	printf("TIempo final: %lf\n",t4-t0);

	return 0;
}
