#ifndef INCLUDE_HISTORY_H
#define INCLUDE_HISTORY_H

#include "include/args.h"

void update_history(Command *command);
void use_history(Command **command);
// add void as a parameter to make the function a valid prototype
bool is_history_empty(void);

#endif
