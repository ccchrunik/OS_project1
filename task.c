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

void time_unit()
{
    volatile unsigned long i; 
    // for(i = 0; i < 1000000UL; i++);
    for(i = 0; i < 1000000UL; i++)
    {
        // for(int j = 0; j < 10; ++j);
    }

}

char* time_concat(struct timespec *t)
{
    char s1[100], s2[100];
    sprintf(s1, "%ld", t->tv_sec);
    sprintf(s2, "%ld", t->tv_nsec);
    char  *result = malloc(strlen(s1) + 9 + 2);
    strcpy(result, s1);
    strcat(result, ".");
    for(int i = 0; i < (9 - strlen(s2)); ++i)
    {
        strcat(result, "0");
    }
    strcat(result, s2);
    return result;
}