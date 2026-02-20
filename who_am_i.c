#include <mpi.h>
#include <stdio.h>


int main(int argc, char* argv[]){

	int number_of_processes,rank;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&number_of_processes);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	printf("The current process with id %d among %d processes\n",rank,number_of_processes);
	MPI_Finalize();
	return 0;

}
