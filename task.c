#include "task.h"

/*
 swap two task
 */
void swap(task_t* a, task_t* b)
{
    task_t temp = *a;
    *a = *b;
    *b = temp;
}