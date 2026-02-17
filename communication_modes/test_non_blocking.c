#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // for sleep()

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
    int flag = 0;

    if (rank == 0)
    {
        int data = 5;
        int result;

        // Send data normally
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Post non-blocking receive
        MPI_Irecv(&result, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

        printf("Master doing heavy computation...\n");

        for (int i = 0; i < 10; i++)
        {
            sleep(1); // simulate work
            MPI_Test(&request, &flag, &status);

            if (flag)
            {
                printf("Master received result early: %d\n", result);
                break;
            }
            else
            {
                printf("Master still computing...\n");
            }
        }

        if (!flag)
        {
            MPI_Wait(&request, &status);
            printf("Master finally received result: %d\n", result);
        }
    }

    else if (rank == 1)
    {
        int data, result;

        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        sleep(3); // simulate processing delay
        result = data * 10;

        MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

/*

$ mpicc test_non_blocking.c -o test_non_blocking
$ mpiexec -n 2 test_non_blocking 

Master doing heavy computation...
Master still computing...
Master still computing...
Master received result early: 50

*/
