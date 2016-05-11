/*

Single Author info:
asarang Abhishek Sarangan
Group info:
asarang Abhishek Sarangan
ayawalk Anuja Chnadrashekhar Yawalkar
smirans Sahibi Miranshah

*/
#include "mysched.h"
#include<math.h>

#define ALARM 32
#define SIGUSR 8
#define BLOCKED 16

int INIT=0;
struct sigaction timer_action;
struct sigaction old_timer_action;
sigset_t mask, mask1;

void printR(mythread_queue_t *headp)
{

  mythread_queue_t old, q, head;

  head = *headp;

  if (head == NULL)
{write(1,"Q Empty\n",8);}
else
{
  q = head;
  do {
  	mythread_t th = (mythread_t)(q->item);
  	printf(("%ld->  "),(th->tid));

    q = q->next;
  } while (q != head);
  write(1,"\n",1);
}



}


static int internal_scheduler()
{
	//write(1,"In internal_scheduler\n",22);
	mythread_t th = mythread_self();
	th->state &=~ALARM;	//removing signals from state
	th->state &=~SIGUSR;
	//th->reschedule = 0;

	if(*mythread_readyq()!=NULL && mythread_inq(mythread_runq(), th))
	{
		//mythread_t head = mythread_deq_head(mythread_readyq());
		//if(th->attribute !=NULL && th->attribute) return 0;
		return 1;
	}

	return 0;
}

static void sig_handler(int sig)
{
	//write(1,"In sig_handler\n",15);
	//printf("%d,,,%d LENGTHHH\n",mythread_lenq(mythread_runq()),mythread_lenq(mythread_readyq()));
	//printRun();
	//printReady();
	mythread_t th = mythread_self();
	if(th == NULL) return;
	th->reschedule = 1;
	if(sig == SIGALRM)
	{
		th->state |= ALARM;
		if(mythread_tryenter_kernel() == TRUE)	//if enters kernel
		{
			mythread_leave_kernel();
		}

	}
	else if(sig == SIGUSR1)
	{
		th->state &=~ALARM;
		th->state |= SIGUSR;
		mythread_enter_kernel();	//using enter instead of try enter as no one will end up scheduling the thread in case try fails.
		mythread_leave_kernel();


	}

	


}

void mythread_init_sched()
{
	//write(1,"In init_sched\n",14);
	struct itimerval timer;
	struct timeval val_timer;

	memset(&timer_action, '\0', sizeof(timer_action));
	timer_action.sa_handler = sig_handler;
	timer_action.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &timer_action, &old_timer_action);
	sigaction(SIGUSR1, &timer_action, &old_timer_action);
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	if(INIT == 0)
	{
		//write(1,"INIT was zero here\n",19);
		INIT = 1;
		val_timer.tv_sec = 0;
		val_timer.tv_usec = 10000;	//1ms
		timer.it_interval = val_timer;
		timer.it_value = val_timer;
		setitimer(ITIMER_REAL, &timer, NULL);
	}
}

void mythread_exit_sched()
{
	sigaction(SIGALRM, &old_timer_action, &timer_action);
	sigaction(SIGUSR1, &old_timer_action, &timer_action);
	sigprocmask(SIG_SETMASK, &mask1, NULL);
}

void mythread_leave_kernel()
{

	mythread_t th = mythread_self();
	if(th->reschedule == 0)
		mythread_leave_kernel_nonpreemptive();
	else
	{
		while(th->reschedule == 1)
		{			
				//write(1,"Entered kernel\n",15);
				th->reschedule = 0;
				if(th->state &= ALARM)
				{
					mythread_queue_t ptr = *mythread_runq();
					mythread_queue_t head = ptr;
					while(ptr != NULL) // loop for all running threads
					{
						if(((mythread_t)((ptr)->item))->tid != th->tid) // don't send signal to self
						{
							syscall(SYS_tgkill, getpid(), ((mythread_t)((ptr)->item))->tid, SIGUSR1);
							//((mythread_attr_t)(((mythread_t)((ptr)->item))->attr)->attr)--;
						}
						ptr = ptr->next;
						if(ptr == head) break;
					} // end while
				}
				((mythread_attr_t)((th)->attr)).attr--;
				//printf("<Look here> Attr is %d</Look here>\n",((mythread_attr_t)((th)->attr)).attr);
				
				if(internal_scheduler() == TRUE)
				{
					

					if(((mythread_attr_t)((th)->attr)).attr > 0)
					{
						//write(1,"attr greater than zero\n",23);
						//printf("attr is %d\n",((mythread_attr_t)((th)->attr)).attr);

						mythread_leave_kernel_nonpreemptive();					
					}
					else	//time to preempt itself!!
					{
						//write(1,"Going to be preempted\n",22);
					(th->preemptions)++;
					((mythread_attr_t)((th)->attr)).attr = pow(2,th->preemptions); //include math.h

					th->state |= BLOCKED;	//CHECKKKK!!!! @shek1608
					mythread_queue_t * readyQ = mythread_readyq();
					mythread_block(readyQ, BLOCKED); // ******check!!!!
					th->state &=~BLOCKED;	//when it wakes again, block removal!

					}
				}
				else
				{
					//write(1,"internal scheduler returned false\n",34);
					mythread_leave_kernel_nonpreemptive();
				}

			while((th->reschedule == 1)&&(!(mythread_tryenter_kernel())));
	/*		{
				mythread_tryenter_kernel();
			}*/
		}// end while
	}	//end else
	//mythread_leave_kernel_nonpreemptive();	//No need for this. Everything seems taken care of
}