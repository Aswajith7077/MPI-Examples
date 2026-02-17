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

    int window_buffer = 0; // memory exposed
    MPI_Win win;

    MPI_Win_create(&window_buffer, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    // Synchronize before RMA
    MPI_Win_fence(0, win);

    if (rank == 1)
    {
        int value = 123;

        printf("Process 1 putting value into Process 0 memory...\n");

        MPI_Put(&value, 1, MPI_INT, 0, 0, 1, MPI_INT, win);
    }

    // Synchronize after RMA
    MPI_Win_fence(0, win);

    if (rank == 0)
    {
        printf("Process 0 sees updated value: %d\n", window_buffer);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}

/*

mpicc one_sided_communication.c -o one_sided_communication
mpiexec -n 2 one_sided_communication

Process 1 putting value into Process 0 memory...
Process 0 sees updated value: 123

*/