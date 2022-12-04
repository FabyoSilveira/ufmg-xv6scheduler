#include "types.h"
#include "user.h"

#define CPU_BOUND 0
#define S_CPU 1
#define IO_BOUND 2

int
main(int argc, char *argv[])
{
	//Get n count from command line
	int n;
	n = atoi(argv[1]);

	int i;
	int j;
	int k;
	int pid;
	int retime;
	int rutime;
	int stime;
	int procStats[3][3];
  int flagSuccessGetChildInfo;
	int countCPU = 0;
	int countCPUS = 0;
	int countIO = 0;

	//Initialize proc stats structure before running the program
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			procStats[i][j] = 0;

	for (i = 0; i < 3 * n; i++) {
		j = i % 3;
		pid = fork();

		if (pid == 0) {//child
			j = getpid() % 3; // ensures independence from the first son's pid when gathering the results in the second part of the program
			switch(j) {
				case CPU_BOUND:
					#ifdef LOTERY
					set_tickets(100);
					#endif
					for (k = 0; k < 100; k++){
						for (j = 0; j < 10000000; j++){}
					}

          wait2(&retime, &rutime, &stime);
          printf(1, "PID: %d - CPU-bound: Tempo de espera - %d, Tempo executando: %d, Tempo de IO: %d\n", getpid(), retime, rutime, stime);

					break;
				case S_CPU:
					#ifdef LOTERY
					set_tickets(100);
					#endif
					for (k = 0; k < 100; k++){
						for (j = 0; j < 1000000; j++){}
						yield();
					}

          wait2(&retime, &rutime, &stime);
					printf(1, "PID: %d - CPU-S bound,: Tempo de espera - %d, Tempo executando: %d, Tempo de IO: %d\n", getpid(), retime, rutime, stime);

					break;
				case IO_BOUND:
					#ifdef LOTERY
					set_tickets(100);
					#endif
					for(k = 0; k < 100; k++){
						sleep(1);
					}

          wait2(&retime, &rutime, &stime);
					printf(1, "PID: %d - I/O bound: Tempo de espera - %d, Tempo executando: %d, Tempo de IO: %d\n", getpid(), retime, rutime, stime);

					break;
			}
			exit();
		}
		continue;
	}

	for (i = 0; i < 3 * n; i++) {
		flagSuccessGetChildInfo = getZombieChildsInfo(&retime, &rutime, &stime, &pid);

    if(flagSuccessGetChildInfo == 0){
      int res = pid % 3;

      switch(res) {
        case CPU_BOUND:
					countCPU++;
          procStats[0][0] += retime;
          procStats[0][1] += rutime;
          procStats[0][2] += stime;
          break;
        case S_CPU:
					countCPUS++;
          procStats[1][0] += retime;
          procStats[1][1] += rutime;
          procStats[1][2] += stime;
          break;
        case IO_BOUND:    
          countIO++;
					procStats[2][0] += retime;
          procStats[2][1] += rutime;
          procStats[2][2] += stime;
          break;
      }
    }	
	}
	//Get average stats for each proc type
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++){
			switch(i){
				case CPU_BOUND:
					procStats[i][j] /= countCPU;
					break;
				case S_CPU:
					procStats[i][j] /= countCPUS;
					break;	
				case IO_BOUND:
					procStats[i][j] /= countIO;
					break;
			}	
		}
			

	//CPU STATS
	printf(1, "\n\nCPU bound:\nTempo médio ready: %d\nTempo médio running: %d\nTempo médio sleeping: %d\nTempo médio para completar: %d\n", procStats[0][0], procStats[0][1], procStats[0][2], procStats[0][0] + procStats[0][1] + procStats[0][2]);
	//CPU-S STATS
	printf(1, "\nCPU-S bound:\nTempo médio ready: %d\nTempo médio running: %d\nTempo médio sleeping: %d\nTempo médio para completar: %d\n", procStats[1][0], procStats[1][1], procStats[1][2], procStats[1][0] + procStats[1][1] + procStats[1][2]);
	//I/O STATS
	printf(1, "\nI/O bound:\nTempo médio ready: %d\nTempo médio running: %d\nTempo médio sleeping: %d\nTempo médio para completar: %d\n\n", procStats[2][0], procStats[2][1], procStats[2][2], procStats[2][0] + procStats[2][1] + procStats[2][2]);
	exit();
}
