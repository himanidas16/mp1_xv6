#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
cpu_intensive_process(int id)
{
  printf("CPU-intensive process %d starting\n", id);
  volatile int dummy = 0;
  
  // Very heavy workload to ensure processes get demoted and stay in lower queues
  for(int i = 0; i < 5000000; i++) {
    dummy += i * i;
    for(int j = 0; j < 50; j++) {
      dummy += j * i;
    }
    
    // Print progress every 1 million iterations
    if(i % 1000000 == 0) {
      printf("Process %d: iteration %d million\n", id, i/1000000);
    }
  }
  
  printf("CPU-intensive process %d finished\n", id);
  exit(0);
}

int
main(int argc, char *argv[])
{
  printf("=== Starvation Prevention Test ===\n");
  printf("Starting multiple CPU-intensive processes to trigger starvation prevention...\n");
  
  // Start 3 CPU-intensive processes
  for(int i = 1; i <= 3; i++) {
    if(fork() == 0) {
      cpu_intensive_process(i);
    }
  }
  
  // Wait for all children to complete
  for(int i = 0; i < 3; i++) {
    wait(0);
  }
  
  printf("=== Starvation test completed ===\n");
  exit(0);
}