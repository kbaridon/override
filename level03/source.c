#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int decrypt(void *ctx)
{
    unsigned char data[17] = {
        0x51, 0x7d, 0x7c, 0x75, 0x60, 0x73, 0x66, 0x67,
        0x7e, 0x73, 0x66, 0x7b, 0x7d, 0x7c, 0x61, 0x33, 0x00
    }; //Q}|u`sfg~sf{}|a3

    size_t len = strlen((char *)data);

    for (size_t i = 0; i < len; i++) {
        data[i] ^= (unsigned char)(uintptr_t)ctx;
    }

    if (strcmp((char *)data, "Congratulations!") == 0) {
        return system("/bin/sh");
    } else {
        puts("\nInvalid Password");
        return 0;
    }
}

void test(int param_1, int param_2)
{
    void *ctx = (void *)(param_2 - param_1);

    if ((long)ctx >= 1 && (long)ctx <= 0x15) {
        decrypt(ctx);
    } else {
        ctx = (void *)rand();
        decrypt(ctx);
    }
}

int main(void)
{
    unsigned int seed;
    int input;

    seed = time(NULL);
    srand(seed);

    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");

    printf("Password: ");
    scanf("%d", &input);

    test(input, 322424845);

    return 0;
}