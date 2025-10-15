#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct
{
    bool valid;
    uint32_t frame_no;
} PTE;

void print_binary(uint32_t num, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
}

int main()
{
    printf("=== Logical to Physical Address Translation using Paging ===\n\n");

    const int LOGICAL_BITS = 15;  // logical address bits
    const int PHYSICAL_BITS = 12; // physical address bits
    const int OFFSET_BITS = 7;    // page size bits -> page size = 2^7 = 128 bytes

    const uint32_t PAGE_SIZE = (1u << OFFSET_BITS);
    const uint32_t LOGICAL_SPACE = (1u << LOGICAL_BITS);
    const uint32_t PHYSICAL_SPACE = (1u << PHYSICAL_BITS);
    const uint32_t NUM_PAGES = LOGICAL_SPACE / PAGE_SIZE;
    const uint32_t NUM_FRAMES = PHYSICAL_SPACE / PAGE_SIZE;

    printf("Configuration:\n");
    printf("  Logical Address Bits : %d\n", LOGICAL_BITS);
    printf("  Physical Address Bits: %d\n", PHYSICAL_BITS);
    printf("  Page Size Bits       : %d => %u bytes\n", OFFSET_BITS, PAGE_SIZE);
    printf("  Logical Pages        : %u\n", NUM_PAGES);
    printf("  Physical Frames      : %u\n\n", NUM_FRAMES);

    // Create and fill a sample page table manually
    PTE page_table[NUM_PAGES];
    for (uint32_t i = 0; i < NUM_PAGES; i++)
    {
        page_table[i].valid = false;
        page_table[i].frame_no = 0;
    }

    page_table[0].valid = true;
    page_table[0].frame_no = 2;
    page_table[1].valid = true;
    page_table[1].frame_no = 5;
    page_table[2].valid = true;
    page_table[2].frame_no = 7;
    page_table[3].valid = true;
    page_table[3].frame_no = 1;
    page_table[4].valid = true;
    page_table[4].frame_no = 0;

    printf("Sample Page Table (page → frame):\n");
    for (uint32_t i = 0; i < NUM_PAGES; i++)
    {
        if (page_table[i].valid)
            printf(" Page %2u → Frame %2u\n", i, page_table[i].frame_no);
    }

    srand(time(NULL));
    printf("\nLogical Address Translation:\n");
    printf("Logical Addr (int)  Binary(Logical)       -> Physical Addr (int)  Binary(Physical)\n");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < 10; i++)
    {
        uint32_t logical_addr = rand() % LOGICAL_SPACE;
        uint32_t page_no = logical_addr >> OFFSET_BITS;
        uint32_t offset = logical_addr & (PAGE_SIZE - 1);

        printf("%10u  ", logical_addr);
        print_binary(logical_addr, LOGICAL_BITS);
        printf("  ->  ");

        if (page_table[page_no].valid)
        {
            uint32_t frame_no = page_table[page_no].frame_no;
            uint32_t physical_addr = (frame_no << OFFSET_BITS) | offset;

            printf("%10u  ", physical_addr);
            print_binary(physical_addr, PHYSICAL_BITS);
            printf("  (page=%u frame=%u offset=%u)\n", page_no, frame_no, offset);
        }
        else
        {
            printf("INVALID (page %u not in memory)\n", page_no);
        }
    }

    return 0;
}
