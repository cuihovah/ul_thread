#ifndef UL_THREAD
#define UL_THREAD

#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <ucontext.h>  

#define NR_TASK 4		// The maximum of number of tasks
#define KB 1024			// 1KB = 1024B

/* This is my thread control black */
struct ul_task_struct{
	int tid;				
	char stack[128*KB];			// context's stack size = 128K
	ucontext_t cui_context;		
};

extern struct ul_task_struct *g_current;
extern struct ul_task_struct *g_tasks[NR_TASK+1];  
extern volatile int g_nr_tasks;

/****************************************************** 
 * The following two functions are auxiliary function * 
 * find_current_tid: get the tid of current task      * 
 * find_next_tid: get the tid of next task            * 
 ******************************************************/
extern int find_current_tid();
extern int find_next_tid();

extern void switch_context(int sig);

// Equivalent pthread_create
extern void create_cuithread(void (*func)(void*));
// I think it might be better to use a macro
extern void init_ulthread();

#endif
