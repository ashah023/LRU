/*
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2014
## Prof. Jose F. Osorio
## Group: Apsalar
## Group Members: Maylem Gonzalez – 2134900
##                Renan Santana – 4031451
## Project: Virtual Memory Management
## Specs:  
## Due Date: 07/25/2014 by 11:55pm
## Module Name: sys_memstats
##
## We Certify that this program code has been written by us
## and no part of it has been taken from any sources.
##########################################################
*/


#include "sys_memstats.h"
#include <time.h>
#define __NR_sys_memstats 302

/*
 * Function for printing the memory statistics.
 * @ algo - Used to set the global variable in order to activate the clock algorithm
*/
void run_sys_call(int algo){
	// The struct used to store the statistics
	struct sys_memstats memstats;
	// This member is used to set which algorithm to use.
	// 0 is for Second Chance
	// 1 is for Clock
	memstats.clock = algo;
	int status = 1;
	// Calls the static system call we created
	status = syscall(__NR_sys_memstats, &memstats);	
	
	// The set of prints we do to show all the statistics 
	// gathered.
	if(status == 0){
		printf("\n---------------------------------------------\n");
                printf("MEMORY STATISTICS\n");
                printf("-----------------------------------------------\n");
                printf("Pages in the active list: %lu\n", memstats.nr_active);
                printf("Pages in the inactive list: %lu\n", memstats.nr_inactive);
                printf("Pages in the active list whose reference bit is set: %lu\n", memstats.nr_SRB_active);
                printf("Pages in the inactive list whose reference bit is set: %lu\n", memstats.nr_SRB_inactive);
                printf("Pages moved from active to the inactive list: %lu\n", memstats.nr_swt_inactive);
                printf("Pages evicted from the inactive list: %lu\n", memstats.nr_evicted);
	}

}

int main(int argc, char *argv[]){		
	int i;
	void *test1, *test2, *test3, *test4;
	// Print the elapsed time for each call.
	clock_t t1, t2;

	// Print type (1)
	// This print only allocates memory and
	// uses the Second Chance Algo
	printf("\n (1) Second Chance Algorithm\n");
	printf("  Memory statistics with allocations (not referenced)\n");
        t1 = clock();
	for(i=0; i < 300; i++){
		
		test1 = malloc(1024*1024);
	}
	run_sys_call(0);
	t2 = clock();
	printf("Time Elapsed %.8f\n", (double)(t2-t1)/CLOCKS_PER_SEC);

	// Print type (2)
	// This print allocates memory, initializes it, 
	// and uses the Second Chance Algo
	printf("\n (2) Second Chance Algorithm\n");
        printf("  Memory statistics with allocations (referenced)\n");
        t1 = clock();
	for(i=0; i < 300; i++){
                test2 = malloc(1024*1024);
		// This set the memory space with ones.
        	memset(test2, 1, 1024*1024);
	}
        run_sys_call(0);
	t2 = clock();
	printf("Time Elapsed %.8f\n", (double)(t2-t1)/CLOCKS_PER_SEC);
	
	// Print type (3)
	// This print allocates memory and uses the Clock algorithm
	printf("\n (3) Clock Based Algorithm\n");
        printf("  Memory statistics with allocations (not referenced)\n");
        t1 = clock();
	for(i=0; i < 300; i++){
                test3 = malloc(1024*1024);
        }
        run_sys_call(1);
	t2 = clock();
	printf("Time Elapsed %.8f\n", (double)(t2-t1)/CLOCKS_PER_SEC);
	
	// Print type (4)
	// This print allocates memory, initializes the memory
	// and uses the Clock
        printf("\n (4) Clock Based Algorithm\n");
        printf("  Memory statistics with allocations (referenced)\n");
        t1 = clock();
	for(i=0; i < 300; i++){
                test4 = malloc(1024*1024);
		// This set the memory space with ones.
                memset(test4, 1, 1024*1024);
        }
        run_sys_call(1);
	t2 = clock();
	printf("Time Elapsed %.8f\n", (double)(t2-t1)/CLOCKS_PER_SEC);
	
	return 0;
}

