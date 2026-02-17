#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("Run with exactly 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Request request;
    MPI_Status status;

    if (rank == 0)
    {
        int data[4] = {10, 20, 30, 40};

        printf("Master calling MPI_Issend...\n");
        MPI_Issend(data, 4, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        printf("Master doing other work...\n");

        MPI_Wait(&request, &status);
        printf("Master confirmed Issend completion.\n");
    }

    else if (rank == 1)
    {
        int data[4];
        int sum = 0;

        printf("Worker calling MPI_Recv...\n");
        MPI_Recv(data, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < 4; i++)
            sum += data[i];

        printf("Worker received data. Sum = %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}


/*

$ mpicc ssend_non_blocking.c -o ssend_non_blocking
$ mpiexec -n 2 ssend_non_blocking 

Worker calling MPI_Recv...
Master calling MPI_Issend...
Master doing other work...
Master confirmed Issend completion.
Worker received data. Sum = 100

*/