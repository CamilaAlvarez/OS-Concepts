#include <stdlib.h>
#include <stdio.h>
#include "include/history.h"

static Command *history = NULL;

void update_history(Command *command)
{
    if (history != NULL)
    {
        free_command(history);
        history = NULL;
    }
    history = command;
}
void use_history(Command **command)
{
    print_args(history);
    printf("\n");
    *command = history;
}
bool is_history_empty(void)
{
    return history == NULL;
}
