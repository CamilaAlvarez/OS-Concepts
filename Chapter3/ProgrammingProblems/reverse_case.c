#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Missing input string\n");
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];
    int input_length = strlen(argv[1]);
    int parentWriteFd[2];
    int childWriteFd[2];
    pipe(parentWriteFd);
    pipe(childWriteFd);
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        close(parentWriteFd[0]);
        close(childWriteFd[1]);
        // + 1 because of \0, we wouldn't want to copy an unterminated string
        write(parentWriteFd[1], (void *)argv[1], input_length + 1);
        wait(NULL);
        read(childWriteFd[0], buffer, input_length + 1);
        printf("%s\n", buffer);
        close(parentWriteFd[1]);
        close(childWriteFd[0]);
    }
    else
    {
        close(parentWriteFd[1]);
        close(childWriteFd[0]);
        read(parentWriteFd[0], buffer, input_length + 1);
        char *buffer_ptr = buffer;
        while (*buffer_ptr != '\0')
        {
            // It's not just if and else, because there might be non-alpha characters
            if (isalpha(*buffer_ptr) && islower(*buffer_ptr))
            {
                *buffer_ptr = toupper(*buffer_ptr);
            }
            else if (isalpha(*buffer_ptr) && isupper(*buffer_ptr))
            {
                *buffer_ptr = tolower(*buffer_ptr);
            }
            buffer_ptr++;
        }
        write(childWriteFd[1], (void *)buffer, input_length + 1);
        close(parentWriteFd[0]);
        close(childWriteFd[1]);
        exit(EXIT_SUCCESS);
    }
    return 0;
}