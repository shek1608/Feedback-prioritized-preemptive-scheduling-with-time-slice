CC = gcc
CFLAGS = -g

all: a5

p5: a5

a5: mysched.o myqueue.o
	$(CC) -lm mytest.c mysched.o myqueue.o mythread.a -o mytest

mysched.o: mysched.c mysched.h mythread.a myqueue.h futex.h futex_inline.h mycond.h mymutex.h

myqueue.o: myqueue.h myqueue.c mythread.a

clean:
	rm -rf mysched.o myqueue.o mytest