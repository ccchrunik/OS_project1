#include "psjf.h"

#define TIME_QUANTUM 500


void time_unit()
{
    volatile unsigned long i; 
    for(i = 0; i < 1000000UL; i++);
    // for(i = 0; i < 1000000UL; ++i)
    // {
    //     for(int j = 0; j < 1; ++j);
    // }
}

/*
 This function is for SJF scheduling.
 */
void schedule(tasks_t* taskArr)
{
    pid_t pid;
    cpu_set_t mask;
    int count = 0, num = taskArr->num;
    int timeline = 0, timestamp = 0;
    PQMgr_t q;

    struct sched_param param;

    // for(int i = 0; i < num; ++i)
    // {
    //     printf("id = %d\n", taskArr->arr[i].id);
    //     printf("Process = %s\n", taskArr->arr[i].name);
    // }

    PQ_init(&q, num);

    // set main scheduler run at core 1
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    if(sched_setaffinity(getpid(), sizeof(mask), &mask) < 0)
    {
        perror("sched_setaffinity error!\n");
        exit(1);
    }

    int create_flag = 0, next = 0, setflag = 0, tflag = 0, checkflag = 0;
    task_t *t;
    task_t target;
    pid_t test;
    while(count < num)
    {
        // task had not been assigned

        // initial case

        // check if new task must be created
        for(int tp = count; tp < num; ++tp)
        {
            // printf("tp = %d\n", tp);
            // for multiple processes arrive at the same time
            if(taskArr->arr[tp].rd_time <= timeline)
            {
                create_flag += 1;
                pq_add(&q, &taskArr->arr[tp]);
                // for(int i = 0; i < q.length; ++i)
                // {
                //     printf("Process %s %d\n", q.queue[i].name, q.queue[i].exe_time);
                // }
                // printf("\n");
                printf("in creating queue\n");
                pq_print(&q);
            }
        }

        
        // create new task
        while(create_flag > 0)
        {
            // record start time
       
            // create new process
            t = &taskArr->arr[count];
            // printf("count = %d\n", count);
            // printf("child process %s created\n", t->name);
            pid = fork();

            // child process
            if(pid == 0)
            {
                // set process priority
                CPU_ZERO(&mask);
                CPU_SET(0, &mask);
                if(sched_setaffinity(0, sizeof(mask), &mask) < 0)
                {
                    perror("sched_setaffinity error!\n");
                    exit(1);
                }
                 
                // if(t->id == 2)
                // {
                //     printf("count = %d\n", count);
                // }
                // if at the top of queue
                if(tflag == 0 && t->id == q.queue[0].id)
                {
                    setpriority(PRIO_PROCESS, getpid(), -20);
                }
                // else
                // {
                //     setpriority(PRIO_PROCESS, getpid(), 19);
                // }
                // print process information
                t->pid = getpid();
 
                // start processing
                for(int i = 0; i < t->exe_time; ++i)
                {
                    if(i == 0)
                    {
                        setpriority(PRIO_PROCESS, getpid(), 19);
                    }
                    // printf("child %s: loop %d!\n", t->name, i);
                    // printf("Process %s priority = %d\n", t->name, getpriority(PRIO_PROCESS, getpid()));
                    time_unit();
                }

                // record the end time, like getnstimeofday
                
                // process termination
                printf("%s %d\n", t->name, t->pid);
                exit(0);
            }
            // main process
            else
            {
                t->pid = pid;
                for(int i = 0; i < q.length; ++i)
                {
                    if(q.queue[i].id == t->id)
                    {
                        q.queue[i].pid = pid;
                        break;
                    }
                }

                create_flag -= 1;
                count += 1;
            }
        }

        // initial case
        if(checkflag == 0 && q.length > 0)
        {
            // printf("initial setting priority!\n");
            checkflag = 1;
            target = q.queue[0];
            setpriority(PRIO_PROCESS, target.pid, -20);
            // pq_remove(&q);
            tflag = 1;
        }

        if(next == num - 1) // end case
        {
            // a task is running
            if(tflag == 1)
            {
                test = waitpid(target.pid, NULL, WNOHANG);
                // printf("test = %d\n", test);
                if(test > 0)
                {
                    next += 1;
                    tflag = 0;
                    // task is done
                    pq_remove(&q);
                }
            }
            // no task is running
            else if(q.length > 0)
            {
                target = q.queue[0];
                setpriority(PRIO_PROCESS, target.pid, -20);
                // pq_remove(&q);
                tflag = 1;
            }

        }
        else if(next < num)
        {
            // a task is running
            if(tflag == 1)
            {
                // printf("target.pid = %d\n", target.pid);
                test = waitpid(target.pid, NULL, WNOHANG);
                // printf("test = %d\n", test);
                // printf("priority = %d\n", getpriority(PRIO_PROCESS, target.pid));
                
                // the task is finish
                if(test > 0)
                {
                    test = 0;
                    next += 1;
                    tflag = 0;
                    pq_remove(&q);
                    timestamp = 0;

                    // check if some tasks are pending
                    if(q.length > 0)
                    {
                        target = q.queue[0];
                        // printf("target.pid = %d", target.pid);
                        setpriority(PRIO_PROCESS, target.pid, -20);
                        // pq_remove(&q);
                        tflag = 1;
                    }
                }
                else if(timestamp >= TIME_QUANTUM && target.rem_time < TIME_QUANTUM)
                {
                    setpriority(PRIO_PROCESS, target.pid, 19);
                    tflag = 0;
                    timestamp = 0;
                    for(int i = 0; i < num; ++i)
                    {
                        if(taskArr->arr[i].pid == target.pid)
                        {
                            pq_remove_item(&q, target.pid);
                            // pq_print(&q);
                            taskArr->arr[i].rem_time -= TIME_QUANTUM;
                            // pq_add(&q, &taskArr->arr[i]);
                            pq_print(&q);

                            // target = taskArr->arr[i];
                            break;
                        }
                    }
                    // have some task pending
                    if(q.length > 0)
                    {
                        tflag = 1;
                        target = q.queue[0];
                        setpriority(PRIO_PROCESS, target.pid, -20);
                    }
                }
                // last over a time quantum, preempt the tasks
                else if(timestamp >= TIME_QUANTUM && target.rem_time >= TIME_QUANTUM)
                {
                    setpriority(PRIO_PROCESS, target.pid, 19);
                    tflag = 0;
                    timestamp = 0;
                    for(int i = 0; i < num; ++i)
                    {
                        if(taskArr->arr[i].pid == target.pid)
                        {
                            pq_remove_item(&q, target.pid);
                            // pq_print(&q);
                            taskArr->arr[i].rem_time -= TIME_QUANTUM;
                            pq_add(&q, &taskArr->arr[i]);
                            pq_print(&q);

                            // target = taskArr->arr[i];
                            break;
                        }
                    }
                    // have some task pending
                    if(q.length > 0)
                    {
                        tflag = 1;
                        target = q.queue[0];
                        setpriority(PRIO_PROCESS, target.pid, -20);
                    }
                }
            }
            // no task is running
            else if(q.length > 0)
            {
                target = q.queue[0];
                setpriority(PRIO_PROCESS, target.pid, -20);
                // pq_remove(&q);
                tflag = 1;
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
        if(next == num - 1) // end case
        {
            // a task is running
            if(tflag == 1)
            {
                test = waitpid(target.pid, NULL, WNOHANG);
                // printf("test = %d\n", test);
                if(test > 0)
                {
                    next += 1;
                    tflag = 0;
                    // task is done
                    pq_remove(&q);
                }
                else if(timestamp >= TIME_QUANTUM && target.rem_time >= TIME_QUANTUM)
                {
                    timestamp = 0;
                    q.queue[0].rem_time -= TIME_QUANTUM;
                    pq_print(&q);
                }
            }
            // no task is running
            else if(q.length > 0)
            {
                target = q.queue[0];
                setpriority(PRIO_PROCESS, target.pid, -20);
                // pq_remove(&q);
                tflag = 1;
            }

        }
        else if(next < num)
        {
            // a task is running
            if(tflag == 1)
            {
                // printf("target.pid = %d\n", target.pid);
                test = waitpid(target.pid, NULL, WNOHANG);
                // printf("test = %d\n", test);
                // printf("priority = %d\n", getpriority(PRIO_PROCESS, target.pid));
                
                // the task is finish
                if(test > 0)
                {
                    test = 0;
                    next += 1;
                    tflag = 0;
                    pq_remove(&q);
                    timestamp = 0;

                    // check if some tasks are pending
                    if(q.length > 0)
                    {
                        target = q.queue[0];
                        // printf("target.pid = %d", target.pid);
                        setpriority(PRIO_PROCESS, target.pid, -20);
                        // pq_remove(&q);
                        tflag = 1;
                    }
                }
                else if(timestamp >= TIME_QUANTUM && target.rem_time < TIME_QUANTUM)
                {
                    setpriority(PRIO_PROCESS, target.pid, 19);
                    tflag = 0;
                    timestamp = 0;
                    for(int i = 0; i < num; ++i)
                    {
                        if(taskArr->arr[i].pid == target.pid)
                        {
                            pq_remove_item(&q, target.pid);
                            // pq_print(&q);
                            taskArr->arr[i].rem_time -= TIME_QUANTUM;
                            // pq_add(&q, &taskArr->arr[i]);
                            pq_print(&q);

                            // target = taskArr->arr[i];
                            break;
                        }
                    }
                    // have some task pending
                    if(q.length > 0)
                    {
                        tflag = 1;
                        target = q.queue[0];
                        setpriority(PRIO_PROCESS, target.pid, -20);
                    }
                }
                // last over a time quantum, preempt the tasks
                else if(timestamp >= TIME_QUANTUM && target.rem_time >= TIME_QUANTUM)
                {
                    setpriority(PRIO_PROCESS, target.pid, 19);
                    tflag = 0;
                    timestamp = 0;
                    for(int i = 0; i < num; ++i)
                    {
                        if(taskArr->arr[i].pid == target.pid)
                        {
                            pq_remove_item(&q, target.pid);
                            // pq_print(&q);
                            taskArr->arr[i].rem_time -= TIME_QUANTUM;
                            pq_add(&q, &taskArr->arr[i]);
                            pq_print(&q);

                            // target = taskArr->arr[i];
                            break;
                        }
                    }
                    // have some task pending
                    if(q.length > 0)
                    {
                        tflag = 1;
                        target = q.queue[0];
                        setpriority(PRIO_PROCESS, target.pid, -20);
                    }
                }
            }
            // no task is running
            else if(q.length > 0)
            {
                target = q.queue[0];
                setpriority(PRIO_PROCESS, target.pid, -20);
                // pq_remove(&q);
                tflag = 1;
            }
            
        }
        time_unit();
        timeline += 1;
        timestamp += 1;
    }



    printf("Parent finish!\n");


}












