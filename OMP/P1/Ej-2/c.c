#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
//este programa cumple con el inciso c y d (ordenar por th_id)

int main(int argc, char const *argv[]){
	
	int n, np, *a, *b, *c, i, th_id, order = 0;
	srand(time(NULL));
	if(argc != 3){
		printf("b.c n np\n");
		return 0;
	}

	n = atoi(argv[1]);
	np = atoi(argv[2]);
	a = (int *) malloc(sizeof(int)*n);
	b = (int *) malloc(sizeof(int)*n);
	c = (int *) malloc(sizeof(int)*n);
	omp_set_num_threads(np);

	#pragma omp parallel private(th_id, i) shared(order)
	{
		th_id = omp_get_thread_num();
		#pragma omp for
			for(i=0;i<n;i++){
				while(1){
					if(i == order){
						*(a+i) = rand()%100;
						*(b+i) = rand()%100;
						*(c+i) = (*(a+i))*(*(b+i));
						printf("THREAD:%d\t i=%d\t-> a=%d\t*  b=%d\t=  c=%d\n",th_id,i,*(a+i),*(b+i),*(c+i));
						order++;
						break;
					}
				}
			}
	}

	printf("c: ");
	for(i=0;i<n;i++)
		printf("%d ",*(c+i));
	printf("\n");

	return 0;
}