#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct
{
    int pid;
    int arrival;
    int burst;
    int remain;
    int finish;
    int tat;
    int wait;
    int resp;
    bool begun;
    bool done;
} Task;

int main()
{
    int n;
    scanf("%d", &n);

    Task proc[n], order[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d", &proc[i].pid, &proc[i].arrival, &proc[i].burst);
        proc[i].remain = proc[i].burst;
        proc[i].begun = false;
        proc[i].done = false;
        order[i] = proc[i];
    }

    int finished = 0, clock = 0;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    while (finished < n)
    {
        int sel = -1, best = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!proc[i].done && proc[i].arrival <= clock && proc[i].remain < best && proc[i].remain > 0)
            {
                best = proc[i].remain;
                sel = i;
            }
        }

        if (sel == -1)
        {
            clock++;
            continue;
        }

        if (!proc[sel].begun)
        {
            proc[sel].resp = clock - proc[sel].arrival;
            proc[sel].begun = true;
        }

        proc[sel].remain--;
        clock++;

        if (proc[sel].remain == 0)
        {
            proc[sel].finish = clock;
            proc[sel].tat = proc[sel].finish - proc[sel].arrival;
            proc[sel].wait = proc[sel].tat - proc[sel].burst;
            proc[sel].done = true;
            finished++;

            sum_tat += proc[sel].tat;
            sum_wt += proc[sel].wait;
            sum_rt += proc[sel].resp;
        }
    }

    // print results in input order
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (order[i].pid == proc[j].pid)
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
