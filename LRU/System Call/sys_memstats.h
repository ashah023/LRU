#include <linux/linkage.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// Includes for gathering system memory statistics
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/mmzone.h>
#include <linux/mm.h>
#include <linux/page-flags.h>
#include <linux/list.h>

struct sys_memstats{
	unsigned long nr_active;
	unsigned long nr_inactive;
	unsigned long nr_SRB_active;
	unsigned long nr_SRB_inactive;
	unsigned long nr_swt_inactive;
	unsigned long nr_evicted;
	int clock;
};

