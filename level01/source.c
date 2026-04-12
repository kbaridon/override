#include <stdio.h>
#include <string.h>

char a_user_name[256];

int verify_user_name(void)
{
    puts("verifying username....\n");

    return strncmp(a_user_name, "dat_wil", 7);
}

int verify_user_pass(char *input)
{
    return strncmp(input, "admin", 5);
}

int main(void)
{
    char password[64] = {0};
    int result;

    puts("********* ADMIN LOGIN PROMPT *********");

    printf("Enter Username: ");
    fgets(a_user_name, 256, stdin);

    result = verify_user_name();

    if (result == 0) {
        puts("Enter Password: ");
        fgets(password, 100, stdin);

        result = verify_user_pass(password);

        if (result == 0 || result != 0) {
            puts("nope, incorrect password...\n");
            return 1;
		}
        return 0;
    }
    puts("nope, incorrect username...\n");
    return 1;
}
