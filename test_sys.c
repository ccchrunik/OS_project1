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

char* time_concat(char *s1, char *s2)
{
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

    char sec[100], nsec[100], *tval1, *tval2;
    sprintf(sec, "%ld", ts.tv_sec);
    sprintf(nsec, "%ld", ts.tv_nsec);
    tval1 = time_concat(sec, nsec);
    
    

    printf("get start time: %s\n", tval1);
    waste_time();

    syscall(334, &te);

    sprintf(sec, "%ld", te.tv_sec);
    sprintf(nsec, "%ld", te.tv_nsec);
    tval2 = time_concat(sec, nsec);
    printf("get end time: %s\n", tval2);

    syscall(335, getpid(), tval1, tval2);
    
    
    return 0;
}












