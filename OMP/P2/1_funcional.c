#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP 
	#include <omp.h>
#else 
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
#endif

int* alpha(int *a, int *b, int tam_vector);
int* beta(int *b, int *c, int tam_vector);
int* delta(int *c, int *d, int tam_vector);
int* gama(int *a, int *b, int tam_vector); //recibe dos parámetros
int* epsilon(int *a, int *b, int tam_vector); // recibe dos parámetros

int main(int argc, char const *argv[]){
	
	srand(time(NULL));
	int *a,*b,*c,*d, *x,*y,*z, *w,i, tam_vector;
	double t0,t1,t2,t3;

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
	#pragma omp parallel sections shared(a,b,c,d, tam_vector)
	{
		t1 = omp_get_wtime();
		#pragma omp section
			x = alpha(a,b,tam_vector);
		#pragma omp section
			y = beta(b,c,tam_vector);
		#pragma omp section
			z = delta(c,d,tam_vector);
	}
	t2 = omp_get_wtime();
	w = gama(x, y, tam_vector);
	x = epsilon(w,z, tam_vector);
	t3 = omp_get_wtime();

	for(i = 0; i < tam_vector; i++){
		if(x[i]<0)
			printf("%d ",-x[i]);
		else
			printf("%d ", x[i]);
	}

	printf("\nSecciones paralelas: %lf\n",t2-t1);
	printf("Ejecución paralela gamma y epsilon: %lf\n",t3-t2);
	printf("Tiempo total: %lf\n",t3-t0);

	return 0;
}

int* alpha(int *a, int *b, int tam_vector){
	int *x;
	x =(int *) malloc(sizeof(int)*tam_vector);
	for(int i = 0; i < tam_vector; i++)
		x[i] = a[i] - b[i];
	return x;
}
int* beta(int *b, int *c, int tam_vector){
	int *x;
	x =(int *) malloc(sizeof(int)*tam_vector);
	for(int i = 0; i < tam_vector; i++)
		x[i] = b[i] + c[i];
	return x;
}
int* delta(int *c, int *d, int tam_vector){
	int *x;
	x =(int *) malloc(sizeof(int)*tam_vector);
	for(int i = 0; i < tam_vector; i++)
		x[i] = c[i] - d[i];
	return x;
}
int* gama(int *a, int *b, int tam_vector){
	int *x;
	x =(int *) malloc(sizeof(int)*tam_vector);
	for(int i = 0; i < tam_vector; i++)
		x[i] = a[i] + b[i];
	return x;
}
int* epsilon(int *a, int *b, int tam_vector){
	int *x;
	x =(int *) malloc(sizeof(int)*tam_vector);
	for(int i = 0; i < tam_vector; i++)
		x[i] = a[i] * b[i];
	return x;
}