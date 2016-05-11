/*

Single Author info:
ayawalk Anuja Chandrashekhar Yawalkar
Group info:
asarang Abhishek Sarangan
ayawalk Anuja Chandrashekhar Yawalkar
smirans Sahibi Miranshah

*/
/*
README:

This test program will help to show how scheduling is done via 2^(n-1) ms where n is number of times it has been preempted.
There are loops printing the thread numbers and you will see that the individual loops run for set amounts of time,
which keep increasing in case of preemptions.

To run program, run the following commands:
make clean;
make
./mytest
*/
#include "mythread.h"
#include <stdio.h>
//#include <mythread.h>

void * func(void * arg)
{
	//printf("Reached thread func\n");
	write(1,"Onee\n",5);
	mythread_t th = mythread_self();
	mythread_attr_t th_attr = th->attr;
	printf("attr is %d\n", th_attr.attr);
	int i;
	for(i=1; i<=10; i++)
	{
		write(1,"One\n",4);
		//printf("%d\n", i);
		sleep(1);
	}
		//while(1);

	printf("....In thread function....\n");
}

void * func2(void * arg)
{
	write(1,"Twoo\n",5);
	mythread_t th = mythread_self();
	mythread_attr_t th_attr = th->attr;
	printf("attr is %d\n", th_attr.attr);
	int i;
/*	for(i=1; i<=10; i++)
	{
		write(1,"Two\n",4);
		//printf("%d\n", i);
		sleep(1);
	} 
*/			while(1);


	printf("....In thread function....\n");
}

void * func3(void * arg)
{
	//printf("Reached thread func3\n");
	write(1,"Tree\n",5);
	mythread_t th = mythread_self();
	mythread_attr_t th_attr = th->attr;
	printf("attr is %d\n", th_attr.attr);
	int i;
	/*for(i=1; i<=10; i++)
	{
		write(1,"Tre\n",4);
		//printf("%d\n", i);
		sleep(1);
	} */
		while(1);

	printf("....In thread function....\n");
}

void * fun1(void * arg)
{
	long double i,j;
	for(i=0; i<35;i++)
	{
		for(j=0; j<1000000; j++);
		write(1,"One\n",4);
	}
}
void * fun2(void * arg)
{
	long double i,j;
	for(i=0; i<35;i++)
	{
		for(j=0; j<1000000; j++);
		write(1,"Two\n",4);
	}
}
void * fun3(void * arg)
{
	long double i,j;
	for(i=0; i<35;i++)
	{
		for(j=0; j<1000000; j++);
		write(1,"Three\n",6);
	}
}
void * fun4(void * arg)
{
	long double i,j;
	for(i=0; i<35;i++)
	{
		for(j=0; j<1000000; j++);
		write(1,"Four\n",5);
	}
}


int main()
{
	mythread_setconcurrency(2);
	//printf("Tid of main is %d\n", mythread_self());


	//mythread_t mainz = mythread_self();
	//printf("__%d__\n",mainz->tid);
	int num=4;
	//char arr[num]={"func","func2","func3"};
	mythread_t tids[num];
	int i;

	/*for(i=0; i<num; i++)
	{
		mythread_create(&tids[i], NULL, func, NULL);
		//printf("Tid of %d is %d\n", i, tids[i]);
	}*/

	mythread_create(&tids[0], NULL, fun1, NULL);
	mythread_create(&tids[1], NULL, fun2, NULL);
	mythread_create(&tids[2], NULL, fun3, NULL);
	mythread_create(&tids[3], NULL, fun4, NULL);



	for(i=0; i<num; i++)
		mythread_join(tids[i], NULL);

	write(1, "PASS\n", 5);
	return 0;
}