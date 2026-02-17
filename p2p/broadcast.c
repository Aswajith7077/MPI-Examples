#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 2)
        value = rank + 1000;

    MPI_Bcast(&value, 1, MPI_INT, 2, MPI_COMM_WORLD);

    printf("I, process %d, received %d of process 2\n", rank, value);

    MPI_Finalize();
}

/*

$ mpiexec -n 7 broadcast

I, process 2, received 1002 of process 2
I, process 3, received 1002 of process 2
I, process 0, received 1002 of process 2
I, process 4, received 1002 of process 2
I, process 5, received 1002 of process 2
I, process 6, received 1002 of process 2
I, process 1, received 1002 of process 2

*/
