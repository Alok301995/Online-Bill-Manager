#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SIZE 30000;

typedef struct parsed_info
{
    char com[100];
    FILE *file_1;
    FILE *file_2;
    FILE *out_file;
    char field[30];
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
                    sprintf(rec->com, "%s", command);
                    sprintf(rec->field, "%s", field);
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
                    sprintf(rec->com, "%s", command);
                    sprintf(rec->field, "%s", field);
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
                sprintf(rec->com, "%s", command);
                sprintf(rec->field, "%s", "");
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

// *************************************************
void _send_to_server(Parse_info *info, int fd, char *buff, int buff_size)
{
    // send command to sever
    bzero(buff, 30000);
    snprintf(buff, 30000, "%s", info->com);
    write(fd, buff, sizeof(buff));
    bzero(buff, 30000);
    if (strcmp(info->com, "/sort") == 0)
    {

        bzero(buff, buff_size);
        int count = fread(buff, sizeof(char), buff_size, info->file_1);
        printf("%d", count);
        if (count == 30000 && fgetc(info->file_1) != EOF)
        {
            printf("file size exceeded\n");
            bzero(buff, 30000);
            fclose(info->file_1);
            free(info);
            return;
        }
        printf("buff at client %s \n", buff);
        fclose(info->file_1);
        free(info);
        return;
    }
    else
    {
        return;
    }
    return;
}

// ***********************************************************************************************
int main(int argc, char *argv[])
{
    int PORT = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in addr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }

    printf("Socket successfully created..\n");
    bzero(&addr, sizeof(addr));
    struct hostent *server = gethostbyname("localhost");
    addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&addr.sin_addr.s_addr, server->h_length);
    addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    char buffer[30000];
    int n;
    char ch;
    while (1)
    {
        bzero(buffer, sizeof(buffer));
        printf("Enter Command: ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n')
            ;
        Parse_info *parsed = _parse_str(buffer);
        bzero(buffer, 30000);
        if (parsed == NULL)
        {
            printf("error \n");
        }
        else
        {
            _send_to_server(parsed, sockfd, buffer, 30000);
            write(sockfd, buffer, sizeof(buffer));
            bzero(buffer, 30000);
            read(sockfd, buffer, sizeof(buffer));
            printf("%s", buffer);
        }
    }

    // write(sockfd, buffer, sizeof(buffer));
    // bzero(buffer, sizeof(buffer));
    close(sockfd);
}