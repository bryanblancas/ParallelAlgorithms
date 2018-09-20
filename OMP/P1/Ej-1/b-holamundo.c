#include <omp.h>
#include <stdio.h>

int main(int argc, char const *argv[]){
	
	#pragma omp parallel
	{ 
		printf("Hola Mundo\n");
	}
	return 0;
}