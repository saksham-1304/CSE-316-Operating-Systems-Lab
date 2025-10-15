#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <inttypes.h>

#define MAX_PROCESSES 10
#define MAX_FRAMES 1024
#define MAX_PAGES 256

typedef struct
{
    bool valid;
    int frame_no;
} PTE;

typedef struct
{
    int pid;
    int num_pages;
    int *page_loaded; 
    PTE *page_table;
} Process;

typedef struct
{
    bool occupied;
    int pid;
    int page_no;
} Frame;

int allocate_frame(Frame frames[], int num_frames, int pid, int page_no)
{
    for (int i = 0; i < num_frames; i++)
    {
        if (!frames[i].occupied)
        {
            frames[i].occupied = true;
            frames[i].pid = pid;
            frames[i].page_no = page_no;
            return i;
        }
    }
    return -1;
}

int main()
{
    srand(time(NULL));
    printf("=== Paging Scheme Simulation ===\n");

    int n, ram_size, page_size;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int process_size[MAX_PROCESSES];
    for (int i = 0; i < n; i++)
    {
        printf("Enter size (in bytes) of process %d: ", i);
        scanf("%d", &process_size[i]);
    }

    printf("Enter RAM size (in bytes): ");
    scanf("%d", &ram_size);
    printf("Enter page size (in bytes): ");
    scanf("%d", &page_size);

    int num_frames = ram_size / page_size;
    Frame frames[MAX_FRAMES] = {0};

    Process processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++)
    {
        processes[i].pid = i;
        processes[i].num_pages = (process_size[i] + page_size - 1) / page_size;
        processes[i].page_table = calloc(processes[i].num_pages, sizeof(PTE));
        processes[i].page_loaded = calloc(processes[i].num_pages, sizeof(int));
    }

    printf("\nTotal Frames in RAM: %d\n", num_frames);
    int allocated = 0;

    while (allocated < num_frames)
    {
        int pid = rand() % n;
        int page_no = rand() % processes[pid].num_pages;

        if (!processes[pid].page_table[page_no].valid)
        {
            int frame_no = allocate_frame(frames, num_frames, pid, page_no);
            if (frame_no == -1)
                break;

            processes[pid].page_table[page_no].valid = true;
            processes[pid].page_table[page_no].frame_no = frame_no;
            allocated++;
        }

        if (allocated == num_frames)
            break;
    }

    printf("\n=== Frame Allocation Table ===\n");
    printf("Frame No | PID | Page No\n");
    for (int i = 0; i < num_frames; i++)
    {
        if (frames[i].occupied)
            printf("%8d | %3d | %7d\n", i, frames[i].pid, frames[i].page_no);
        else
            printf("%8d |  -  |   Free\n", i);
    }

    printf("\n=== Page Tables for Each Process ===\n");
    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d (Total Pages: %d)\n", i, processes[i].num_pages);
        printf("Page No | Valid | Frame No\n");
        for (int p = 0; p < processes[i].num_pages; p++)
        {
            if (processes[i].page_table[p].valid)
                printf("%7d |  Yes  | %8d\n", p, processes[i].page_table[p].frame_no);
            else
                printf("%7d |   No  |    -\n", p);
        }
    }

    for (int i = 0; i < n; i++)
    {
        free(processes[i].page_table);
        free(processes[i].page_loaded);
    }

    return 0;
}
