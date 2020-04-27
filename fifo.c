#include "fifo.h"


// new idea, just fork and waidpid(... continued), and when catch, set newpid to pid
// at outer loop, compare if temp_pid == new_pid(they are the same in the beginning)
// if not, then it meansa process terminated or somewhat, 
// then change other process's priority

void time_unit()
{
    volatile unsigned long i; 
    for(i = 0; i < 1000000UL; i++);
}

/*
 This function is for FIFO scheduling.
 */
void schedule(tasks_t* taskArr)
{
    pid_t pid;
    cpu_set_t mask;
    int count = 0, num = taskArr->num;
    int timeline = 0, timestamp = 0;

    // done = malloc(taskArr->num * sizeof(task_t));

    // set main scheduler run at core 1
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    if(sched_setaffinity(getpid(), sizeof(mask), &mask) < 0)
    {
        perror("sched_setaffinity error!\n");
        exit(1);
    }

    int create_flag = 0, next = 0, setflag = 0;
    task_t* t;
    pid_t test;
    while(count < num)
    {
        // printf("count = %d\n", count);
        // printf("timeline = %d\n", timeline);
        // check if some task complete
        if(next == num - 1) // end case
        {
            test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);
            if(test < 0 && setflag == 0)
            {
                setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
            }
            else if(test > 0)
            {
                // printf("process %d finish\n", next);
                next += 1;
            }
        }
        else if(next < num - 1)
        {
            // already created
            if(taskArr->arr[next].pid > 0)
            {
                test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);

                // not complete, set priority high and not already set priority
                if(test < 0 && setflag == 0)
                {
                    setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                }
                // done
                else if(test > 0) 
                {
                    // printf("process %d finish\n", next);
                    next += 1;
                    setflag = 0;
                    // next process already being created
                    if(taskArr->arr[next].pid > 0)
                    {
                        setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                        setflag = 1;
                    }
                }
            }

        }

        // check if new task must be created
        for(int tp = count; tp < num; ++tp)
        {
            // for multiple processes arrive at the same time
            if(taskArr->arr[tp].rd_time <= timeline)
            {
                create_flag += 1;
            }
        }


        // create new task
        while(create_flag > 0)
        {
            // record start time

            // create new process
            t = &taskArr->arr[count];
            pid = fork();

            // child process
            if(pid == 0)
            {
                // printf("child process %d created\n", getpid());
                // set process priority
                CPU_ZERO(&mask);
                CPU_SET(0, &mask);
                if(sched_setaffinity(0, sizeof(mask), &mask) < 0)
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
                // print process information
                t->pid = getpid();
 
                // start processing
                for(int i = 0; i < t->exe_time; ++i)
                {
                    // printf("child %s: loop %d!\n", t->name, i);
                    time_unit();
                }
                printf("%s %d\n", t->name, t->pid);

                // record the end time, like getnstimeofday
                
                // process termination
                exit(0);
            }
            // main process
            else
            {
                t->pid = pid;
                create_flag -= 1;
                count += 1;
            }
        }

        // count time
        time_unit();
        timeline += 1;
        timestamp += 1;
    }

    // after creating all process, wait for remain processes to finish
    while(next < num)
    {
        // check if some task complete
        if(next == num - 1) // end case
        {
            test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);
            if(test < 0 && setflag == 0)
            {
                setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
            }
            else if(test > 0)
            {
                // printf("process %d finish\n", next);
                // break the loop
                next += 1; 
            }
        }
        else if(next < num - 1)
        {
            // already created
            if(taskArr->arr[next].pid > 0)
            {
                test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);

                // not complete, set priority high and not already set priority
                if(test < 0 && setflag == 0)
                {
                    setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                }
                // done
                else if(test > 0) 
                {
                    // printf("process %d finish\n", next);
                    next += 1;
                    setflag = 0;
                    // next process already being created
                    if(taskArr->arr[next].pid > 0)
                    {
                        setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                        setflag = 1;
                    }
                }
            }

        }

    }
    // maybe of no use
    // check if it termintate normally
    for(int i = 0; i < taskArr->num; ++i)
    {
        waitpid(taskArr->arr[i].pid, NULL, 0);
    }

    printf("Parent finish!\n");


}












