#include <stdio.h>

typedef struct
{
    int id, at, bt, rt, ct, tat, wt, prio, res;
} proc;

int main()
{
    int n;
    scanf("%d", &n);
    proc p[n], o[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d%d%d%d", &p[i].id, &p[i].at, &p[i].bt, &p[i].prio);
        o[i] = p[i];
        p[i].rt = p[i].bt;
        p[i].res = -1;
    }
    int completed = 0, t = 0;

    while (completed < n)
    {
        int idx = -1;
        int highest = 1000000;
        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= t && p[i].rt > 0)
            {
                if (p[i].prio < highest || (p[i].prio == highest && p[i].at < (idx == -1 ? 1000000 : p[idx].at)))
                {
                    highest = p[i].prio;
                    idx = i;
                }
            }
        }
        if (idx == -1)
        {
            t++;
            continue;
        }
        if (p[idx].res == -1)
            p[idx].res = t - p[idx].at;
        p[idx].rt--;
        t++;
        if (p[idx].rt == 0)
        {
            p[idx].ct = t;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }

    float tot_tat = 0, tot_wt = 0, tot_res = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (p[j].id == o[i].id)
            {
                printf("%d %d %d %d %d %d %d\n", p[j].id, p[j].prio, p[j].at, p[j].bt, p[j].tat, p[j].wt, p[j].res);
                tot_tat += p[j].tat;
                tot_wt += p[j].wt;
                tot_res += p[j].res;
                break;
            }
        }
    }
    printf("Average Turnaround Time = %.2f\n", tot_tat / n);
    printf("Average Waiting Time = %.2f\n", tot_wt / n);
    printf("Average Response Time = %.2f\n", tot_res / n);
    return 0;
}