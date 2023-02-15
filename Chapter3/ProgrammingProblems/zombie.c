#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// When running ps -l the child has state Z
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