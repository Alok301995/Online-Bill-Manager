#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ********** Global************
typedef struct command
{
    char *command;
    char *field;
} Command;

typedef struct files
{
    char *file_1;
    char *file_2;
} Files;

// *****************************

void parse_command(Command *com, Files *file_names, char *str)
{
}

int main()
{
    char *str = "/merge bill_1.txt bill_2.txt out.txt price";
    char buff[200];
    char command[30];
    char msg[30];
    sscanf(str, "%[^' '] %[^\n]", command, msg);
    printf("%s \n%s", command, msg);
}