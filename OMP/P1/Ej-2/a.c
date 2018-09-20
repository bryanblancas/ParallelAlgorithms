#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]){
	
	int n, *a, *b, *c, i;
	srand(time(NULL));
	if(argc != 2){
		printf("a.c n\n");
		return 0;
	}

	n = atoi(argv[1]);
	a = (int *) malloc(sizeof(int)*n);
	b = (int *) malloc(sizeof(int)*n);
	c = (int *) malloc(sizeof(int)*n);

	for(i=0;i<n;i++){
		*(a+i) = rand()%100;
		*(b+i) = rand()%100;
		*(c+i) = (*(a+i))*(*(b+i));
		printf("i=%d\t-> a=%d\t*  b=%d\t=  c=%d\n",i,*(a+i),*(b+i),*(c+i));
	}

	printf("c: ");
	for(i=0;i<n;i++)
		printf("%d ",*(c+i));
	printf("\n");
	return 0;
}