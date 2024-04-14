#include <console.h>

// este array para readline

void execute_script(char *file_path);

char *COMMAND_NAMES[] = {
    "EJECUTAR_SCRIPT",
    "INICIAR_PROCESO",
    "FINALIZAR_PROCESO",
    "DETENER_PLANIFICACION",
    "INICIAR_PLANIFICACION",
    "MULTIPROGRAMACION",
    "PROCESO_ESTADO",
    NULL};

/* When non-zero, this global means the user is done using this program. */
int done;

void do_nothing(char *algo)
{
    printf("me llego un comando %s\n", algo);
}

t_command COMMANDS[] = {
    {"EJECUTAR_SCRIPT", execute_script, "Execute a script", 1},
    {"INICIAR_PROCESO", init_process, "Init a process", 1},
    {"FINALIZAR_PROCESO", end_process, "Terminate a process", 1},
    {"DETENER_PLANIFICACION", stop_scheduler, "stop the short term scheduler", 0},
    {"INICIAR_PLANIFICACION", start_scheduler, "start or restart the short term scheduler", 0},
    {"MULTIPROGRAMACION", multiprogramacion, "change multiprogramacion value", 1},
    {"PROCESO_ESTADO", list_processes_by_state, "list processes by their current state", 0},
    {NULL, NULL}};

char *generator(const char *text, int state)
{
    static int idx;
    char *ret;

    if (!state)
        idx = 0;

    while ((ret = COMMANDS[idx++].name))
    {
        if (strstr(ret, text) != NULL)
            return strdup(ret);
    }

    return NULL;
}

char **completion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, generator);
}

void parse_command(char *string)
{
    char **command_split = string_n_split(string, 2, " ");
    char *name = command_split[0];
    char *param = command_split[1];

    // check for names
    for (int i = 0; COMMANDS[i].name != NULL; i++)
    {
        if (!strcmp(name, COMMANDS[i].name))
        {
            if (COMMANDS[i].expects_parameter && param == NULL)
            {
                printf("esperaba un parametro, ");
                break;
            }
            if (!COMMANDS[i].expects_parameter && param != NULL)
            {
                printf("no esperaba parametros, ");
                break;
            }
            COMMANDS[i].func(param);
            string_array_destroy(command_split);
            return;
        }
    }

    printf("comando invalido\n");
    string_array_destroy(command_split);
}

void start_console(void)
{

    rl_bind_key('\t', rl_complete);
    rl_attempted_completion_function = completion;

    char *line;

    for (;;)
    {
        line = readline(">: ");

        if (!line)
            break;

        /* Remove leading and trailing whitespace from the line.
           Then, if there is anything left, add it to the history list
           and execute it. */
        string_trim(&line);

        if (*line)
        {
            add_history(line);
            parse_command(line);
        }

        free(line);
    }
}

// JUSTO ESTE COMANDO LA TENGO QUE PONER ACA POR TEMA DE INCLUDES

void execute_script(char *file_path)
{
    FILE *f = fopen(file_path, "r");
    char buffer[BUFFER_MAX_LENGTH];
    if (!f)
    {
        printf("script no encontrado\n");
        return;
    }
    t_list *commands = list_create();
    while (!feof(f))
    {
        fgets(buffer, BUFFER_MAX_LENGTH, f);
        if ('\n' == buffer[strlen(buffer) - 1])
            buffer[strlen(buffer) - 1] = '\0';

        list_add(commands, strdup(buffer));
    }

    void exec_comm(void *comm)
    {
        char *command = (char *)comm;
        parse_command(command);
    }

    list_iterate(commands, exec_comm);
    list_destroy_and_destroy_elements(commands, free);
}