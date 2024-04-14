#ifndef CONSOLE_H
#define CONSOLE_H

#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <commons/string.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commands.h>

typedef struct
{
    char *name;
    void (*func)(char *);
    char *doc;
    int expects_parameter;
} t_command;

#define BUFFER_MAX_LENGTH 100

void start_console(void);

#endif