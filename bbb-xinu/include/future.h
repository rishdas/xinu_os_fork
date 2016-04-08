#ifndef _FUTURE_H_
#define _FUTURE_H_
 
#define	NFUTURES	  100	/* number of semaphores*/
/* define states */
#define FUTURE_UNUSED    -1
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1
#define FUTURE_SHARED     2
#define FUTURE_QUEUE      3	
#define LINE_LEN         255

typedef struct futent
{
    int   value;
    char  str[LINE_LEN];
    int   flag;
    int   state;
    pid32 pid;
    int32 set_count;
    int32 get_count;
    qid16 set_queue;
    qid16 get_queue;
} future;

extern struct futent futtab[];

/* Interface for system call */
fut32 future_alloc(int future_flags);
syscall future_free(fut32 fut);
syscall future_get(fut32, int*);
syscall future_set(fut32, int*);
syscall future_get_str(fut32, int*, char*);
syscall future_set_str(fut32, int*, char*);

#define	isbadfut(f)	((int32)(f) < 0 || (f) >= NFUTURES) 
#endif /* _FUTURE_H_ */
