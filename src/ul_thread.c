#include "ul_thread.h"

struct ul_task_struct *g_current;
struct ul_task_struct *g_tasks[NR_TASK+1];  
volatile int g_nr_tasks = 0;

//Return Value: The tid of current task
inline int find_current_tid(){
	// Simple traversal g_tasks
	int i = 0;
	while(g_current->tid != g_tasks[i]->tid){
		i++;
	}
	return i;
}
//Return Value: The tid of next task
int find_next_tid(){
	int current_id = find_current_tid();
	return (current_id + 1)%(g_nr_tasks+1);
}

// Complete context switching functions
void switch_context(int sig){
	int _cnt = find_current_tid();
	int _nxt = find_next_tid();

	struct ul_task_struct * l_next = g_tasks[_nxt];
	g_current = g_tasks[_cnt];

	struct ul_task_struct * t_old = g_current;
	g_current = l_next;
	t_old->cui_context.uc_link = &(l_next->cui_context);

	swapcontext(&(t_old->cui_context), &(l_next->cui_context));
}

/************************************************
* Every time you call this function, you create *
* a new thread.                                 * 
* func: Callback function                       *
*************************************************/
void create_cuithread(void (*func)(void*)){
	struct ul_task_struct * cts = 
			(struct ul_task_struct*)malloc(sizeof(struct ul_task_struct));
	cts->tid = ++g_nr_tasks;
	getcontext(&(cts->cui_context));
	cts->cui_context.uc_stack.ss_sp = cts->stack;
    cts->cui_context.uc_stack.ss_size = sizeof(cts->stack);
    cts->cui_context.uc_stack.ss_flags = 0;
	makecontext(&(cts->cui_context), (void (*)(void))func, 0);
	g_tasks[g_nr_tasks] = cts;
}

/********************************************
* Initialization ul_thread
* set timer 0.1s signal(ITIMER_REAL:SIGALRM)
* set g_tasks[0] is main thread
**********************************************/
void init_ulthread(){
	struct itimerval l_tv = {{0, 1}, {0, 100000}};  
    g_tasks[0] = (struct ul_task_struct*)malloc(sizeof(struct ul_task_struct));
    g_current = g_tasks[0];
	setitimer(ITIMER_REAL, &l_tv, NULL);
	// In fact, I think ITIMER_VIRTUAL is more appropriate;
	// signal SIGVTALRM and swapcontext are not very good at 
	// running together.
	if(signal(SIGALRM, switch_context)==SIG_ERR)
		exit(1);
}

