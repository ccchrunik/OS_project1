#include <stdio.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>

void waste_time()
{
    for(int i = 0; i < 100000000; ++i)
    {
        int a = 10000 * 10000;
    }
}

int main()
{
    struct timespec ts, te;

    printf("start testing\n");
    printf("add %ld\n", syscall(333, 1, 1));
    // syscall(334, &ts);

    printf("get start time\n");
    waste_time();

    // syscall(334, &te);
    printf("get end time\n");

    syscall(335, getpid(), &ts, &te);
    
    
    return 0;
}












