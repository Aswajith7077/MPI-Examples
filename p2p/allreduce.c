#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int rank, nb_procs, value, product, i;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    value = 10;
  else
    value = rank;

  MPI_Allreduce(&value, &product, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

  printf("I, process %d, received the value of the global product %d\n", rank, product);

  MPI_Finalize();
}

/*

$  mpiexec -n 6 ./allreduce

I, process 3, received the value of the global product 1200
I, process 2, received the value of the global product 1200
I, process 5, received the value of the global product 1200
I, process 0, received the value of the global product 1200
I, process 4, received the value of the global product 1200
I, process 1, received the value of the global product 1200

*/
