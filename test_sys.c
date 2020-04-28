#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <string.h>

void waste_time()
{
    for(int i = 0; i < 100000000; ++i)
    {
        int a = 10000 * 10000;
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




int main()
{
    struct timespec ts, te;

    printf("start testing\n");
    printf("add %ld\n", syscall(333, 1, 1));
    syscall(334, &ts);

    // char sec[100], nsec[100], *tval1, *tval2;
    // sprintf(sec, "%ld", ts.tv_sec);
    // sprintf(nsec, "%ld", ts.tv_nsec);
    // tval1 = time_concat(sec, nsec);

    char *s1 = time_concat(&ts);
    printf("get start time: %s\n", s1);
    waste_time();

    syscall(334, &te);

    // sprintf(sec, "%ld", te.tv_sec);
    // sprintf(nsec, "%ld", te.tv_nsec);
    // tval2 = time_concat(sec, nsec);

    char *s2 = time_concat(&te);
    printf("get end time: %s\n", s2);

    syscall(335, getpid(), s1, s2);
    
    
    return 0;
}












