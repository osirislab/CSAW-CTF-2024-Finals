#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

struct string {
    uint16_t len;
    char data[];
};

struct msg {
    char location[32];
    struct string *description;
};

#define MAX_MESSAGES 1024
size_t message_count = 0;
struct msg messages[MAX_MESSAGES];

void print_menu() {
    printf("\nDept of Sanitization Reporting System\n");
    printf("1. Add report\n");
    printf("2. View reports\n");
    printf("3. Resolve report\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void add_message() {
    if (message_count >= MAX_MESSAGES) {
        printf("Message limit reached!\n");
        return;
    }

    struct msg *new_message = &messages[message_count++];

    printf("Enter location (max 31 characters): ");
    read(STDIN_FILENO, &new_message->location, 32);
    new_message->location[strcspn(new_message->location, "\n")] = 0;

    printf("Enter description: ");
    char temp[4096];
    fgets(temp, 4096, stdin);
    temp[strcspn(temp, "\n")] = 0;
    
    size_t len = strlen(temp);
    new_message->description = malloc(sizeof(struct string) + len);
    if (!new_message->description) {
        printf("Memory allocation failed!\n");
        _exit(1);
    }
    new_message->description->len = len;
    memcpy(new_message->description->data, temp, len);

    printf("Message added successfully!\n");
}

void view_message() {
    if (message_count == 0) {
        printf("No messages to view!\n");
        return;
    }

    printf("Enter message index (0-%lu): ", message_count - 1);
    int index;
    scanf("%d", &index);
    clear_input_buffer();

    if (index < 0 || index >= message_count) {
        printf("Invalid index!\n");
        return;
    }

    printf("\nMessage #%d:\n", index);
    printf("Location: %s\n", messages[index].location);
    printf("Description: ");
    write(STDOUT_FILENO, messages[index].description->data, messages[index].description->len);
}

void resolve_message() {
    if (message_count == 0) {
        printf("No messages to resolve!\n");
        return;
    }

    printf("Enter message index to resolve (0-%lu): ", message_count - 1);
    int index;
    scanf("%d", &index);
    clear_input_buffer();

    if (index < 0 || index >= message_count) {
        printf("Invalid index!\n");
        return;
    }

    free(messages[index].description);

    printf("Message resolved successfully!\n");
}

int main() {
    int choice;

    setbuf(stdout, NULL);

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_message();
                break;
            case 2:
                view_message();
                break;
            case 3:
                resolve_message();
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

