#include <stdio.h>

typedef struct
{
    int pid;     // process id
    int arrival; // arrival time
    int burst;   // burst time
    int finish;  // completion time
    int tat;     // turnaround time
    int wait;    // waiting time
    int resp;    // response time
    int done;    // flag for completion
} Job;

int main()
{
    int n;
    scanf("%d", &n);

    Job proc[n], ref[n];

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d", &proc[i].pid, &proc[i].arrival, &proc[i].burst);
        proc[i].done = 0; // mark as not completed
        ref[i] = proc[i]; // keep copy for original order
    }

    int finished = 0, time = 0;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    while (finished < n)
    {
        int sel = -1;
        int shortest = 1e9;

        // find process with minimum burst among arrived and not finished
        for (int i = 0; i < n; i++)
        {
            if (!proc[i].done && proc[i].arrival <= time)
            {
                if (proc[i].burst < shortest ||
                    (proc[i].burst == shortest && proc[i].arrival < proc[sel].arrival))
                {
                    shortest = proc[i].burst;
                    sel = i;
                }
            }
        }

        if (sel == -1)
        {
            time++;
            continue;
        }

        proc[sel].resp = time - proc[sel].arrival;
        time += proc[sel].burst;
        proc[sel].finish = time;
        proc[sel].tat = proc[sel].finish - proc[sel].arrival;
        proc[sel].wait = proc[sel].tat - proc[sel].burst;
        proc[sel].done = 1;
        finished++;

        sum_tat += proc[sel].tat;
        sum_wt += proc[sel].wait;
        sum_rt += proc[sel].resp;
    }

    // print in original input order
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (ref[i].pid == proc[j].pid)
            {
                printf("%d %d %d %d %d %d\n",
                       proc[j].pid, proc[j].arrival, proc[j].burst,
                       proc[j].tat, proc[j].wait, proc[j].resp);
                break;
            }
        }
    }

    printf("Average Turnaround Time = %.2f\n", sum_tat / n);
    printf("Average Waiting Time = %.2f\n", sum_wt / n);
    printf("Average Response Time = %.2f\n", sum_rt / n);

    return 0;
}
