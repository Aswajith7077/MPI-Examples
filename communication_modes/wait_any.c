#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // sleep()

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3)
    {
        if (rank == 0)
            printf("Run with exactly 3 processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
    {
        int send_val = 10;
        int recv_vals[2];

        MPI_Request requests[2];
        MPI_Status status;

        // Send to workers
        MPI_Send(&send_val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&send_val, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        // Post non-blocking receives
        MPI_Irecv(&recv_vals[0], 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&recv_vals[1], 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &requests[1]);

        int completed = 0;

        printf("Master waiting for results using Waitany...\n");

        while (completed < 2)
        {
            int index;
            MPI_Waitany(2, requests, &index, &status);

            printf("Master received result from worker %d: %d\n",
                   index + 1, recv_vals[index]);

            completed++;
        }
    }

    else
    {
        int value, result;

        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sleep(rank); // Worker 1 sleeps 1 sec, Worker 2 sleeps 2 sec

        result = value * rank;
        MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

/*

mpicc wait_any.c -o wait_any
mpiexec -n 3 wait_any

Master waiting for results using Waitany...
Master received result from worker 1: 10
Master received result from worker 2: 20

*/
