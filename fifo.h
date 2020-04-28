#ifndef FIFO_H
#define FIFO_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#include <sys/resource.h>
#include <sys/wait.h>
// #include <linux/timekeeping.h>
#include <time.h>
#include <sys/syscall.h>

#include "plist.h"

void schedule_fifo(tasks_t* t);
void time_unit();

#endif



