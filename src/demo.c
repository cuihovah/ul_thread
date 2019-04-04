/**********************************
* cuiyunfeng: cuihovah@gmail.com  *
* 2015-07-10                      *
***********************************/
#include <ul_thread.h>
#include <time.h>

/************************************************
ul_thread interface:

	void create_cuithread(void (*func)(void*));

	void init_ulthread();
************************************************/

inline void do_function(char * func_name, int times){
	int i = 1;
    time_t start, end;
    /* In order to prevent brush screen */
    while(i <= times){
        time(&start);
L1:
        sleep(1);
        time(&end);
        // I'm sorry, I don't wanna goto
        if(start == end)
            goto L1;
        printf("%s : %d\n", func_name, i++);
    }

}

void func1(void * arg){  
	do_function("func1", 1000);
}


void func2(void * arg){  
	do_function("func2", 200);
}

int main()  
{
	init_ulthread();
	create_cuithread(func1);
	create_cuithread(func2);

	while(1){
		switch_context(0);
	}
	
    return 0;  
}
