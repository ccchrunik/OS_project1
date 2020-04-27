#include "pqueue.h"

void PQ_init(PQMgr_t* q, int num)
{
    q->length = 0;
    q->queue = malloc(num * sizeof(task_t));
}



/*
 remove top node and rebuild the heap
 */
void front_heapify(PQMgr_t* q, int idx)
{
    q->length -= 1;
    int test;
    int t1 = idx * 2 + 1;
    int t2 = idx * 2 + 2;
    // swap until read end
    while(t1 < q->length)
    {
        // only compare t1, boundary case
        if(t2 == q->length)
        {
            if(q->queue[idx].rem_time > q->queue[t1].rem_time)
            {
                swap(&q->queue[idx], &q->queue[t1]);
            }
            break;
        }
        // t1 and t2 are all in the range
        else
        {
            // determine which node is smaller
            test = q->queue[t1].rem_time <= q->queue[t2].rem_time ? t1 : t2;
            if(q->queue[idx].rem_time > q->queue[test].rem_time)
            {
                swap(&q->queue[idx], &q->queue[test]);
                idx = test;
                t1 = idx * 2 + 1;
                t2 = idx * 2 + 2;
            }
            // idx smaller than next node, break
            else
            {
                break;
            }
        }
    }
    
}

/*
 add a new node and rebuild the heap
 */
void back_heapify(PQMgr_t* q, task_t* t)
{
    int idx = q->length;
    q->queue[idx] = *t;
    q->length += 1;

    while(idx > 0)
    {
        int test = (idx - 1) / 2;
        if(q->queue[test].rem_time > q->queue[idx].rem_time)
        {
            swap(&q->queue[test], &q->queue[idx]);
            idx = test;
        }
        else
        {
            break;
        }
    }

}

void pq_add(PQMgr_t* q, task_t *t)
{
    if(q->length == 0)
    {
        q->queue[0] = *t;
        q->length += 1;
    }
    else
    {
        back_heapify(q, t);
    }
}

void pq_remove(PQMgr_t* q)
{
    if(q->length <= 1)
    {
        q->length = 0;
    }
    else
    {
        int idx = q->length - 1;
        swap(&q->queue[0], &q->queue[idx]);
        front_heapify(q, 0);
    }
}

void pq_remove_item(PQMgr_t* q, pid_t pid)
{
    
    if(q->length == 1)
    {  
        q->length = 0;
    }
    else
    {
        int idx;
        for(int i = 0; i < q->length; ++i)
        {
            if(q->queue[i].pid == pid)
            {
                idx = i;
                break;
            }
        }

        int end = q->length - 1;
        swap(&q->queue[idx], &q->queue[end]);
        front_heapify(q, idx);
        
    }
    

}


void pq_print(PQMgr_t *q)
{
    printf("print queue\n");
    for(int i = 0; i < q->length; ++i)
    {
        printf("%s %d\n", q->queue[i].name, q->queue[i].rem_time);
    }
    printf("\n");
}


