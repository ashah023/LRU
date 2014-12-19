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

#define __NR_sys_memstats 302

/*
Runs the system call (with the default second chance algorithm) and prints out memory statistics.
*/
void run_sys_call(){
	struct sys_memstats memstats;
	int status = 1;	
	status = syscall(__NR_sys_memstats, &memstats);	

	if(status == 0){
		printf("\n----------------------------------------------\n");
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

int main(void){
	int page = getpagesize();

	printf("\n(1) Memory statistics without allocations.\n");
	run_sys_call();
	
	printf("\n(2) Memory statistics with allocations (not referenced)\n");
	void * test1 = malloc(page*128000);
	sleep(1);
	run_sys_call();
	
	printf("\n(3) Memory statistics with allocations (referenced).\n");
	memset(test1, 1, page*128000);
	run_sys_call();
	
	printf("\n(4) Freed all allocations.\n");
	free(test1);
	run_sys_call();	

	return 0;
}

