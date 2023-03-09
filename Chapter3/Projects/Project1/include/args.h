#ifndef INCLUDE_ARGS_H
#define INCLUDE_ARGS_H

#include <stdbool.h>
#define MAX_LINE 80 // maximum length command

typedef enum
{
    EMPTY,
    SERIAL_COMMAND,
    PARALLEL_COMMAND,
    HISTORY_COMMAND
} CommandType;

typedef struct command
{
    char *command;
    char *arguments[MAX_LINE / 2 + 1]; // command line arguments: Array of char*
    char *input;
    char *output;
    struct command *next_command;
    CommandType type;
} Command;

Command *parse_args_as_command(void);
void print_args(Command *command);
void free_arguments(char *arguments[]);
void free_command(Command *command);
#endif
