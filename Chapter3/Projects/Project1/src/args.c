#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/args.h"

static void fill_command(Command *parsed_args, int number_args)
{
    // last token is newline
    if (number_args > 0)
    {
        parsed_args->command = parsed_args->arguments[0];
        parsed_args->arguments[number_args] = NULL;
        if (strcmp("&", parsed_args->arguments[number_args - 1]) == 0)
        {
            parsed_args->type = PARALLEL_COMMAND;
            parsed_args->arguments[number_args - 1] = NULL;
        }
        else if (strcmp("!!", parsed_args->arguments[0]) == 0)
        {
            parsed_args->type = HISTORY_COMMAND;
        }
        else
        {
            parsed_args->type = SERIAL_COMMAND;
        }
    }
    else
    {
        parsed_args->type = EMPTY;
        parsed_args->arguments[number_args] = NULL;
    }
}

// Parsing arguments, we can modify args since it is an array!
Command *parse_args_as_command(void)
{
    Command *parsed_args = (Command *)malloc(sizeof(Command));
    Command *initial_command = parsed_args;
    parsed_args->input = NULL;
    parsed_args->output = NULL;
    parsed_args->next_command = NULL;
    char buffer[MAX_LINE];
    fgets(buffer, MAX_LINE, stdin);

    char *token = strtok(buffer, " \n");
    int number_args = 0;
    bool is_next_input = false;
    bool is_next_output = false;
    while (token != NULL)
    {
        if (strlen(token) > 0)
        {
            // we're assuming the string is constructed correctly: we don't have something like ls > < in.txt => this should lead to a parse error
            if (strcmp(token, "<") == 0)
            {
                is_next_input = true;
                token = strtok(NULL, " \n");
                continue;
            }
            else if (strcmp(token, ">") == 0)
            {
                is_next_output = true;
                token = strtok(NULL, " \n");
                continue;
            }
            // Pipe
            else if (strcmp(token, "|") == 0)
            {
                // 1. Finalize current command
                fill_command(parsed_args, number_args);
                // 2. Reset number arguments
                number_args = 0;
                // 3. Create and set next command
                parsed_args->next_command = (Command *)malloc(sizeof(Command));
                // 4. Set current command to next command
                parsed_args = parsed_args->next_command;
                token = strtok(NULL, " \n");
                continue;
            }
            if (is_next_input)
            {
                parsed_args->input = strdup(token);
                is_next_input = false;
                token = strtok(NULL, " \n");
                continue;
            }
            else if (is_next_output)
            {
                parsed_args->output = strdup(token);
                is_next_output = false;
                token = strtok(NULL, " \n");
                continue;
            }
            // strdup creates dynamic memory and duplicates token, memory must be then freed
            parsed_args->arguments[number_args] = strdup(token);
            number_args++;
        }
        token = strtok(NULL, " \n");
    }
    // last token is newline
    fill_command(parsed_args, number_args);
    return initial_command;
}
void print_args(Command *command)
{
    char **args = command->arguments;
    while (*args != NULL)
    {
        printf("%s ", *args);
        args++;
    }
    if (command->input != NULL)
    {
        printf("< %s ", command->input);
    }
    if (command->output != NULL)
    {
        printf("> %s ", command->output);
    }
    if (command->next_command != NULL)
    {
        printf("| ");
        print_args(command->next_command);
    }
}
void free_command(Command *command)
{
    char **args = command->arguments;
    free_arguments(args);
    if (command->input != NULL)
    {
        free(command->input);
    }
    if (command->output != NULL)
    {
        free(command->output);
    }
    Command *next_command = command->next_command;
    while (next_command != NULL)
    {
        Command *aux = next_command->next_command;
        free(next_command);
        next_command = aux;
    }
    free(command);
}
void free_arguments(char *arguments[])
{
    while (*arguments != NULL)
    {
        free(*arguments);
        *arguments = NULL;
        arguments++;
    }
}
