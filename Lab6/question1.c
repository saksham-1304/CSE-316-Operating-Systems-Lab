#include <stdio.h>

typedef struct
{
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

void sortByArrival(Process p[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p[j].arrival_time > p[j + 1].arrival_time)
            {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int n;
    scanf("%d", &n);

    Process p[n], original[n];

    for (int i = 0; i < n; i++)
    {
        scanf("%d%d%d", &p[i].pid, &p[i].arrival_time, &p[i].burst_time);
        original[i] = p[i]; // save original order
    }

    sortByArrival(p, n);

    int current_time = 0;
    float total_turnaround_time = 0, total_waiting_time = 0, total_response_time = 0;

    for (int i = 0; i < n; i++)
    {
        if (current_time < p[i].arrival_time)
        {
            current_time = p[i].arrival_time;
        }
        p[i].response_time = current_time - p[i].arrival_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        current_time = p[i].completion_time;

        total_turnaround_time += p[i].turnaround_time;
        total_waiting_time += p[i].waiting_time;
        total_response_time += p[i].response_time;
    }

    // Print in original input order
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (original[i].pid == p[j].pid)
            {
                printf("%d %d %d %d %d %d\n", p[j].pid, p[j].arrival_time, p[j].burst_time,
                       p[j].turnaround_time, p[j].waiting_time, p[j].response_time);
                break;
            }
        }
    }

    printf("Average Turnaround Time = %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time = %.2f\n", total_waiting_time / n);
    printf("Average Response Time = %.2f\n", total_response_time / n);

    return 0;
}