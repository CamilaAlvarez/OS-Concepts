#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_SIZE 256

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "missing at least one filename!\n");
        exit(EXIT_FAILURE);
    }
    char buffer[READ_SIZE];
    int fd[2];
    char *input = argv[1];
    char *dest = argv[2];
    pipe(fd);
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        close(fd[0]);
        FILE *input_fd = fopen(input, "r");
        while (fgets(buffer, READ_SIZE, input_fd) != NULL)
        {
            write(fd[1], buffer, READ_SIZE);
        }
        close(fd[1]);
        fclose(input_fd);
        wait(NULL);
    }
    else
    {
        close(fd[1]);
        FILE *dest_fd = fopen(dest, "w");
        // TODO: we should check for errors, read might return 0 if there's an error
        while (read(fd[0], buffer, READ_SIZE) > 0)
        {
            fputs(buffer, dest_fd);
        }
        close(fd[0]);
        fclose(dest_fd);
        exit(EXIT_SUCCESS);
    }
    return 0;
}