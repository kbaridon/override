
#include <stdio.h>
#include <string.h>

void gethash(void)
{
    char login[32];
    size_t len;
    unsigned int hash;
    int i;

    printf("Enter login: ");
    if (fgets(login, sizeof(login), stdin) == NULL)
        return ;

    len = strcspn(login, "\n");
    login[len] = '\0';

    len = strnlen(login, 32);
    if ((int)len < 6)
        return ;

    hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;

    for (i = 0; i < (int)len; i++)
    {
        if (login[i] < 32)
            return ;

        hash += ((int)login[i] ^ hash) % 0x539;
    }

    printf("%u", hash);
}

int main()
{
	gethash();
	return 0;
}
