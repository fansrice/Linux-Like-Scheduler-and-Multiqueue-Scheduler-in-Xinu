#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <stdio.h>
#include <math.h>
#include <lab1.h>

#define LOOP 50

int prA, prB, prC;
volatile int a_cnt = 0;
volatile int b_cnt = 0;
volatile int c_cnt = 0;
volatile int main_cnt = 0;

int count=0;

void proc(char c) {
	int i;
	int count = 0;

	while (count++ < LOOP) {
		kprintf("%c", c);
		for (i = 0; i < 10000000; i++);
	}

}

void proc_a(char c)
{
	int i;

	while (1) {
		for (i = 0; i < 10000000; i++)
		; //wait for 10000 iterations before printing letters. keeps character prints low.  
		kprintf("%c", c);
		a_cnt++;
	}
}

void proc_b(char c)
{
	int i;

	while (1) {
		for (i = 0; i < 10000000; i++)
			; 
		kprintf("%c", c);
		b_cnt++;
	}
}

void proc_c(c)
	char c; 
{
	int i;

	while (1) {
		for (i = 0; i < 10000000; i++)
			;
		kprintf("%c", c);
		c_cnt++;
	}
}


int main() {
	int i;
	int count = 0;
	char buf[8];

	kprintf("\n\n########## Test Case1: linux-like scheduling:normal case\n");
	setschedclass(LINUXSCHED);
	resume(prA = create(proc, 2000, 5, "proc A", 1, 'A'));
	resume(prB = create(proc, 2000, 50, "proc B", 1, 'B'));
	resume(prC = create(proc, 2000, 90, "proc C", 1, 'C'));
	while (count++ < LOOP) {
		kprintf("M");

		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);
	kill(prC);

	//sleep(5) ; 	
	
	count = 0 ; 
	kprintf("\n\n########## Test Case2: linux-like scheduling:Equal priority case\n");
	resume(prA = create(proc, 2000, 20, "proc A", 1, 'A'));
	resume(prB = create(proc, 2000, 20, "proc B", 1, 'B'));
	resume(prC = create(proc, 2000, 20, "proc C", 1, 'C'));
	while (count++ < LOOP) {
		kprintf("M");
		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);
	kill(prC);
	
	//sleep(5) ; 
	count = 0 ; 

	#if 1 
	kprintf("\n\n################# Testcase3: MultiqueueScheduler.Equal Priority Realtime Process. \n\n"); 
	setschedclass(MULTIQSCHED);
	prA = createReal(proc_a, 2000, 30, "proc A", 1, 'A');
	prB = createReal(proc_b, 2000, 30, "proc B", 1, 'B');
	prC = createReal(proc_c, 2000, 30, "proc C", 1, 'C');
	resume(prA);
	resume(prB);
	resume(prC);
	while (count++ < LOOP) {
		kprintf("M");
		main_cnt++ ; 
		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);
	kill(prC);
	
 	
	double total_cnt;
	double a_percent, b_percent, c_percent;
	double main_percent ; 
	total_cnt = a_cnt + b_cnt + c_cnt;
	a_percent = (double) a_cnt / total_cnt * 100;
	b_percent = (double) b_cnt / total_cnt * 100;
	c_percent = (double) c_cnt / total_cnt * 100;
	main_percent = (double) main_cnt / total_cnt * 100;
	
	kprintf("\n\nTest RESULT: A = %d, B = %d, C = %d, Main = %d (%d : %d : %d : %d)\n", a_cnt,
			b_cnt, c_cnt, main_cnt, (int) a_percent, (int) b_percent, (int) c_percent, (int)main_percent);
/*make all counters zero*/	
	a_cnt = b_cnt = c_cnt = total_cnt = 0 ; 
	count = 0 ;
	main_cnt = 0; 
#endif 

	
//	sleep(5) ;
	kprintf("\n\n########## Test Case4, Multiqueuescheduler. 2 Real processes different priority.\n");
	//setschedclass(MULTIQSCHED);
	prA = createReal(proc_a, 2000, 30, "proc A", 1, 'A');
	prB = createReal(proc_b, 2000, 20, "proc B", 1, 'B');
	resume(prA);
	resume(prB);
	kprintf("\nPid of A = %d, Pid of B = %d\n", prA, prB);
	while (count++ < LOOP) {
		kprintf("M");
		main_cnt++  ; 
		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);
	
	
	total_cnt = a_cnt + b_cnt ;
	a_percent = (double) a_cnt / total_cnt * 100;
	b_percent = (double) b_cnt / total_cnt * 100;
	main_percent = (double) main_cnt / total_cnt * 100;
	
	kprintf("Test RESULT: A = %d, B = %d,Main=%d, (%d : %d : %d )\n", a_cnt,
			b_cnt,main_cnt, (int) a_percent, (int) b_percent, (int)main_percent) ; 
	count = 0 ; 
	a_cnt = b_cnt = c_cnt = total_cnt = 0 ;
	main_cnt = 0 ;  

	
//	sleep(5)	; 
	kprintf("\n\n########## Test Case5, Multiqueuescheduler. 1 Real processes 1 Normal Process equal priority.\n");
	prA = createReal(proc_a, 2000, 30, "proc A", 1, 'A');
	prB = create(proc_b, 2000, 30, "proc B", 1, 'B');
	resume(prA);
	resume(prB);
	while (count++ < LOOP) {
		kprintf("M");
		main_cnt++  ; 
		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);
 	
	total_cnt = a_cnt + b_cnt ;
	a_percent = (double) a_cnt / total_cnt * 100;
	b_percent = (double) b_cnt / total_cnt * 100;
	kprintf("Test RESULT: A = %d, B = %d, (%d : %d )\n", a_cnt,
			b_cnt,  (int) a_percent, (int) b_percent);
	count = 0 ; 
	a_cnt = b_cnt = c_cnt = total_cnt = 0 ; 
	main_cnt =0 ; 
	//sleep(5) ; 
	kprintf("\n\n########## Test Case6, Multiqueuescheduler. 1 Real processes 1 Normal Process different priority.\n");
	prA = createReal(proc_a, 2000, 30, "proc A", 1, 'A');
	prB = create(proc_b, 2000, 40, "proc B", 1, 'B');
	resume(prA);
	resume(prB);
	while (count++ < LOOP) {
		kprintf("M");
		main_cnt++; 
		for (i = 0; i < 10000000; i++);
	}
	kill(prA);
	kill(prB);

total_cnt = a_cnt + b_cnt ;
	a_percent = (double) a_cnt / total_cnt * 100;
	b_percent = (double) b_cnt / total_cnt * 100;
	kprintf("Test RESULT: A = %d, B = %d, (%d : %d )\n", a_cnt,
			b_cnt,  (int) a_percent, (int) b_percent);
	
	kprintf("\n\n\nAll Testcases completed\n\n\n");
}


