#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>

int auth(char *login, unsigned int serial)
{
    size_t len;
    unsigned int hash;
    int i;

    len = strcspn(login, "\n");
    login[len] = '\0';

    len = strnlen(login, 32);
    if ((int)len < 6)
        return 1;

    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
    {
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m'---------------------------'");
        return 1;
    }

    hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;

    for (i = 0; i < (int)len; i++)
    {
        if (login[i] < 32)
            return 1;

        hash += ((int)login[i] ^ hash) % 0x539;
    }

    if (serial == hash)
        return 0;

    return 1;
}

int main(void)
{
    char login[32];
    unsigned int serial;

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(login, sizeof(login), stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");

    printf("-> Enter Serial: ");
    scanf("%u", &serial);

    if (auth(login, serial) == 0)
    {
        puts("Authenticated!");
        system("/bin/sh");
    }

    return 0;
}
