#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "include/command.h"
#include "include/history.h"

static void perform_chain_command(Command *command);
static bool parse_args_with_history(Command **command, bool *has_history_param)
{
    bool should_execute = true;
    if ((*command)->type == EMPTY)
    {
        printf("Empty command\n");
        should_execute = false;
    }
    else if ((*command)->type == HISTORY_COMMAND)
    {
        if (is_history_empty())
        {
            printf("No commands in history\n");
            should_execute = false;
        }
        else
        {
            *has_history_param = true;
            use_history(command);
        }
    }
    return should_execute;
}
static void change_input_output(Command *command, int *in_fd, int *out_fd)
{

    if (command->input != NULL)
    {
        *in_fd = open(command->input, O_RDONLY);
        if (*in_fd < 0)
        {
            printf("Couldn't open input file: %s\n", command->input);
        }
        else
        {
            dup2(*in_fd, STDIN_FILENO);
        }
    }
    if (command->output != NULL)
    {
        *out_fd = open(command->output, O_WRONLY | O_CREAT);
        if (*out_fd < 0)
        {
            printf("Couldn't open output file: %s\n", command->output);
        }
        else
        {
            dup2(*out_fd, STDOUT_FILENO);
        }
    }
}
static void execute_command_in_child(Command *command)
{
    // if there is a next command we create a pipe and fork
    if (command->next_command != NULL)
    {
        perform_chain_command(command);
        // If the previous function returns it means that exec failed
        printf("Error while executing command: %s\n", command->command);
        // If we couldn't execute the command we should free the memory
        free_command(command);
        wait(NULL);
    }
    else
    {
        int in_fd = -1, out_fd = -1;
        change_input_output(command, &in_fd, &out_fd);
        execvp(command->command, command->arguments);
        printf("Error while executing command: %s\n", command->command);
        // If we couldn't execute the command we should free the memory
        free_command(command);
        if (in_fd > 0)
        {
            close(in_fd);
        }
        if (out_fd > 0)
        {
            close(out_fd);
        }
    }
}
static void perform_chain_command(Command *command)
{
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid < 0)
    {
        printf("Fork has failed\n");
        return;
    }
    // Current command writes (uses fd[1])
    else if (pid > 0)
    {
        close(fd[0]);
        // Write to fd[1]
        dup2(fd[1], STDOUT_FILENO);
        execvp(command->command, command->arguments);
    }
    // Next command reads (uses fd[0])
    else
    {
        close(fd[1]);
        // Read from fd[0]
        dup2(fd[0], STDIN_FILENO);
        // we give priority to < before | (ls | sort < in.txt will sort in.txt)
        execute_command_in_child(command->next_command);
    }
}
void execute_command(Command *command)
{
    bool use_history = false;
    bool should_execute = parse_args_with_history(&command, &use_history);
    if (!should_execute)
    {
        return;
    }
    int pid = fork();
    if (pid < 0)
    {
        printf("Fork has failed\n");
        return;
    }
    else if (pid == 0)
    {
        execute_command_in_child(command);
        return;
    }
    else if (pid > 0)
    {
        if (!use_history)
        { // Save args in history
            update_history(command);
        }
        if (command->type == SERIAL_COMMAND)
        {
            wait(NULL);
        }
        else
        {
            printf("\n");
        }
    }
}
