#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

void get_input();
void get_flag();
uint64_t val;

int cash = 20;
int plane_ticket_cost = 120;

void get_input() {
    char buf[64]; // User input buffer
    uint64_t val_1 = val; // Copy of the saved canary for checking

    // Print the canary for reference (for debugging purposes)
    printf("\n\tI found this address on a post-it note on the ground: 0x%" PRIx64 "\n", val);
    puts("\n\tCan you get more cash?");
    puts(">");
    fflush(stdout); // Ensure output is flushed

    gets(buf); // Vulnerable to buffer overflow

    // Check if the canary was modified
    if (val_1 != val) {
        //printf("\nExpected canary: 0x%" PRIx64, val);
        //printf("\nCurrent canary: 0x%" PRIx64 "\n", val_1);
        printf("That's too bad, try again\n");
        fflush(stdout);
        exit(1);
    }
}

int main() {
    // Disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    // Seed the random number generator and initialize the canary
    srand(time(NULL));
    val = ((uint64_t)rand() << 32) | rand();

    // Modify the cash value
    cash += 10;

    // Call get_input() for the challenge
    get_input();

    // Check if cash matches the plane ticket cost
    if (cash == plane_ticket_cost) {
        get_flag();
    } else {
        //printf("\n\tCurrent cash: %d, plane ticket cost: %d\n", cash, plane_ticket_cost);
        if (cash < plane_ticket_cost) {
            puts("\n\tThat's not enough cash! Vacay next year? :(\n\n");
        } else {
            puts("\n\tThat's too much cash, we don't have change :(\n\n");
        }
        fflush(stdout);
        exit(0);
    }

    return 0;
}

void get_flag() {
    // Print the flag if conditions are met
     if (cash == plane_ticket_cost) {
        system("cat flag.txt");
        fflush(stdout);
        exit(0);
    }
    printf("Haha, not so fast. Try again >.<\n");
    
}
