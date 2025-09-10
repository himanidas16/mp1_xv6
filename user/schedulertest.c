#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NFORK 10
#define IO 5

int main() {
  int n, pid;
  int start_time = uptime();
  
  printf("=== Scheduler Performance Test ===\n");
  printf("Creating %d processes (%d I/O-bound, %d CPU-bound)\n", NFORK, IO, NFORK-IO);
  
  for (n = 0; n < NFORK; n++) {
    pid = fork();
    if (pid < 0) {
      printf("Fork failed for process %d\n", n);
      break;
    }
    if (pid == 0) {
      // Child process
      int child_start = uptime();
      printf("Process %d starting at time %d (type: %s)\n", 
             n, child_start, (n < IO) ? "I/O-bound" : "CPU-bound");
             
      if (n < IO) {
        // I/O-bound process - wait for 200 ticks using uptime
        int wait_start = uptime();
        while(uptime() - wait_start < 200) {
          // Simulate I/O wait - busy wait for 200 ticks
        }
      } else {
        // CPU-bound process - intensive computation  
        for (volatile int i = 0; i < 1000000000; i++) {}
      }
      
      int child_end = uptime();
      printf("Process %d finished at time %d (duration: %d ticks)\n", 
             n, child_end, child_end - child_start);
      exit(0);
    }
  }
  
  // Parent waits for all children
  printf("Parent waiting for all processes to complete...\n");
  for (n = 0; n < NFORK; n++) {
    wait(0);
  }
  
  int total_time = uptime() - start_time;
  printf("=== All processes completed in %d ticks ===\n", total_time);
  
  exit(0);
}

