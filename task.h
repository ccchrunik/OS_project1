#ifndef TASK_H
#define TASK_H

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


#define TID 0
#define TNAME 1
#define TRD_T 2
#define TEXE_T 3
#define TREM_T 4

typedef struct task{
    char* name;
    int id;
    int pid;
    int rd_time;
    int exe_time;
    int rem_time;
    int fd[2];
    struct timespec start_time;
    struct timespec end_time;

} task_t;

typedef struct tasks{
    int num;
    char *name;
    task_t *arr;
    task_t *q;
    
} tasks_t;

typedef struct process_set{
    pid_t *pid_set;
    int *status;
} p_set;

void swap(task_t*, task_t*);
void time_unit();
char* time_concat(struct timespec *t);


#endif