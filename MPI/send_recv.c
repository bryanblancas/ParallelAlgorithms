#include <mpi.h> 
#include <stdio.h>

/*
SEND Y RECV
	MPI_Send(start, count, datatype, dest, tag, comm)  //bloqueante
		1. start: puntero a los datos a enviar
		2. count: número de elementos a enviar
		3. datatype: tipo de dato 
		4. dest: identificación del proceso destino
		5. tag: etiqueta de la comunicación
		6. comm: identificación del comunicador

	MPI_Recv(start, count, datatype, source, tag, comm, status)
		1. start: puntero para la recepción de datos
		2. count: número de elementos
		3. datatype: tipo de dato
		4. source: identificación del proceso origen
		5. tag: etiqueta de la comunicación
		6. comm: identificacipon del comunicador
		7. status: puntero para acceso a la información sobre el mensaje

*/

#define TAG 10
int main(int argc, char const *argv[]){

	char msg[100];
	int source, dest, myrank;
	MPI_Status info;

	MPI_Init(&argc,(char ***) &argv);
		
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

		if(myrank == 0){
			dest = 1;
			sprintf(msg, "\n Esto es un mensaje del proceso %d al proceso %d", myrank, dest);
			MPI_Send(msg, 100, MPI_CHAR, dest, TAG, MPI_COMM_WORLD);
			printf("Mensaje enviado a %d desde %d\n",dest, myrank);
		}
		else if(myrank == 1){
			MPI_Recv(msg, 100, MPI_CHAR, source, TAG, MPI_COMM_WORLD,&info);
			printf("Mensaje recibido en %d de %d\n", myrank, source);
			printf("%s\n",msg);
		}

	MPI_Finalize();

	return 0;
}
