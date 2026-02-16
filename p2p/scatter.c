#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int nb_values = 8, rank, nb_procs, block_length, i;
  float *values, *recvdata;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  block_length = nb_values / nb_procs;
  recvdata = (float *)malloc(block_length * sizeof(float));


  if (rank == 2) {
    values = (float *)malloc(nb_values * sizeof(float));

    for (i = 0; i < nb_values; i++)
      values[i] = 1001. + i;

    printf("I, process %d send my values array : ", rank);

    for (i = 0; i < nb_values; i++) {
      printf("%f ", values[i]);
    }
    printf("\n");
  }

  MPI_Scatter(values, block_length, MPI_FLOAT, recvdata, block_length,
              MPI_FLOAT, 2, MPI_COMM_WORLD);

  printf("I, process %d, received ", rank);

  for (i = 0; i < block_length; i++)
    printf("%f ", recvdata[i]);

  printf("of process 2\n");
  MPI_Finalize();
}

/*

$ mpiexec -n 7 scatter

I, process 2 send my values array : 1001.000000 1002.000000 1003.000000 1004.000000 1005.000000 1006.000000 1007.000000 1008.000000 
I, process 2, received 1003.000000 of process 2
I, process 4, received 1005.000000 of process 2
I, process 3, received 1004.000000 of process 2
I, process 5, received 1006.000000 of process 2
I, process 6, received 1007.000000 of process 2
I, process 1, received 1002.000000 of process 2
I, process 0, received 1001.000000 of process 2

*/