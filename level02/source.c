#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char input_pwd[112] = {0};
    char flag[48] = {0};
    char username[100] = {0};
    int size_flag = 0;
    FILE *fd = NULL;

    fd = fopen("/home/users/level03/.pass", "r");
    if (fd == NULL) {
        fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
        exit(1);
    }

    size_t sVar2 = fread(flag, 1, 41, fd);
    size_flag = (int)sVar2;

    sVar2 = strcspn(flag, "\n");
    flag[sVar2] = '\0';

    if (size_flag != 41) {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }

    fclose(fd);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    printf("--[ Username: ");
    fgets(username, 100, stdin);
    sVar2 = strcspn(username, "\n");
    username[sVar2] = '\0';

    printf("--[ Password: ");
    fgets(input_pwd, 100, stdin);
    sVar2 = strcspn(input_pwd, "\n");
    input_pwd[sVar2] = '\0';

    puts("*****************************************");

    if (strncmp(flag, input_pwd, 41) == 0) {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");
        return 0;
    }

    printf(username);
    puts(" does not have access!");

    exit(1);
}
