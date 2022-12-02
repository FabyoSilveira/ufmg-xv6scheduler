#include "types.h"
#include "user.h"

#define CPU_BOUND 0
#define S_CPU 1
#define IO_BOUND 2

int
main(int argc, char *argv[])
{
	if (argc != 2){
    printf(1, "Usage: sanity <n>\n");
    exit();
  }

	int i;
	int n;
	int j = 0;
	int k;
	int retime;
	int rutime;
	int stime;
	int sums[3][3];
  int flagSuccessGetChildInfo;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			sums[i][j] = 0;

	n = atoi(argv[1]);
	i = n; //unimportant

	int pid;

	for (i = 0; i < 3 * n; i++) {
		j = i % 3;
		pid = fork();

		if (pid == 0) {//child
			j = (getpid() - 4) % 3; // ensures independence from the first son's pid when gathering the results in the second part of the program
			switch(j) {
				case CPU_BOUND: //CPU‐bound process (CPU):
					for (k = 0; k < 100; k++){
						for (j = 0; j < 1000000; j++){}
					}

          wait2(&retime, &rutime, &stime);
          printf(1, "CPU-bound, pid: %d, ready: %d, running: %d, sleeping: %d, turnaround: %d\n", getpid(), retime, rutime, stime, retime + rutime + stime);

					break;
				case S_CPU: //short tasks based CPU‐bound process (S‐CPU):
					for (k = 0; k < 100; k++){
						for (j = 0; j < 1000000; j++){}
						yield();
					}

          wait2(&retime, &rutime, &stime);
          printf(1, "CPU-S bound, pid: %d, ready: %d, running: %d, sleeping: %d, turnaround: %d\n", getpid(), retime, rutime, stime, retime + rutime + stime);

					break;
				case IO_BOUND:// simulate I/O bound process (IO)
					for(k = 0; k < 100; k++){
						sleep(1);
					}

          wait2(&retime, &rutime, &stime);
          printf(1, "I/O bound, pid: %d, ready: %d, running: %d, sleeping: %d, turnaround: %d\n", getpid(), retime, rutime, stime, retime + rutime + stime);

					break;
			}
			exit(); // children exit here
		}
		continue; // father continues to spawn the next child
	}

	for (i = 0; i < 3 * n; i++) {
		flagSuccessGetChildInfo = getZombieChildsInfo(&retime, &rutime, &stime, &pid);

    if(flagSuccessGetChildInfo == 0){
      int res = (pid - 4) % 3; // correlates to j in the dispatching loop

      switch(res) {
        case CPU_BOUND: // CPU bound processes
          sums[0][0] += retime;
          sums[0][1] += rutime;
          sums[0][2] += stime;
          break;
        case S_CPU: // CPU bound processes, short tasks
          sums[1][0] += retime;
          sums[1][1] += rutime;
          sums[1][2] += stime;
          break;
        case IO_BOUND: // simulating I/O bound processes       
          sums[2][0] += retime;
          sums[2][1] += rutime;
          sums[2][2] += stime;
          break;
      }
    }	
	}

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			sums[i][j] /= n;

	printf(1, "\n\nCPU bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[0][0], sums[0][1], sums[0][2], sums[0][0] + sums[0][1] + sums[0][2]);
	printf(1, "CPU-S bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[1][0], sums[1][1], sums[1][2], sums[1][0] + sums[1][1] + sums[1][2]);
	printf(1, "I/O bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[2][0], sums[2][1], sums[2][2], sums[2][0] + sums[2][1] + sums[2][2]);
	
  exit();
}
