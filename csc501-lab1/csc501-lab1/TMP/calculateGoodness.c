#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>
void calculateGoodness(int i)
{
	goodness[i] = -1;
	if(proctab[i].timeQuantum <= 0)
		goodness[i] = 0;
	else
		goodness[i] = proctab[i].timeQuantum + proctab[i].pprio;
}