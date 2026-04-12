#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned char buffer[100];
    unsigned int i = 0;

    fgets((char *)buffer, 100, stdin);

    while (1)
    {
        unsigned int len = 0;
        while (buffer[len] != '\0')
            len++;

        if (len <= i)
        {
            printf((char *)buffer);
            exit(0);
        }
        if (buffer[i] >= 'A' && buffer[i] <= 'Z')
            buffer[i] = buffer[i] ^ 32;
        i++;
    }
}
