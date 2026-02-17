#include <mpi.h>
#include <stdio.h>

/*

int MPI_Win_create(
    void *base,
    MPI_Aint size,
    int disp_unit,
    MPI_Info info,
    MPI_Comm comm,
    MPI_Win *win
);

| Parameter   | Meaning                                      |
| ----------- | -------------------------------------------- |
| `base`      | Starting address of exposed memory           |
| `size`      | Size of memory (bytes)                       |
| `disp_unit` | Scaling factor for displacements             |
| `info`      | Optimization hints (`MPI_INFO_NULL` usually) |
| `comm`      | Communicator                                 |
| `win`       | Output window object                         |



int MPI_Put(
    const void *origin_addr,
    int origin_count,
    MPI_Datatype origin_datatype,
    int target_rank,
    MPI_Aint target_disp,
    int target_count,
    MPI_Datatype target_datatype,
    MPI_Win win
);


| Parameter         | Meaning                     |
| ----------------- | --------------------------- |
| `origin_addr`     | Local buffer                |
| `origin_count`    | Number of elements to send  |
| `origin_datatype` | Datatype of local data      |
| `target_rank`     | Rank of remote process      |
| `target_disp`     | Offset inside remote window |
| `target_count`    | Number of remote elements   |
| `target_datatype` | Datatype at remote side     |
| `win`             | Window object               |


int MPI_Win_fence(
    int assert,
    MPI_Win win
);


| Parameter | Meaning                        |
| --------- | ------------------------------ |
| `assert`  | Optimization hints (usually 0) |
| `win`     | Window                         |



*/

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