#include <assert.h>
#include <errno.h>
// #include <mpi/mpi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Prepare settings for each emulator
uint64_t *MakeSettings(int num_elements) {
  uint64_t *data = (uint64_t *)malloc(sizeof(uint64_t) * num_elements);
  assert(data != NULL);
  return data;
}

void RunSimulation(int tag) {
    printf("Running simulation on tag: %d\n", tag);
}

int main(int argc, char **argv) {
  int num_elements_per_proc = 1;

  // Each MPI call returns an error code that should be checked
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Prepare settings for each proc
  uint64_t *settings = NULL;
  if (world_rank == 0) {
    settings = MakeSettings(num_elements_per_proc * world_size);
  }

  // For each process, create a buffer that will hold configurations for every proc
  uint64_t *tag_subset =
      (uint64_t *)malloc(sizeof(uint64_t) * num_elements_per_proc);
  assert(tag_subset != NULL);

  MPI_Scatter(settings, num_elements_per_proc, MPI_UINT64_T, tag_subset,
              num_elements_per_proc, MPI_UINT64_T, 0, MPI_COMM_WORLD);

  // Wait for all simulations to be ready to run
  MPI_Barrier(MPI_COMM_WORLD);
  
  // Run simulation
  RunSimulation(world_rank);

  // Gather simulation results
  uint64_t *results = NULL;
  if (world_rank == 0) {
    results = (uint64_t *)malloc(sizeof(uint64_t) * world_size);
    assert(results != NULL);
  }

  MPI_Gather(&results, 1, MPI_UINT64_T, results, 1, MPI_UINT64_T, 0,
             MPI_COMM_WORLD);

  // Clean up
  if (world_rank == 0) {
    free(results);
    free(settings);
  }
  free(tag_subset);

  // Wait for all procs to synchronise
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}