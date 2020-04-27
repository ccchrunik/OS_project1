#include "rr.h"


// new idea, just fork and waidpid(... continued), and when catch, set newpid to pid
// at outer loop, compare if temp_pid == new_pid(they are the same in the beginning)
// if not, then it meansa process terminated or somewhat, 
// then change other process's priority

#define RR_QUANTUM 500

void time_unit()
{
    volatile unsigned long i; 
    // for(i = 0; i < 1000000UL; i++);
    for(i = 0; i < 1000000UL; ++i)
    {
        for(int j = 0; j < 1; ++j);
    }
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

    plist_t p;
    list_init(&p);

    // for(int i = 0; i < num; ++i)
    // {
    //     list_insert_tail(&p, &taskArr->arr[i]);
    // }

    // list_print(&p);
    // done = malloc(taskArr->num * sizeof(task_t));

    // set main scheduler run at core 1
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    if(sched_setaffinity(getpid(), sizeof(mask), &mask) < 0)
    {
        perror("sched_setaffinity error!\n");
        exit(1);
    }

    int create_flag = 0, next = 0, tflag = 0, checkflag = 0;
    task_t *t, *target;
    pid_t test;
    while(count < num)
    {
        // printf("count = %d\n", count);
        // printf("timeline = %d\n", timeline);
        // check if some task complete
        

        // check if new task must be created
        for(int tp = count; tp < num; ++tp)
        {
            // for multiple processes arrive at the same time
            if(taskArr->arr[tp].rd_time <= timeline)
            {
                // insert a node into list
                list_insert_tail(&p, &taskArr->arr[tp]);
                create_flag += 1;
                list_print(&p);
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
                t->pid = getpid();

                // printf("Process %s count = %d\n", t->name, count);
                if(count == 0)
                {
                    setpriority(PRIO_PROCESS, getpid(), -20);
                }
                else
                {
                    setpriority(PRIO_PROCESS, getpid(), 19);
                }
                // print process information
 
                // start processing
                for(int i = 0; i < t->exe_time; ++i)
                {
                    
                    // printf("child %s: loop %d!\n", t->name, i);
                    // printf("Process %s priority = %d\n", t->name, getpriority(PRIO_PROCESS, getpid()));
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

        if(checkflag == 0 && p.length > 0)
        {
            // printf("initial set priority!\n");
            checkflag = 1;
            tflag = 1;
            target = p.dummy->next->task;
            setpriority(PRIO_PROCESS, target->pid, -20);
            // printf("Process %s priority %d\n", target->name, getpriority(PRIO_PROCESS, target->pid));
        }

        if(next == num - 1) // end case
        {
            test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);
            

            if(test > 0)
            {
                // printf("process %d finish\n", next);
                next += 1;
            }
            else if(timestamp >= RR_QUANTUM)
            {
                // setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                timestamp = 0;
                target->rem_time -= RR_QUANTUM;
                list_print(&p);
            }
        }
        else if(next < num - 1)
        {
            // a task is running
            if(tflag == 1)
            {
                test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);

                // done
                if(test > 0) 
                {
                    // printf("process %d finish\n", next);
                    next += 1;
                    tflag = 0;
                    list_delete_head(&p);
                    // next process already being created
                    if(p.length > 0)
                    {
                        tflag = 1;
                        target = p.dummy->next->task;
                        setpriority(PRIO_PROCESS, target->pid, -20);
                    }
                }
                // not complete, set priority high and not already set priority
                else if(timestamp >= RR_QUANTUM)
                {
                    // switch to a new task
                    target->rem_time -= RR_QUANTUM;
                    timestamp = 0;
                    tflag = 0;
                    setpriority(PRIO_PROCESS,target->pid, 19);
                    list_delete_head(&p);
                    list_insert_tail(&p, target);

                    // list_print(&p);
                    if(p.length > 0)
                    {
                        tflag = 1;
                        target = p.dummy->next->task;
                        setpriority(PRIO_PROCESS, target->pid, -20);
                    }

                    
                }
            }
            else if(tflag == 0)
            {
                if(p.length > 0)
                {
                    tflag = 1;
                    target = p.dummy->next->task;
                    setpriority(PRIO_PROCESS, target->pid, -20);
                }
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
        // printf("target = %s\n", target->name);
        // printf("timestamp = %d\n", timestamp);
        // check if some task complete
        if(next == num - 1) // end case
        {
            test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);
            

            if(test > 0)
            {
                // printf("process %d finish\n", next);
                next += 1;
            }
            else if(timestamp >= RR_QUANTUM)
            {
                // setpriority(PRIO_PROCESS, taskArr->arr[next].pid, -20);
                timestamp = 0;
                target->rem_time -= RR_QUANTUM;
                // list_print(&p);
            }
        }
        else if(next < num - 1)
        {
            // a task is running
            if(tflag == 1)
            {
                test = waitpid(taskArr->arr[next].pid, NULL, WNOHANG);

                // done
                if(test > 0) 
                {
                    // printf("process %d finish\n", next);
                    next += 1;
                    tflag = 0;
                    list_delete_head(&p);
                    // next process already being created
                    if(p.length > 0)
                    {
                        tflag = 1;
                        target = p.dummy->next->task;
                        setpriority(PRIO_PROCESS, target->pid, -20);
                    }
                }
                // not complete, set priority high and not already set priority
                else if(timestamp >= RR_QUANTUM)
                {
                    // switch to a new task
                    target->rem_time -= RR_QUANTUM;
                    timestamp = 0;
                    tflag = 0;
                    setpriority(PRIO_PROCESS,target->pid, 19);
                    list_delete_head(&p);
                    list_insert_tail(&p, target);

                    // if(target->rem_time < 0)
                    // {
                    //     list_delete_head(&p);
                    // }

                    // list_print(&p);
                    if(p.length > 0)
                    {
                        tflag = 1;
                        target = p.dummy->next->task;
                        setpriority(PRIO_PROCESS, target->pid, -20);
                    }

                    
                }
            }
            else if(tflag == 0)
            {
                if(p.length > 0)
                {
                    tflag = 1;
                    target = p.dummy->next->task;
                    setpriority(PRIO_PROCESS, target->pid, -20);
                }
            }

        }

        time_unit();
        timeline += 1;
        timestamp += 1;

    }
    // maybe of no use
    // check if it termintate normally
    // for(int i = 0; i < taskArr->num; ++i)
    // {
    //     waitpid(taskArr->arr[i].pid, NULL, 0);
    // }

    printf("Parent finish!\n");


}












