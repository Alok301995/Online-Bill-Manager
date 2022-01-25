#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct parsed_info
{
    char *com;
    FILE *file_1;
    FILE *file_2;
    FILE *out_file;
    char *field;
    int _n;

} Parse_info;

Parse_info *_allocate_mem()
{
    Parse_info *rec = (Parse_info *)malloc(sizeof(Parse_info));
    return rec;
}

// ************Parse Command************************

// Return a structure if command is correct else  retutrn NULL

Parse_info *_parse_str(char *str)
{
    char command[100];
    char msg[100];
    sscanf(str, "%[^' '] %[^\n]", command, msg);

    if (strcmp(command, "/sort") == 0)
    {
        char file_name[100];
        char field[30];
        int _arg_count = sscanf(msg, "%[^' '] %[^\n]", file_name, field);
        if (_arg_count == 2 && strlen(file_name) > 0 && strlen(field) > 0)
        {
            // check for field
            if (strcmp(field, "price") == 0 || strcmp(field, "name") == 0 || strcmp(field, "date") == 0)
            {

                FILE *file = fopen(file_name, "r");
                if (file == NULL)
                {
                    return NULL;
                }
                else
                {
                    Parse_info *rec = _allocate_mem();
                    rec->com = command;
                    rec->field = field;
                    rec->file_1 = file;
                    rec->file_2 = NULL;
                    rec->out_file = NULL;
                    rec->_n = 1;
                    return rec;
                }
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    else if (strcmp(command, "/merge") == 0)
    {
        char file_name_1[100];
        char file_name_2[100];
        char out_file_name[100];
        char field[30];
        int _arg_count = sscanf(msg, "%[^' '] %[^' '] %[^' '] %[^\n]", file_name_1, file_name_2, out_file_name, field);
        if (_arg_count == 4 && strlen(file_name_1) > 0 && strlen(file_name_2) > 0 && strlen(out_file_name) > 0 && strlen(field) > 0)
        {
            if (strcmp(field, "price") == 0 || strcmp(field, "name") == 0 && strcmp(field, "price") == 0)
            {
                FILE *file_1 = fopen(file_name_1, "r");
                FILE *file_2 = fopen(file_name_2, "r");
                FILE *out_f = fopen(out_file_name, "w");
                if (file_1 == NULL || file_2 == NULL)
                {
                    return NULL;
                }
                else
                {
                    Parse_info *rec = _allocate_mem();
                    rec->com = command;
                    rec->field = field;
                    rec->file_1 = file_1;
                    rec->file_2 = file_2;
                    rec->out_file = out_f;
                    rec->_n = 2;
                    return rec;
                }
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    else if (strcmp(command, "/similarity") == 0)
    {
        char file_name_1[100];
        char file_name_2[100];
        int _arg_count = sscanf(msg, "%[^' '] %[^\n]", file_name_1, file_name_2);
        if (_arg_count == 2 && strlen(file_name_1) > 0 && strlen(file_name_2) > 0)
        {
            FILE *file_1 = fopen(file_name_1, "r");
            FILE *file_2 = fopen(file_name_2, "r");
            if (file_1 == NULL || file_2 == NULL)
            {
                return NULL;
            }
            else
            {
                Parse_info *rec = _allocate_mem();
                rec->com = command;
                rec->field = NULL;
                rec->file_1 = file_1;
                rec->file_2 = file_2;
                rec->_n = 0;
                return rec;
            }
        }
        else
        {
            return NULL;
        }
    }
    else if (strcmp(command, "/exit") == 0)
    {
        if (strlen(msg) > 0)
        {
            return NULL;
        }
        // perform exit condition
    }
    else
    {
        return NULL;
    }
}
// ***************************************************

// ***************Send file to server ****************

// ***************************************************

int main()
{

    char *com = "/merge bill_1.txt bill_2.txtda out.txt price";
    Parse_info *parsed = _parse_str(com);
    if (parsed == NULL)
    {
        printf("Invalid command\n");
    }
    else
    {
        printf("valid command\n");
    }
}