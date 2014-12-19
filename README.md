LRU
===

Source: http://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/9_VirtualMemory.html

9.4.4 LRU Page Replacement

The prediction behind LRU, the Least Recently Used, algorithm is that the page that has not been used in the longest time is the one that will not be used again in the near future. ( Note the distinction between FIFO and LRU: The former looks at the oldest load time, and the latter looks at the oldest use time. )
Some view LRU as analogous to OPT, except looking backwards in time instead of forwards. ( OPT has the interesting property that for any reference string S and its reverse R, OPT will generate the same number of page faults for S and for R. It turns out that LRU has this same property. )
Figure 9.15 illustrates LRU for our sample string, yielding 12 page faults, ( as compared to 15 for FIFO and 9 for OPT. )
 

Figure 9.15 - LRU page-replacement algorithm.

LRU is considered a good replacement policy, and is often used. The problem is how exactly to implement it. There are two simple approaches commonly used:
Counters. Every memory access increments a counter, and the current value of this counter is stored in the page table entry for that page. Then finding the LRU page involves simple searching the table for the page with the smallest counter value. Note that overflowing of the counter must be considered.
Stack. Another approach is to use a stack, and whenever a page is accessed, pull that page from the middle of the stack and place it on the top. The LRU page will always be at the bottom of the stack. Because this requires removing objects from the middle of the stack, a doubly linked list is the recommended data structure.
Note that both implementations of LRU require hardware support, either for incrementing the counter or for managing the stack, as these operations must be performed for every memory access.
Neither LRU or OPT exhibit Belady's anomaly. Both belong to a class of page-replacement algorithms called stack algorithms, which can never exhibit Belady's anomaly. A stack algorithm is one in which the pages kept in memory for a frame set of size N will always be a subset of the pages kept for a frame size of N + 1. In the case of LRU, ( and particularly the stack implementation thereof ), the top N pages of the stack will be the same for all frame set sizes of N or anything larger.
 

Figure 9.16 - Use of a stack to record the most recent page references.

9.4.5 LRU-Approximation Page Replacement

Unfortunately full implementation of LRU requires hardware support, and few systems provide the full hardware support necessary.
However many systems offer some degree of HW support, enough to approximate LRU fairly well. ( In the absence of ANY hardware support, FIFO might be the best available choice. )
In particular, many systems provide a reference bit for every entry in a page table, which is set anytime that page is accessed. Initially all bits are set to zero, and they can also all be cleared at any time. One bit of precision is enough to distinguish pages that have been accessed since the last clear from those that have not, but does not provide any finer grain of detail.
9.4.5.1 Additional-Reference-Bits Algorithm

Finer grain is possible by storing the most recent 8 reference bits for each page in an 8-bit byte in the page table entry, which is interpreted as an unsigned int.
At periodic intervals ( clock interrupts ), the OS takes over, and right-shifts each of the reference bytes by one bit.
The high-order ( leftmost ) bit is then filled in with the current value of the reference bit, and the reference bits are cleared.
At any given time, the page with the smallest value for the reference byte is the LRU page.
Obviously the specific number of bits used and the frequency with which the reference byte is updated are adjustable, and are tuned to give the fastest performance on a given hardware platform.
9.4.5.2 Second-Chance Algorithm

The second chance algorithm is essentially a FIFO, except the reference bit is used to give pages a second chance at staying in the page table.
When a page must be replaced, the page table is scanned in a FIFO ( circular queue ) manner.
If a page is found with its reference bit not set, then that page is selected as the next victim.
If, however, the next page in the FIFO does have its reference bit set, then it is given a second chance:
The reference bit is cleared, and the FIFO search continues.
If some other page is found that did not have its reference bit set, then that page will be selected as the victim, and this page ( the one being given the second chance ) will be allowed to stay in the page table.
If , however, there are no other pages that do not have their reference bit set, then this page will be selected as the victim when the FIFO search circles back around to this page on the second pass.
If all reference bits in the table are set, then second chance degrades to FIFO, but also requires a complete search of the table for every page-replacement.
As long as there are some pages whose reference bits are not set, then any page referenced frequently enough gets to stay in the page table indefinitely.
This algorithm is also known as the clock algorithm, from the hands of the clock moving around the circular queue.
 

Figure 9.17 - Second-chance ( clock ) page-replacement algorithm.

9.4.5.3 Enhanced Second-Chance Algorithm

The enhanced second chance algorithm looks at the reference bit and the modify bit ( dirty bit ) as an ordered page, and classifies pages into one of four classes:
( 0, 0 ) - Neither recently used nor modified.
( 0, 1 ) - Not recently used, but modified.
( 1, 0 ) - Recently used, but clean.
( 1, 1 ) - Recently used and modified.
This algorithm searches the page table in a circular fashion ( in as many as four passes ), looking for the first page it can find in the lowest numbered category. I.e. it first makes a pass looking for a ( 0, 0 ), and then if it can't find one, it makes another pass looking for a ( 0, 1 ), etc.
The main difference between this algorithm and the previous one is the preference for replacing clean pages if possible.
9.4.6 Counting-Based Page Replacement

There are several algorithms based on counting the number of references that have been made to a given page, such as:
Least Frequently Used, LFU: Replace the page with the lowest reference count. A problem can occur if a page is used frequently initially and then not used any more, as the reference count remains high. A solution to this problem is to right-shift the counters periodically, yielding a time-decaying average reference count.
Most Frequently Used, MFU: Replace the page with the highest reference count. The logic behind this idea is that pages that have already been referenced a lot have been in the system a long time, and we are probably done with them, whereas pages referenced only a few times have only recently been loaded, and we still need them.
In general counting-based algorithms are not commonly used, as their implementation is expensive and they do not approximate OPT well.
