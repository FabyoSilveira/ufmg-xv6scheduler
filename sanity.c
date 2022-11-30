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

  int arrProcInfo[4];
  int arrParentReceiveInfo[4];

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
          //Print proc info after finish
          printf(1, "\nId-%d CPU-Bound / Tempo de espera - executando - IO", getpid());

          //Info about, respectively, what proc type has executed and rutime, retime stime
          arrProcInfo[0] = 0;
          arrProcInfo[1] = 4;
          arrProcInfo[2] = 4;
          arrProcInfo[3] = 4;

          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));

          exit();
        break;

        case S_CPU:
          for(int x = 0; x < 100; x++){
            for(int y = 0; y < 1000000; y++){
              continue;
            }
            //yield syscall comes here, needs to be created
          }
          //Print proc info after finish
          printf(1, "\nId-%d S_Bound / Tempo de espera - executando - IO\n", getpid());

          //Info about, respectively, what proc type has executed and rutime, retime stime
          arrProcInfo[0] = 1;
          arrProcInfo[1] = 5;
          arrProcInfo[2] = 5;
          arrProcInfo[3] = 5;
  
          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));

          exit();
        break;

        case IO_BOUND:
          for(int x = 0; x < 100; x++){
            sleep(1);
          }
          //Print proc info after finish
          printf(1, "\nId-%d IO-Bound / Tempo de espera - executando - IO", getpid());

          //Info about, respectively, what proc type has executed and rutime, retime stime
          arrProcInfo[0] = 2;
          arrProcInfo[1] = 6;
          arrProcInfo[2] = 6;
          arrProcInfo[3] = 6;

          //Write info into fathers pipe
          dup(fd[1]);  
          write(1, arrProcInfo, sizeof(arrProcInfo));
          
          exit();
        break;

        default:
          printf(1, "Entrei default");
          exit();
      }
    }else{
      //Important to execute father after child and pipe read/write works
      wait();

      //close write channel of pipe since parent's only reads
      close(0);
      close(fd[1]);     

      read(fd[0], arrParentReceiveInfo, sizeof(arrParentReceiveInfo));

      printf(1, "\nPai executando: child procType executed -  %d\n", arrParentReceiveInfo[0]);
    }
    
  }

  printf(1, "\nTerminei, CPUqtd: %d - Sqtd: %d - IOqtd: %d\n", CPU_bound_procs->qtdProc, S_cpu_procs->qtdProc, IO_bound_procs->qtdProc);
  exit();
}