#include <mpi.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {

    int rank, num_ranks;
    int b_value;              // value to broadcast
    int counter = 1;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


    MPI_Barrier(MPI_COMM_WORLD);

    // Only rank 0 starts with data
    if (rank == 0) {
        b_value = 100;
        printf("Process %d starts with value %d\n", rank, b_value);
    }

    while (counter < num_ranks) {

        if (rank < counter) {
            // Sender
            if (rank + counter < num_ranks) {
                MPI_Send(&b_value, 1, MPI_INT,
                         rank + counter, 0, MPI_COMM_WORLD);
            }
        }
        else if (rank < 2 * counter) {
            // Receiver
            MPI_Recv(&b_value, 1, MPI_INT,
                     rank - counter, 0,
                     MPI_COMM_WORLD, &status);

            printf("Process %d received %d from process %d\n",
                   rank, b_value, status.MPI_SOURCE);
        }

        counter *= 2;
    }

    printf("Final value at process %d = %d\n", rank, b_value);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    
    return 0;
}


/*

$ mpiexec -n 7 ./recursive-doubling

Process 0 starts with value 100
Process 1 received 100 from process 0
Final value at process 1 = 100
Final value at process 0 = 100
Process 2 received 100 from process 0
Final value at process 2 = 100
Process 4 received 100 from process 0
Final value at process 4 = 100
Process 3 received 100 from process 1
Final value at process 3 = 100
Process 6 received 100 from process 2
Final value at process 6 = 100
Process 5 received 100 from process 1
Final value at process 5 = 100

*/