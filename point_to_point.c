#include <mpi.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
    int rank,value;
    int tag=100;

    MPI_Status statut_msg;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 2) {
        value = 1000;
        MPI_Send(&value,1,MPI_INT,5,tag,MPI_COMM_WORLD);
    } else if ( rank == 5){
        MPI_Recv(&value,1,MPI_INT,2,tag,MPI_COMM_WORLD,&statut_msg);
        printf("I, process 5, I received %d from the process 2.\n", value);
    }

    MPI_Finalize();
 }
