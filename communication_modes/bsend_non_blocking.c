#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
        int data[4] = {5, 6, 7, 8};

        // Compute required buffer size
        int pack_size;
        MPI_Pack_size(4, MPI_INT, MPI_COMM_WORLD, &pack_size);
        int buffer_size = pack_size + MPI_BSEND_OVERHEAD;

        void *buffer = malloc(buffer_size);
        MPI_Buffer_attach(buffer, buffer_size);

        printf("Master using MPI_Ibsend...\n");
        MPI_Ibsend(data, 4, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        printf("Master doing other work...\n");

        MPI_Wait(&request, &status);
        printf("Master confirmed Ibsend completion.\n");

        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    }

    else if (rank == 1)
    {
        int data[4];
        int sum = 0;

        MPI_Recv(data, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < 4; i++)
            sum += data[i];

        printf("Worker received data. Sum = %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}

/*

$ mpicc bsend_blocking.c -o bsend_blocking
$ mpiexec -n 2 bsend_non_blocking 

Master using MPI_Ibsend...
Master doing other work...
Master confirmed Ibsend completion.
Worker received data. Sum = 26

*/