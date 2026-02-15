#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {

  int rank, nb_procs, value, sum, i;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    value = 1000;
  else
    value = rank;

  MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    printf("I, process 0, have the global sum value %d\n", sum);

  MPI_Finalize();
}
