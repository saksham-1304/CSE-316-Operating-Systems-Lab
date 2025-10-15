#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int size;
    int allocated;
    int process_id;
    int internal_fragmentation;
} Block;

typedef struct
{
    int size;
    int allocated;
    int block_id;
} Process;

void displayStatus(Block blocks[], int nb, Process processes[], int np)
{
    printf("\n------------------------------------------------------------\n");
    printf("Block No.\tBlock Size\tProcess No.\tProcess Size\tInternal Frag\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < nb; i++)
    {
        if (blocks[i].allocated)
        {
            printf("%d\t\t%d\t\tP%d\t\t%d\t\t%d\n",
                   i + 1, blocks[i].size, blocks[i].process_id + 1,
                   processes[blocks[i].process_id].size, blocks[i].internal_fragmentation);
        }
        else
        {
            printf("%d\t\t%d\t\t---\t\t---\t\t---\n", i + 1, blocks[i].size);
        }
    }
    printf("------------------------------------------------------------\n");
}

void calculateFragmentation(Block blocks[], int nb, Process processes[], int np)
{
    int total_internal = 0, external = 0;
    for (int i = 0; i < nb; i++)
    {
        if (blocks[i].allocated)
            total_internal += blocks[i].internal_fragmentation;
        else
            external += blocks[i].size;
    }
    printf("\nTotal Internal Fragmentation = %d KB\n", total_internal);

    int not_allocated = 0;
    for (int i = 0; i < np; i++)
        if (!processes[i].allocated)
            not_allocated = 1;

    if (not_allocated)
        printf("External Fragmentation = %d KB (Total free space)\n", external);
    else
        printf("No External Fragmentation\n");
}

void firstFit(Block blocks[], int nb, Process processes[], int np)
{
    printf("\n=============== FIRST FIT ALLOCATION ===============\n");

    // Reset block and process allocations
    for (int i = 0; i < nb; i++)
    {
        blocks[i].allocated = 0;
        blocks[i].process_id = -1;
        blocks[i].internal_fragmentation = 0;
    }
    for (int i = 0; i < np; i++)
    {
        processes[i].allocated = 0;
        processes[i].block_id = -1;
    }

    for (int i = 0; i < np; i++)
    {
        for (int j = 0; j < nb; j++)
        {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size)
            {
                blocks[j].allocated = 1;
                blocks[j].process_id = i;
                blocks[j].internal_fragmentation = blocks[j].size - processes[i].size;
                processes[i].allocated = 1;
                processes[i].block_id = j;
                break;
            }
        }
    }

    displayStatus(blocks, nb, processes, np);
    calculateFragmentation(blocks, nb, processes, np);
}

void bestFit(Block blocks[], int nb, Process processes[], int np)
{
    printf("\n=============== BEST FIT ALLOCATION ===============\n");

    // Reset allocations
    for (int i = 0; i < nb; i++)
    {
        blocks[i].allocated = 0;
        blocks[i].process_id = -1;
        blocks[i].internal_fragmentation = 0;
    }
    for (int i = 0; i < np; i++)
    {
        processes[i].allocated = 0;
        processes[i].block_id = -1;
    }

    for (int i = 0; i < np; i++)
    {
        int best_index = -1;
        for (int j = 0; j < nb; j++)
        {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size)
            {
                if (best_index == -1 || blocks[j].size < blocks[best_index].size)
                    best_index = j;
            }
        }
        if (best_index != -1)
        {
            blocks[best_index].allocated = 1;
            blocks[best_index].process_id = i;
            blocks[best_index].internal_fragmentation = blocks[best_index].size - processes[i].size;
            processes[i].allocated = 1;
            processes[i].block_id = best_index;
        }
    }

    displayStatus(blocks, nb, processes, np);
    calculateFragmentation(blocks, nb, processes, np);
}

int main()
{
    int total_memory, nb, np, choice;

    printf("Enter size of Main Memory (in KB): ");
    scanf("%d", &total_memory);

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    Block blocks[nb];
    printf("\nChoose Partition Type:\n1. Equal Size Blocks\n2. Unequal Size Blocks\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        int block_size = total_memory / nb;
        printf("\nEach Block Size = %d KB\n", block_size);
        for (int i = 0; i < nb; i++)
            blocks[i].size = block_size;
    }
    else
    {
        printf("\nEnter sizes of %d blocks (in KB):\n", nb);
        for (int i = 0; i < nb; i++)
            scanf("%d", &blocks[i].size);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &np);

    Process processes[np];
    printf("Enter sizes of %d processes (in KB):\n", np);
    for (int i = 0; i < np; i++)
        scanf("%d", &processes[i].size);

    firstFit(blocks, nb, processes, np);
    bestFit(blocks, nb, processes, np);

    return 0;
}
