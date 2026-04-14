#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned int get_unum(void)
{
    char buf[32];

    if (fgets(buf, sizeof(buf), stdin) == NULL)
        exit(1);

    return (unsigned int)strtoul(buf, NULL, 10);
}

int store_number(int *data)
{
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();

    printf(" Index: ");
    index = get_unum();

    if ((index % 3 == 0) || ((number >> 24) == 183)) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }

    data[index] = number;
    return 0;
}

int read_number(int *data)
{
    unsigned int index;

    printf(" Index: ");
    index = get_unum();

    printf(" Number at data[%u] is %u\n", index, data[index]);

    return 0;
}

int main(int argc, char **argv, char **envp)
{
    int data[100];
    char cmd[20];
    int result = 0;

    memset(data, 0, sizeof(data));
    memset(cmd, 0, sizeof(cmd));

    for (char **p = argv; *p != NULL; p++) {
        memset(*p, 0, strlen(*p));
    }

    for (char **p = envp; *p != NULL; p++) {
        memset(*p, 0, strlen(*p));
    }

    puts(
        "----------------------------------------------------\n"
        "  Welcome to wil's crappy number storage service!   \n"
        "----------------------------------------------------\n"
        " Commands:\n"
        "    store - store a number into the data storage\n"
        "    read  - read a number from the data storage\n"
        "    quit  - exit the program\n"
        "----------------------------------------------------\n"
        "   wil has reserved some storage :>\n"
        "----------------------------------------------------\n"
    );

    while (1)
	{
        printf("Input command: ");

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        cmd[strcspn(cmd, "\n")] = '\0';

        result = 1;

        if (strcmp(cmd, "store") == 0)
            result = store_number(data);
        else if (strcmp(cmd, "read") == 0)
            result = read_number(data);
        else if (strcmp(cmd, "quit") == 0)
            return 0;

        if (result == 0)
            printf(" Completed %s command successfully\n", cmd);
        else
            printf(" Failed to do %s command\n", cmd);

        memset(cmd, 0, sizeof(cmd));
    }

    return 0;
}
