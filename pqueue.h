#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "task.h"

typedef struct PQMgr
{
    task_t *queue;
    int length;
} PQMgr_t;

void PQ_init(PQMgr_t* q, int num);
void front_heapify(PQMgr_t* q, int idx);
void back_heapify(PQMgr_t* q, task_t* t);
void pq_add(PQMgr_t* q, task_t *t);
void pq_remove(PQMgr_t* q);
void pq_remove_item(PQMgr_t* q, pid_t pid);
void pq_print(PQMgr_t* q);

#endif






