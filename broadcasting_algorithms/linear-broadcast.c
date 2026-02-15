#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  int rank, num_ranks;
  int b_value;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  // All Start at once
  MPI_Barrier(MPI_COMM_WORLD);

  // Only rank 0 initializes the value
  if (rank == 0) {
    b_value = 100;
    printf("Process %d starts with value %d\n", rank, b_value);

    if (num_ranks > 1) {
      MPI_Send(&b_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
  } else if (rank < num_ranks - 1) {

    MPI_Recv(&b_value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

    printf("Process %d received %d from process %d\n", rank, b_value,
           status.MPI_SOURCE);

    MPI_Send(&b_value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
  } else { // Last process

    MPI_Recv(&b_value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

    printf("Process %d received %d from process %d\n", rank, b_value,
           status.MPI_SOURCE);
  }

  printf("Final value at process %d = %d\n", rank, b_value);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  return 0;
}

/*

$ mpiexec -n 7 ./linear-broadcast

Process 0 starts with value 100
Final value at process 0 = 100
Process 2 received 100 from process 1
Final value at process 2 = 100
Process 1 received 100 from process 0
Final value at process 1 = 100
Process 3 received 100 from process 2
Final value at process 3 = 100
Process 4 received 100 from process 3
Final value at process 4 = 100
Process 5 received 100 from process 4
Final value at process 5 = 100
Process 6 received 100 from process 5
Final value at process 6 = 100

*/