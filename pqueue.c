#include "pqueue.h"

void PQ_init(PQMgr_t* q, int num)
{
    q->length = 0;
    q->queue = malloc(num * sizeof(task_t));
}



/*
 remove top node and rebuild the heap
 */
void front_heapify(PQMgr_t* q)
{
    q->length -= 1;
    int idx = 0, test;
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

void add(PQMgr_t* q, task_t *t)
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

void remove(PQMgr_t* q)
{
    if(q->length <= 1)
    {
        q->length = 0;
    }
    else
    {
        int idx = q->length - 1;
        swap(&q->queue[0], &q->queue[idx]);
        front_heapify(q);
    }
}






