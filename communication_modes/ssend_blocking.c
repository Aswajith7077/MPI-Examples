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

    if (rank == 0)
    {
        int data[4] = {1, 2, 3, 4};

        printf("Master calling MPI_Ssend...\n");
        MPI_Ssend(data, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Master finished MPI_Ssend.\n");
    }

    else if (rank == 1)
    {
        int data[4];
        int sum = 0;

        printf("Worker sleeping before receive...\n");

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

$ mpicc ssend_blocking.c -o ssend_blocking
$ mpiexec -n 2 ssend_blocking

Master calling MPI_Ssend...
Worker sleeping before receive...
Worker calling MPI_Recv...
Worker received data. Sum = 10
Master finished MPI_Ssend.

*/