# MPI Programs Compilation and Execution Guide

This repository contains various MPI (Message Passing Interface) programs demonstrating point-to-point communication, collective communication, and broadcasting algorithms.

## Prerequisites

Ensure you have an MPI implementation installed (e.g., MPICH, OpenMPI, or MS-MPI on Windows).
You should have the `mpicc` compiler wrapper and `mpiexec` or `mpirun` loader available in your path.

1. `mpicc` - For C source files
2. `mpicxx` - For C++ source files

## Compilation

To compile an MPI program, use the `mpicc` command.
The general syntax is:
```bash
mpicc <source_file.c> -o <output_executable>
```

A Most simpler one

```bash
mpicc <source_file.c>
mpiexec -n <number_of_processes> ./a.out
```

For C++ here is the following

```bash
mpicxx <source_file.cpp>
mpiexec -n <number_of_processes> ./a.out
```




### Examples:

**Root Directory Files:**
```bash
mpicc point_to_point.c -o point_to_point
mpicc who_am_i.c -o who_am_i
mpicc wild_card_p2p.c -o wild_card_p2p
```

**P2P Collective Operations:**
```bash
mpicc p2p/broadcast.c -o broadcast
mpicc p2p/scatter.c -o scatter
mpicc p2p/gather.c -o gather
mpicc p2p/allgather.c -o allgather
mpicc p2p/reduce.c -o reduce
mpicc p2p/allreduce.c -o allreduce
```

**Broadcasting Algorithms:**
```bash
mpicc broadcasting_algorithms/linear-broadcast.c -o linear-broadcast
mpicc broadcasting_algorithms/recursive-doubling.c -o recursive-doubling
```

---

## Execution

To run the compiled executable, use `mpiexec` or `mpirun`. You need to specify the number of processes (ranks) to launch using the `-n` flag.

The general syntax is:
```bash
mpiexec -n <number_of_processes> ./<executable_name>
```
*Note: On Windows, you might execute directly like `mpiexec -n 4 program.exe` without `./`.*

### Examples:

**Run with 4 processes:**
```bash
mpiexec -n 4 ./who_am_i
```

**Run P2P Operations (e.g., Scatter with 4 processes):**
```bash
mpiexec -n 4 ./scatter
```

**Run Broadcasting Algorithms (e.g., Recursive Doubling with 8 processes):**
```bash
mpiexec -n 8 ./recursive-doubling
```

## Platform Specific Notes

### Windows (MS-MPI)
If you are using MS-MPI on Windows, the commands are the same, but the executable will have a `.exe` extension automatically.
```powershell
mpicc point_to_point.c -o point_to_point.exe
mpiexec -n 4 point_to_point.exe
```
