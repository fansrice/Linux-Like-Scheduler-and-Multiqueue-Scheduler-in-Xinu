/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
#include <lab1.h>
#include <stdio.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	static int randomNumber = 0;
	int newEpoch, x;
	int queueType = -1;
	/* no switch needed if current process priority higher than next*/

	if(schedulerType == 0)
	{
	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
	}
	else if(schedulerType == 1)
	{
		int i, maxGoodnessProcess;
		newEpoch = 1;
		proctab[currpid].timeQuantum = preempt;
		for(i=1; i<NPROC; i++)
		{
			if(proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY)
			{
				calculateGoodness(i);
				if(goodness[i] > 0)
					newEpoch = 0;		
			}
		}
		if(newEpoch == 1)
		{
			for(i=1; i<NPROC; i++)
			{
				proctab[i].timeQuantum = floor(proctab[i].timeQuantum/2) + proctab[i].pprio;
			}
		}
		maxGoodnessProcess = findMaxGoodnessProcess();
		if (optr->pstate == PRCURR)
		{
            optr->pstate = PRREADY;
            insert(currpid,rdyhead,optr->pprio);
        }
		if(maxGoodnessProcess == currpid)
		{
			preempt = proctab[maxGoodnessProcess].timeQuantum;
			return OK;
		}
        else
		{
            optr = &proctab[currpid];
            nptr = &proctab[maxGoodnessProcess];
            if (optr->pstate == PRCURR)
				optr->pstate = PRREADY;
            currpid = maxGoodnessProcess;
			dequeue(currpid);
            nptr->pstate = PRCURR;
            nptr = &proctab[ currpid ];
			#ifdef  RTCLOCK
                preempt = nptr->timeQuantum;              // reset preemption counter
            #endif
            ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;
        }
	}
	else if(schedulerType == 2)
	{
		if(queueType == -1)
		{
			srand(randomNumber);
			randomNumber++;
			x = rand() % 10;
			if(x>=0 && x<=6)
				queueType = 1;
			else	
				queueType = 2;
		}
		int i, maxGoodnessProcess;
		if( isempty(realhead))
			queueType = 2;
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail) ==0) ) {
		return(OK);
	}
		
		newEpoch = 1;
		proctab[currpid].timeQuantum = preempt;
		for(i=1; i<NPROC; i++)
		{
			if((proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY))
			{
				if((queueType == 2)&&(proctab[i].queueType==2))
				{
					calculateGoodness(i);
					if(goodness[i] > 0)
						newEpoch = 0;		
				}
				else if((queueType == 1)&&(proctab[i].queueType == 1))
				{
					if(proctab[i].timeQuantum>0)
						newEpoch = 0;
				}
			}
		}
		if(newEpoch == 1)
		{
			//if(currpid != 0)
				//kprintf("new epoch");
			srand(randomNumber);
			randomNumber++;
			x = rand() % 10;
			if(x>=0 && x<=6)
				queueType = 1;
			else	
				queueType = 2;
	if( isempty(realhead))
			queueType = 2;
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail) ==0) ) {
		return(OK);
	}		for(i=1; i<NPROC; i++)
			{
				if(queueType == 2 && proctab[i].queueType == 2)
				{
					proctab[i].timeQuantum = floor(proctab[i].timeQuantum/2) + proctab[i].pprio;
				}
				else if(queueType == 1 && proctab[i].queueType == 1)
				{
					proctab[i].timeQuantum = floor(proctab[i].timeQuantum/2) + 100;
				}
			}			
		}
		if(queueType == 2)
			maxGoodnessProcess = findMultiMaxGoodnessProcess();
		else
			maxGoodnessProcess = getlast(realtail);
		if (optr->pstate == PRCURR)
		{
            optr->pstate = PRREADY;
			if(proctab[currpid].queueType == 2)
				insert(currpid,rdyhead,optr->pprio);
			else
				insert(currpid,realhead,optr->pprio);
        }
		if(maxGoodnessProcess == currpid)
		{
			preempt = proctab[maxGoodnessProcess].timeQuantum;
			return OK;
		}
        else
		{
            optr = &proctab[currpid];
            nptr = &proctab[maxGoodnessProcess];
            if (optr->pstate == PRCURR)
				optr->pstate = PRREADY;
            currpid = maxGoodnessProcess;
            nptr->pstate = PRCURR;
			if(queueType == 2)
				currpid = dequeue(maxGoodnessProcess);
            nptr = &proctab[ (currpid ) ];
			#ifdef  RTCLOCK
                preempt = nptr->timeQuantum;              // reset preemption counter
            #endif
            ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;
        }
		
	}

}