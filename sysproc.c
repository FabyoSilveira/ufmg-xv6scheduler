#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//Sets the tickets number for scheduler policy, to the current process
int
sys_set_tickets(void)
{
  int tickets;
  
  //Invalid tickets set call
  if(argint(0, &tickets) < 0)
    return -1;

  struct proc *curproc = myproc();
  curproc->tickets = tickets;

  //Check if process tickets are set correctly, if not return -1 as expected
  if(curproc->tickets != tickets)
    return -1;

  return 0;
}

int
sys_wait2(void)
{
  int retime;
  int rutime;
  int stime;

  struct proc *curproc = myproc();

  argint(0, &retime);
  argint(0, &rutime);
  argint(0, &stime);

  curproc->retime = retime;
  curproc->rutime = rutime;
  curproc->stime = stime;

  if(curproc->retime != retime){
    return -1;
  }

  if(curproc->rutime != rutime){
    return -1;
  }

  if(curproc->stime != stime){
    return -1;
  }

  return 0;
}