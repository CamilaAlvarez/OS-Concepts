#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BASE 10

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Missing input number\n");
        exit(EXIT_FAILURE);
    }
    // reset errno
    errno = 0;
    int input_number = (int)strtol(argv[1], (char **)NULL, BASE);

    if (input_number == 0 && errno != 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (input_number < 0)
    {
        fprintf(stderr, "We only accept possitve numbers!\n");
        exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Missing input number\n");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        while (input_number != 1)
        {
            printf("%d, ", input_number);
            if (input_number % 2 == 0)
            {
                input_number /= 2;
            }
            else
            {
                input_number = 3 * input_number + 1;
            }
        }
        printf("%d\n", input_number);
        exit(EXIT_SUCCESS);
    }
    return 0;
}