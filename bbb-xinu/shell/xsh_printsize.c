#include <xinu.h>

void entry_point(void);

shellcmd xsh_printsize(int nargs, char *args[])

{//create process

resume(create(entry_point,1024,20,"proc1",0));
/*
 struct procent *ptr;
 char *addr;

 ptr = &proctab[currpid];
 addr = ptr->prstkptr;

 kprintf("%u\n",(int32)addr);

*/
return OK;
}

 

void entry_point(void)

{//print stack pointer of currently running process.

 struct procent *ptr;

 ptr = &proctab[currpid];

 printf("%u\n",ptr->prstkptr);

}
