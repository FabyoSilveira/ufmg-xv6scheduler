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
  procMetrics *CPU_bound_procs = &procMetrics_constructor;
  procMetrics *S_cpu_procs = &procMetrics_constructor;
  procMetrics *IO_bound_procs = &procMetrics_constructor;
  
  int pid;
  int procType;
  int totalProcesses = 3;

  int retime;
  int rutime;
  int stime;

  int arrProcInfo[5];
  int arrParentReceiveInfo[5];

  //Create a pipe to allow child process, communicate with parent their infos after finish the execution
  int fd[2];
  pipe(fd);

  for(int createdProcesses = 0; createdProcesses < totalProcesses; createdProcesses++){
    //Create new process
    pid = fork();

    if(pid == 0){
      //Check that fork suceed
      if(pid < 0){
        printf(1, "\nFork has failed, killing process...\n");
        exit();
      }

      //Get proc type, specified at TP command
      procType = getpid()%3;

      //close read channel of pipe since child's only writes
      close(fd[0]);
      close(1);
      
      switch(procType){
        case CPU_BOUND:
          for(int x = 0; x < 100; x++){
            for(int y = 0; y < 10000000; y++){
              continue;
            }
            continue;
          }
          wait2(&retime, &rutime, &stime);

          //Info about, respectively, what proc type has executed and retime, rutime, stime and childPid
          arrProcInfo[0] = 0;
          arrProcInfo[1] = retime;
          arrProcInfo[2] = rutime;
          arrProcInfo[3] = stime;
          arrProcInfo[4] = getpid();

          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));
          break;

        case S_CPU:
          for(int x = 0; x < 100; x++){
            for(int y = 0; y < 1000000; y++){
              continue;
            }
            yield();
          }
          wait2(&retime, &rutime, &stime);

          //Info about, respectively, what proc type has executed and retime, rutime, stime and childPid
          arrProcInfo[0] = 1;
          arrProcInfo[1] = retime;
          arrProcInfo[2] = rutime;
          arrProcInfo[3] = stime;
          arrProcInfo[4] = getpid();
  
          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));
          break;

        case IO_BOUND:
          for(int x = 0; x < 100; x++){
            sleep(1);
          }
          wait2(&retime, &rutime, &stime);

          //Info about, respectively, what proc type has executed and retime, rutime, stime and childPid
          arrProcInfo[0] = 2;
          arrProcInfo[1] = retime;
          arrProcInfo[2] = rutime;
          arrProcInfo[3] = stime;
          arrProcInfo[4] = getpid();

          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));
          break;
      }
      exit();
    }else{
      //Important to execute father after child and pipe read/write works
      wait();

      //printf(1, "\nWait working or not%d %d %d %d\n", wait2(&retime, &rutime, &stime), retime, rutime, stime);
      
      //close write channel of pipe since parent's only reads
      close(0);
      close(fd[1]);     

      read(fd[0], arrParentReceiveInfo, sizeof(arrParentReceiveInfo));

      switch(arrParentReceiveInfo[0]){
        case CPU_BOUND:
          //Increment cpu bound counter
          CPU_bound_procs->qtdProc++;

          //Print proc info after finish
          printf(1, "\nPID-%d CPU-Bound / Tempo de espera - %d Tempo executando - %d Tempo de IO - %d\n", arrParentReceiveInfo[4], arrParentReceiveInfo[1], arrParentReceiveInfo[2], arrParentReceiveInfo[3]);
          break;

        case S_CPU:
          //Increment s bound counter
          S_cpu_procs->qtdProc++;

          //Print proc info after finish
          printf(1, "\nPID-%d S-Bound / Tempo de espera - %d Tempo executando - %d Tempo de IO - %d\n", arrParentReceiveInfo[4], arrParentReceiveInfo[1], arrParentReceiveInfo[2], arrParentReceiveInfo[3]);
          break;

        case IO_BOUND:
          //Increment io bound counter
          IO_bound_procs->qtdProc++;
          
          //Print proc info after finish
          printf(1, "\nPID-%d IO-Bound / Tempo de espera - %d Tempo executando - %d Tempo de IO - %d\n", arrParentReceiveInfo[4], arrParentReceiveInfo[1], arrParentReceiveInfo[2], arrParentReceiveInfo[3]);
          break;
      }
    }
    
  }

  //printf(1, "\nTerminei, CPUqtd: %d - Sqtd: %d - IOqtd: %d\n", CPU_bound_procs->qtdProc, S_cpu_procs->qtdProc, IO_bound_procs->qtdProc);
  exit();
}