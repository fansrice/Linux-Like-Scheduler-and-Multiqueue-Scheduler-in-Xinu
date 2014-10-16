#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab1.h>
int findMaxGoodnessProcess()
{
	char test;
	int i, max = -1;
	for(i=1; i<NPROC; i++)
	{
		if(proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY)
		{
			calculateGoodness(i);
			if(goodness[i] > goodness[max])
				max = i;
		}
	}
	return max;
}
int findMultiMaxGoodnessProcess()
{
	int i, max = 0;
	for(i=0; i<NPROC; i++)
	{
		if((proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY)&&(proctab[i].queueType==2))
		{
			calculateGoodness(i);
			if(goodness[i] > goodness[max])
				max = i;
		}
	}
	return max;
}