#include <stdio.h>

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
} Job;

int main()
{
    int n;
    scanf("%d", &n);

    Job procs[n], input[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d", &procs[i].pid, &procs[i].arrival, &procs[i].burst);
        input[i] = procs[i];
        procs[i].remain = procs[i].burst;
        procs[i].resp = -1;
    }

    int tq;
    scanf("%d", &tq);

    int time = 0, finished = 0;
    int queue[1000], head = 0, tail = 0;
    int inQueue[1000] = {0};

    for (int i = 0; i < n; i++)
    {
        if (procs[i].arrival == 0)
        {
            queue[tail++] = i;
            inQueue[i] = 1;
        }
    }

    while (finished < n)
    {
        if (head == tail)
        {
            time++;
            for (int i = 0; i < n; i++)
            {
                if (procs[i].arrival <= time && !inQueue[i] && procs[i].remain > 0)
                {
                    queue[tail++] = i;
                    inQueue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[head++];

        if (procs[idx].resp == -1)
            procs[idx].resp = time - procs[idx].arrival;

        int run = (procs[idx].remain > tq) ? tq : procs[idx].remain;
        procs[idx].remain -= run;
        time += run;

        for (int i = 0; i < n; i++)
        {
            if (procs[i].arrival <= time && !inQueue[i] && procs[i].remain > 0)
            {
                queue[tail++] = i;
                inQueue[i] = 1;
            }
        }

        if (procs[idx].remain > 0)
        {
            queue[tail++] = idx;
        }
        else
        {
            finished++;
            procs[idx].finish = time;
            procs[idx].tat = procs[idx].finish - procs[idx].arrival;
            procs[idx].wait = procs[idx].tat - procs[idx].burst;
        }
    }

    float sum_tat = 0, sum_wt = 0, sum_resp = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (input[i].pid == procs[j].pid)
            {
                printf("%d %d %d %d %d %d\n",
                       procs[j].pid, procs[j].arrival, procs[j].burst,
                       procs[j].tat, procs[j].wait, procs[j].resp);
                sum_tat += procs[j].tat;
                sum_wt += procs[j].wait;
                sum_resp += procs[j].resp;
                break;
            }
        }
    }

    printf("Average Turnaround Time = %.2f\n", sum_tat / n);
    printf("Average Waiting Time = %.2f\n", sum_wt / n);
    printf("Average Response Time = %.2f\n", sum_resp / n);

    return 0;
}
