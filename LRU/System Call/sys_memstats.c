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

#include <linux/sys_memstats.h>
#include <linux/part2.h>

#define foreach_zone(zone)							\
	for( zone = first_online_pgdat()->node_zones; zone;	\
	zone = next_zone(zone) )							\

#define list_type(zone ,cond)							\
	cond ? zone->active_list : zone->inactive_list	 	\


/*Returns the number of pages that had their reference bit set in a 
given zone and list type*/
unsigned long isPG_referenced( struct zone *zone, int cond ){
	unsigned long num_referenced = 0;
	struct list_head *tmp;	
	
	list_for_each( tmp, list_type(&zone, cond) ){
		struct page *page = list_entry(tmp, struct page, lru);
		
		if(PageReferenced(page)){ num_referenced++; }
	}	
	return num_referenced;
}

/* For all zones, calculates (1) the number of pages in the active list, 
(2) the number of pages in the inactive list, (3) the number of pages in 
the active list with their reference bit set, (4) the number of pages in 
the inactive list with their reference bit set, (5) the number of pages 
demoted from the active list to the inactive list, and (6) the number of 
pages evicted from the inactive list, whether due to a promotion or due 
to being freed. These values are all stored in the provided sys_memstats 
struct.*/
asmlinkage long sys_sys_memstats(struct sys_memstats *memstats){
	
        struct zone *zone;
     
	unsigned long nr_active = 0;		// pages in the active list
        unsigned long nr_inactive = 0;		// pages in the inactive list
        unsigned long nr_SRB_active = 0;        // active pages whose ref bit is set
        unsigned long nr_SRB_inactive = 0;      // inactive pages whose ref bit is set
        unsigned long nr_swt_inactive = 0;	// pages switched from active to inactive
        unsigned long nr_evicted = 0;		// pages evicted from the inactive list

	// if the user didn't provide stack or heap space then return -22
	if(&memstats == NULL){ return -22; }

	// if the user set the clock algorithm, set our global variable	
	if(memstats->clock == 1){ CLOCK_ALGO = 1; }
	else CLOCK_ALGO = 0; // otherwise use the regular second chance algorithm

        // traverse all the memory zones and add the zone data needed to our counters
	foreach_zone( zone ){
        	if(zone != NULL){
        		nr_active += zone->nr_active;
       			nr_inactive += zone->nr_inactive;
       			nr_SRB_active += isPG_referenced(zone, 1);
       			nr_SRB_inactive += isPG_referenced(zone, 0);
       			nr_swt_inactive += zone->nr_swt_inactive;
       			nr_evicted += zone->nr_evicted;
       		}
       	}

	// pass these values to the user's provided sys_memstats struct	
	memstats->nr_active = nr_active;
        memstats->nr_inactive = nr_inactive;
        memstats->nr_SRB_active = nr_SRB_active;    
        memstats->nr_SRB_inactive = nr_SRB_inactive; 
        memstats->nr_swt_inactive = nr_swt_inactive;
        memstats->nr_evicted = nr_evicted;

	return 0;
}

