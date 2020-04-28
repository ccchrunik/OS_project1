# makefile for scheduling program
#
# make rr - for round-robin scheduling
# make fifo - for FIFO scheduling
# make sjf - for SJF scheduling
# make sjfp - for SJFP scheduling

CC=gcc
CFLAGS=-Wall

all: main.o task.o plist.o pqueue.o fifo.o rr.o sjf.o psjf.o
	$(CC) $(CFLAGS) -o schedule main.o task.o plist.o pqueue.o fifo.o rr.o sjf.o psjf.o

clean:
	rm -rf *.o
	rm -rf fifo
	rm -rf rr
	rm -rf sjf
	rm -rf psjf

fifo: main.o plist.o fifo.o task.o
	$(CC) $(CFLAGS) -o fifo main.o fifo.o plist.o task.o

rr: main.o plist.o rr.o task.o
	$(CC) $(CFLAGS) -o rr main.o rr.o plist.o task.o

sjf: main.o plist.o pqueue.o sjf.o
	$(CC) $(CFLAGS) -o sjf main.o sjf.o plist.o pqueue.o task.o

psjf: main.o plist.o pqueue.o psjf.o
	$(CC) $(CFLAGS) -o psjf main.o psjf.o plist.o pqueue.o task.o


main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

fifo.o: fifo.c fifo.h
	$(CC) $(CFLAGS) -c fifo.c

rr.o: rr.c rr.h
	$(CC) $(CFLAGS) -c rr.c

sjf.o: sjf.c sjf.h
	$(CC) $(CFLAGS) -c sjf.c

psjf.o: psjf.c psjf.h
	$(CC) $(CFLAGS) -c psjf.c

task.o: task.c task.h
	$(CC) $(CFLAGS) -c task.c

plist.o: plist.c plist.h
	$(CC) $(CFLAGS) -c plist.c

pqueue.o: pqueue.c pqueue.h
	$(CC) $(CFLAGS) -c pqueue.c

# tellwait.o: tellwait.c tellwait.h
# 	$(CC) $(CFLAGS) -c tellwait.c


