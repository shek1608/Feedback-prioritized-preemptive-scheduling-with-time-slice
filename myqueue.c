/*

Single Author info:
smirans Sahibi Miranshah
Group info:
asarang Abhishek Sarangan
ayawalk Anuja Chnadrashekhar Yawalkar
smirans Sahibi Miranshah

*/
/*
 * myqueue.c -- implementation for queue ops
 */

#include "myqueue.h"
#include "mythread.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int mythread_inq(mythread_queue_t *headp, void *item)
{
  mythread_queue_t old, q, head;

  head = *headp;

#ifndef TRASH
  if (head == NULL)
    return FALSE;
#endif

  q = head;
  do {
    if (q->item == item)
      return TRUE; /* found */
    q = q->next;
  } while (q != head);

  return FALSE; /* not found */
}

int mythread_lenq(mythread_queue_t *headp)
{
  mythread_queue_t old, q, head;
  int ret = 0;

  head = *headp;

#ifndef TRASH
  if (head == NULL)
    return ret;
#endif

  q = head;
  do {
    q = q->next;
    ret++;
  } while (q != head);

  return ret;
}

void mythread_q_init(mythread_queue_t *headp, void *item)
{
  mythread_queue_t new, head;

  new = (mythread_queue_t) malloc(sizeof(struct mythread_queue));
  if (new == NULL) {
    printf("malloc error\n");
    exit(1);
  }
  new->item = item;

  new->prev = new;
  new->next = new;

  *headp = new;
}

void mythread_enq_tail(mythread_queue_t *headp, void *item)
{
  mythread_queue_t new, head, temp;

  if (*headp == NULL) {
    mythread_q_init(headp, item);
    return;
  }

  head = *headp;
  
  new = (mythread_queue_t) malloc(sizeof(struct mythread_queue));
  if (new == NULL) {
    printf("malloc error\n");
    exit(1);
  }
  new->item = item;

  //Search for position
  mythread_queue_t qt = (mythread_queue_t)(head->prev);
  mythread_t th = (mythread_t)(qt->item);
  mythread_t new_item = (mythread_t)(new->item);
  mythread_t head_item = (mythread_t)(head->item);

  if(th->preemptions<=new_item->preemptions)  //if new item needs to be added at end of queue.
  {
  	new->prev = head->prev;
  	new->next = head;
  	head->prev->next = new;
  	head->prev = new;
  }
  else if(new_item->preemptions < head_item->preemptions) //if new item needs to be added at head;
  {
  	new->prev = head->prev;
  	new->next = head;
  	head->prev->next = new;
  	head->prev = new;
  	//head = new;
  	*headp = head->prev;
  }
  else  //needs to be added somewhere between
  {
  	temp = *headp;
  	mythread_t temp_item = (mythread_t)(temp->item);
  	while(temp_item->preemptions <= new_item->preemptions) //we will have the item whose previous should contain the new item after this while
  	{
  		temp=temp->next;
  		temp_item = (mythread_t)(temp->item);
  	}
  	new->prev=temp->prev;
  	new->next=temp;
  	temp->prev->next=new;
  	temp->prev=new;
  }

  


  /*
  mythread_queue_t new, head;

  if (*headp == NULL) {
    mythread_q_init(headp, item);
    return;
  }

  head = *headp;

  new = (mythread_queue_t) malloc(sizeof(struct mythread_queue));
  if (new == NULL) {
    printf("malloc error\n");
    exit(1);
  }
  new->item = item;

  new->prev = head->prev;
  new->next = head;
  head->prev->next = new;
  head->prev = new;
  */
}

void mythread_enq_head(mythread_queue_t *headp, void *item)
{
  mythread_queue_t new, head;

  if (*headp == NULL) {
    mythread_q_init(headp, item);
    return;
  }

  head = *headp;

  mythread_enq_tail(&head->next, item);

  *headp = head->prev;
}

void mythread_deq(mythread_queue_t *headp, void *item)
{
  mythread_queue_t old, q, head;

  head = *headp;

  q = head;
  do {
    if (q->item == item) {
      old = q;
      old->next->prev = q->prev;
      old->prev->next = q->next;
      if (old == head) {
	if (old->next == old)
	  *headp = (mythread_queue_t) NULL;
	else
	  *headp = old->next;
      }
      free(old);
      return; /* found */
    }
    q = q->next;
  } while (q != head);

  /* not found */
}

void* mythread_deq_head(mythread_queue_t *headp)
{
	mythread_queue_t head = *headp;
	if(head!=NULL) return head->item;
	else return NULL;
}