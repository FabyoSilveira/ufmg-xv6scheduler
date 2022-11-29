#include "types.h"
#include "stat.h"
#include "user.h"

#define CPU_BOUND 0
#define S_CPU 1
#define IO_BOUND 2

struct procMetrics{
  int totalSleepingTime;
  int totalReadyTime;
  int totalTurnaroundTime;
  int qtdProc;
} procMetrics_constructor = {0, 0, 0, 0};

typedef struct procMetrics procMetrics;

int main(void) 
{ 
  procMetrics CPU_bound_procs = procMetrics_constructor;
  procMetrics S_cpu_procs = procMetrics_constructor;
  procMetrics IO_bound_procs = procMetrics_constructor;

  int pid;
  int procType;
  int totalProcesses = 3*2;

  for(int createdProcesses = 0; createdProcesses < totalProcesses; createdProcesses++){
    //Create new process
    pid = fork();

    //Check that fork suceed
    if(pid < 0){
      printf(1, "\nFork has failed, killing program...\n");
      kill(pid);
      continue;
    }

    //Get proc type, specified at TP command
    procType = pid%3;

    switch(procType){
      case CPU_BOUND:
        for(int x = 0; x < 100; x++){
          for(int y = 0; y < 10000000; y++){
            continue;
          }
          continue;
        }
        CPU_bound_procs.qtdProc++;
        kill(pid);
        break;
      case S_CPU:
        for(int x = 0; x < 100; x++){
          for(int y = 0; y < 1000000; y++){
            continue;
          }
          //yield syscall comes here, needs to be created
        }
        S_cpu_procs.qtdProc++;
        kill(pid);
        break;
      case IO_BOUND:
        for(int x = 0; x < 100; x++){
          sleep(1);
        }
        IO_bound_procs.qtdProc++;
        kill(pid);      
        break;
    }
    wait();
  }

  printf(1, "\nTerminei, CPUqtd: %d - Sqtd: %d - IOqtd: %d\n", CPU_bound_procs.qtdProc, S_cpu_procs.qtdProc, IO_bound_procs.qtdProc);
  exit();
}