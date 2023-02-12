#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int pid = fork();
    // Child process
    if (pid == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        sleep(30);
    }
    return 0;
}