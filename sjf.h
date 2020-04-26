#ifndef SJF_H
#define SJF_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#include <sys/resource.h>
#include <sys/wait.h>

#include "plist.h"
#include "pqueue.h"

void schedule(tasks_t* t);
void time_unit();

#endif



