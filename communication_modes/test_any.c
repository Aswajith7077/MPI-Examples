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

        MPI_Send(&send_val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&send_val, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        MPI_Irecv(&recv_vals[0], 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&recv_vals[1], 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &requests[1]);

        int completed = 0;

        printf("Master checking results using Testany...\n");

        while (completed < 2)
        {
            int index, flag;

            MPI_Testany(2, requests, &index, &flag, &status);

            if (flag)
            {
                printf("Master received result from worker %d: %d\n",
                       index + 1, recv_vals[index]);

                completed++;
            }
            else
            {
                printf("Master doing other work...\n");
                sleep(1); // simulate computation
            }
        }
    }

    else
    {
        int value, result;
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        sleep(rank); // Different completion times
        result = value * rank;
        MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

/*

mpicc test_any.c -o test_any
mpiexec -n 3 test_any

Master checking results using Testany...
Master doing other work...
Master doing other work...
Master doing other work...
Master received result from worker 1: 10
Master received result from worker 2: 20

*/
