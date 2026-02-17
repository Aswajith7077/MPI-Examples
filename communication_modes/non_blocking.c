#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    int rank, size;
    MPI_Init(&argc, &argv);

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
        // Master
        int data[4] = {1, 2, 3, 4};
        int result;

        printf("Master initiating non-blocking send...\n");
        MPI_Isend(data, 4, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        // Do other work here
        printf("Master doing other work...\n");

        MPI_Wait(&request, &status);
        printf("Master send completed.\n");

        // Receive result non-blocking
        MPI_Irecv(&result, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

        printf("Master doing more work...\n");

        MPI_Wait(&request, &status);
        printf("Master received sum = %d\n", result);
    }

    else if (rank == 1)
    {
        // Worker
        int data[4];
        int sum = 0;

        printf("Worker initiating non-blocking receive...\n");
        MPI_Irecv(data, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        printf("Worker doing other work...\n");

        MPI_Wait(&request, &status);
        printf("Worker received data.\n");

        for (int i = 0; i < 4; i++)
            sum += data[i];

        printf("Worker initiating non-blocking send...\n");
        MPI_Isend(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

        MPI_Wait(&request, &status);
        printf("Worker send completed.\n");
    }

    MPI_Finalize();
    return 0;
}

/*

$ mpicc non_blocking.c -o non_blocking
$ mpiexec -n 2 non_blocking
56
Master initiating non-blocking send...
Worker initiating non-blocking receive...
Master doing other work...
Master send completed.
Master doing more work...
Master received sum = 10
Worker doing other work...
Worker received data.
Worker initiating non-blocking send...
Worker send completed.


*/