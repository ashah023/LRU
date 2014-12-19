#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>

struct sys_memstats{
	unsigned long nr_active;
	unsigned long nr_inactive;
	unsigned long nr_SRB_active;
	unsigned long nr_SRB_inactive;
	unsigned long nr_swt_inactive;
	unsigned long nr_evicted;
	int clock;
};

