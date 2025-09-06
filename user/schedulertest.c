#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
cpu_workload(int process_id, int work_amount)
{
  int start_time = uptime();
  volatile int dummy = 0;
  
  printf("Process %d starting at time %d\n", process_id, start_time);
  
  // CPU intensive work
  for(int i = 0; i < work_amount; i++) {
    dummy += i * i;
  }
  dummy = dummy + 1; // Use dummy to prevent optimization
  
  int end_time = uptime();
  int running_time = end_time - start_time;
  
  printf("Process %d finished at time %d (running time: %d ticks)\n", 
         process_id, end_time, running_time);
  
  exit(running_time); // Return running time as exit status
}

int
main(int argc, char *argv[])
{
  printf("=== Scheduler Performance Test ===\n");
  printf("Creating 3 CPU-intensive processes\n");
  
  int start_time = uptime();
  int pid1, pid2, pid3;
  int status1, status2, status3;
  
  // Create three processes with different workloads
  if((pid1 = fork()) == 0) {
    cpu_workload(1, 100000000);
  }
  
  // Small delay to ensure different creation times for FCFS
  for(int i = 0; i < 1000000; i++);
  
  if((pid2 = fork()) == 0) {
    cpu_workload(2, 100000000);
  }
  
  // Small delay to ensure different creation times for FCFS
  for(int i = 0; i < 1000000; i++);
  
  if((pid3 = fork()) == 0) {
    cpu_workload(3, 100000000);
  }
  
  // Parent waits for all children and collects running times
  wait(&status1);
  wait(&status2); 
  wait(&status3);
  
  int total_time = uptime() - start_time;
  
  printf("=== Performance Results ===\n");
  printf("Process 1 running time: %d ticks\n", status1);
  printf("Process 2 running time: %d ticks\n", status2);
  printf("Process 3 running time: %d ticks\n", status3);
  printf("Total completion time: %d ticks\n", total_time);
  
  // Calculate average times
  int avg_running = (status1 + status2 + status3) / 3;
  printf("Average running time: %d ticks\n", avg_running);
  
  // Waiting time = total_time - running_time for each process
  int waiting1 = total_time - status1;
  int waiting2 = total_time - status2;
  int waiting3 = total_time - status3;
  int avg_waiting = (waiting1 + waiting2 + waiting3) / 3;
  
  printf("Average waiting time: %d ticks\n", avg_waiting);
  printf("=== Test completed ===\n");
  
  exit(0);
}