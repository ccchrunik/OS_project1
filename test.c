#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#include <sys/resource.h>
#include <sys/wait.h>

void waste_time()
{
    int a = 1000000000;
    while(a != 0)
    {
        int tmp = 10000*10000;
        int t = 10000*10000;
        a--;
    }
    // sleep(1);
}



int main()
{
    pid_t *p, *d;
    pid_t test;
    int count = 0;
    int *s;
    cpu_set_t mask;

    p = malloc(5 * sizeof(pid_t));
    s = malloc(5 * sizeof(int));
    for(int i = 0; i < 5; ++i)
    {
        s[i] = -100;
        p[i] = -100;
    }

    // set main process in core 1
    int head = 0, start = 0, end = 5;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    if(sched_setaffinity(getpid(), sizeof(mask), &mask) < 0)
    {
        perror("sched_setaffinity error!\n");
        exit(1);
    }

    int timestamp = 0;
    while(count < 5)
    {
        printf("timestamp = %d\n", timestamp);
        // printf("%d\n", count);
        if(head == 4)
        {
            if(p[head] > 0)
            {
                printf("checking!\n");
                test = waitpid(p[head], NULL, WNOHANG);
                printf("test = %d\n", test);
                if(test > 0)
                {
                    printf("p[%d] = %d\n", head + 1, p[head]);
                    printf("process %d complete!\n", head + 1);
                    printf("print process %d priority %d\n", p[head], getpriority(PRIO_PROCESS, p[head]));
                    head += 1;
                }
            }
        }
        else
        {
            // check if that task has been created
            if(p[head] > 0)
            {
                printf("checking!\n");
                test = waitpid(p[head], NULL, WNOHANG);
                printf("test = %d\n", test);
                if(test > 0)
                {
                    head += 1;
                    printf("print process %d priority %d\n", p[head], getpriority(PRIO_PROCESS, p[head]));
                    printf("p[%d] = %d\n", head, p[head]);
                    printf("process %d complete!\n", head);
                    setpriority(PRIO_PROCESS, p[head], -20);
                }
            }
            
        }
        
        if(timestamp % 3 == 0)
        {
            printf("create new process!\n");
            pid_t pid;
            pid = fork();

            if(pid == 0)
            {
                // set child process in core 0
                CPU_ZERO(&mask);
                CPU_SET(0, &mask);
                if(sched_setaffinity(getpid(), sizeof(mask), &mask) < 0)
                {
                    perror("sched_setaffinity error!\n");
                    exit(1);
                }

                if(count == 0)
                {
                    setpriority(PRIO_PROCESS, getpid(), -20);
                }
                else
                {
                    setpriority(PRIO_PROCESS, getpid(), 19);
                }
                
                for(int k = 0; k < 1; ++k)
                {
                    waste_time();
                }
                printf("process %d priority is %d\n", getpid(), getpriority(PRIO_PROCESS, getpid()));
                exit(0);
            }
            else
            {
                // printf("parent get pid = %d\n", pid);
                p[count] = pid;
                // printf("p %d is %d\n", count, p[count]);
                count += 1;
            }

        }

        waste_time();
        timestamp += 1;
        
        
    }

    while(head < 5)
    {
        if(head == 4)
        {
            if(p[head] > 0)
            {
                printf("checking!\n");
                test = waitpid(p[head], NULL, WNOHANG);
                printf("test = %d\n", test);
                if(test > 0)
                {
                    printf("p[%d] = %d\n", head + 1, p[head]);
                    printf("process %d complete!\n", head + 1);
                    printf("print process %d priority %d\n", p[head], getpriority(PRIO_PROCESS, p[head]));
                    head += 1;
                }
            }
        }
        else
        {
            // check if that task has been created
            if(p[head] > 0)
            {
                printf("checking!\n");
                test = waitpid(p[head], NULL, WNOHANG);
                printf("test = %d\n", test);
                if(test > 0)
                {
                    head += 1;
                    printf("p[%d] = %d\n", head, p[head]);
                    printf("process %d complete!\n", head);
                    setpriority(PRIO_PROCESS, p[head], -20);
                    printf("print process %d priority %d\n", p[head], getpriority(PRIO_PROCESS, p[head]));
                }
            }
            
        }

        waste_time();
    }

    // for(int i = 0; i < head; ++i)
    // {
    //     waitpid(p[i], NULL, 0);
    // }


    printf("parent process ends\n");
    

    return 0;
}





