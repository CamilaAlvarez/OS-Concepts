#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "include/args.h"
#include "include/command.h"

int main(void)
{
    int should_run = 1;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout); // Writes buffer content to stdout
        Command *command = parse_args_as_command();
        execute_command(command);
    }
    return 0;
}
