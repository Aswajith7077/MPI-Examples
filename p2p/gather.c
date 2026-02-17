#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int nb_values = 8, rank, nb_procs, block_length, i;
  float recvdata[nb_values], *values;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  block_length = nb_values / nb_procs;

  values = (float *)malloc(block_length * sizeof(float));
  for (i = 0; i < block_length; i++)
    values[i] = 1001. + rank * block_length + i;

  printf("I, process %d sent my values array : ", rank);
  for (i = 0; i < block_length; i++)
  {
    printf("%f ", values[i]);
  }

  printf("\n");
  MPI_Gather(values, block_length, MPI_FLOAT, recvdata, block_length, MPI_FLOAT, 2, MPI_COMM_WORLD);

  if (rank == 2)
  {
    printf("I, process %d, received ", rank);
    for (i = 0; i < nb_values; i++)
    {
      printf("%f ", recvdata[i]);
    }
    printf("\n");
  }
  MPI_Finalize();
}

/*
$ mpiexec -n 6 ./gather

I, process 3 sent my values array : 1004.000000
I, process 5 sent my values array : 1006.000000
I, process 2 sent my values array : 1003.000000
I, process 4 sent my values array : 1005.000000
I, process 1 sent my values array : 1002.000000
I, process 0 sent my values array : 1001.000000
I, process 2, received 1001.000000 1002.000000 1003.000000 1004.000000 1005.000000 1006.000000 -0.000000 0.000000

*/