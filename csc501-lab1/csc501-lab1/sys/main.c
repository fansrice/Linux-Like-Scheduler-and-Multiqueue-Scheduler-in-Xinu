#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <sched.h>
#include <stdio.h>
#include <math.h>
#include <lab1.h>

#define LOOP	50

int prA, prB, prC, prD, prE, prF;
int proc(char c);

int main() {
	int i;
	int count = 0;
	char buf[8];

    for(i=0;i<10;i++){
    	srand(i);
    	printf("%d\n", rand() );
    }

    /* Linux like Scheduler */  
   	setschedclass(MULTIQSCHED);
	resume(prA = createReal(proc, 2000, 5, "proc A", 1, 'A'));
	resume(prB = createReal(proc, 2000, 50, "proc B", 1, 'B'));
	resume(prC = create(proc, 2000, 90, "proc C", 1, 'C'));
	while (count++ < LOOP) {
		kprintf("M");

		for (i = 0; i < 10000000; i++);
	}

}

proc(char c) {
	int i;
	int count = 0;

	while (count++ < LOOP) {
		kprintf("%c", c);
		for (i = 0; i < 10000000; i++);
	}

}