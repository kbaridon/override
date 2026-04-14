#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void log_wrapper(FILE *file, char *prefix, char *user_input)
{
    char buffer[264];

    strcpy(buffer, prefix);
    size_t len = strlen(buffer);

    snprintf(buffer + len, sizeof(buffer) - len, user_input);

    size_t newline_pos = strcspn(buffer, "\n");
    buffer[newline_pos] = '\0';

    fprintf(file, "LOG: %s\n", buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    FILE *log_file = fopen("./backups/.log", "w");
    if (!log_file) {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    log_wrapper(log_file, "Starting back up: ", argv[1]);

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    char path[104];
    strncpy(path, "./backups/", sizeof(path) - 1);
    path[sizeof(path) - 1] = '\0';

    strncat(path, argv[1], sizeof(path) - strlen(path) - 1);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0700);
    if (fd < 0) {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    int c;
    char ch;
    while ((c = fgetc(input)) != EOF) {
        ch = (char)c;
        write(fd, &ch, 1);
    }

    log_wrapper(log_file, "Finished back up ", argv[1]);

    fclose(input);
    close(fd);

    return 0;
}
