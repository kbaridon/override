#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char msg[140];
    char username[40];
    int  msg_size;
} MessageContext;

void secret_backdoor(void)
{
    char cmd[128];

    fgets(cmd, 128, stdin);
    system(cmd);
}

void set_msg(MessageContext *ctx)
{
    char buf[1024];

    memset(buf, 0, sizeof(buf));

    puts(">: Msg @Unix-Dude");
    printf(">: ");

    fgets(buf, 1024, stdin);

    strncpy((char *)ctx, buf, (size_t)ctx->msg_size);
}

void set_username(MessageContext *ctx)
{
    char buf[144];

    memset(buf, 0, 128);

    puts(">: Enter your username");
    printf(">: ");

    fgets(buf, 128, stdin);

    int i = 0;
    while (i < 41 && buf[i] != '\0')
	{
        ctx->username[i] = buf[i];
        i++;
    }

    printf(">: Welcome, %s", ctx->username);
}

void handle_msg(void)
{
    MessageContext ctx;

    memset(&ctx.username[0], 0, 5 * sizeof(long long));

    ctx.msg_size = 0x8c;

    set_username(&ctx);
    set_msg(&ctx);

    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------");
    handle_msg();
    return 0;
}
