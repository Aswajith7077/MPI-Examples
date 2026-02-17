#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // for sleep()

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
        int send1 = 10, send2 = 20;
        int recv1, recv2;

        MPI_Request requests[2];
        MPI_Status statuses[2];

        MPI_Send(&send1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&send2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        MPI_Irecv(&recv1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&recv2, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &requests[1]);

        int flag = 0;

        printf("Master doing work while waiting for all results...\n");

        while (!flag)
        {
            MPI_Testall(2, requests, &flag, statuses);

            if (!flag)
            {
                printf("Still waiting...\n");
                sleep(1); // simulate computation
            }
        }

        printf("Master received: %d and %d\n", recv1, recv2);
    }

    else
    {
        int value, result;

        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        sleep(rank); // simulate different completion times

        result = value * rank;

        MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
