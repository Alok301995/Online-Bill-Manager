#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct parsed_info
{
    char com[20];
    FILE *file[3];
    char field[20];
    int _n;

} Parse_info;

Parse_info *_allocate_mem()
{
    Parse_info *rec = (Parse_info *)malloc(sizeof(Parse_info));
    return rec;
}

// No. of line
int _NLINEX(FILE *file)
{
    int _number_of_lines = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            _number_of_lines++;
        }
    }
    if (ftell(file) > 0)
    {
        _number_of_lines = _number_of_lines + 1;
        return _number_of_lines;
    }
    return _number_of_lines;
}

// *************

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

                FILE *file = fopen(file_name, "r+");
                if (file == NULL)
                {
                    return NULL;
                }
                else
                {
                    Parse_info *rec = _allocate_mem();
                    sprintf(rec->com, "%s", command);
                    sprintf(rec->field, "%s", field);
                    rec->file[0] = file;
                    rec->file[1] = NULL;
                    rec->file[2] = file;
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
                FILE *file_1 = fopen(file_name_1, "r+");
                FILE *file_2 = fopen(file_name_2, "r+");
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
                    rec->file[0] = file_1;
                    rec->file[1] = file_2;
                    rec->file[2] = out_f;
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
                rec->file[0] = file_1;
                rec->file[1] = file_2;
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
    // sender the the parsed info to the server
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
    int max_size = 1024;
    int file_buff_size = 65000;
    char buffer[max_size];
    char file_buff[file_buff_size];
    int n;
    char ch;
    while (1)
    {
        bzero(buffer, max_size);
        printf("\n[+]Enter Command: ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n')
            ;
        Parse_info *parse = _parse_str(buffer);
        if (parse == NULL)
        {
            printf("Incorrect Command");
            continue;
        }
        else
        {

            bzero(buffer, 1024);
            sprintf(buffer, "%s %s", parse->com, parse->field);
            write(sockfd, buffer, sizeof(buffer));
            bzero(buffer, 1024);
            read(sockfd, buffer, sizeof(buffer));
            if (strcmp(buffer, "1") == 0)
            {
                // prepare the files for transfer
                bzero(buffer, 1024);
                sprintf(buffer, "%d", parse->_n);
                write(sockfd, buffer, max_size);
                bzero(buffer, 1024);
                read(sockfd, buffer, sizeof(buffer));
                bzero(buffer, 1024);
                read(sockfd, buffer, sizeof(buffer));
                if (strcmp(buffer, "1") == 0)
                {
                    for (int i = 0; i < parse->_n; i++)
                    {
                        bzero(buffer, 1024);
                        if (parse->file[i] != NULL)
                        {
                            int _n_lines = _NLINEX(parse->file[i]);
                            rewind(parse->file[i]);
                            int count = 0;
                            while (count < _n_lines)
                            {
                                bzero(buffer, 1024);
                                fgets(buffer, 1024, parse->file[i]);
                                write(sockfd, buffer, sizeof(buffer));
                                bzero(buffer, 1024);
                                read(sockfd, buffer, sizeof(buffer));
                                if (strcmp(buffer, "1") == 0)
                                {
                                    bzero(buffer, 1024);
                                    count++;
                                    continue;
                                }
                            }
                            bzero(buffer, 1024);
                            sprintf(buffer, "%d", 1);
                            write(sockfd, buffer, sizeof(buffer));
                            bzero(buffer, 1024);
                        }
                    }
                }
                // Prepare for result

                read(sockfd, buffer, sizeof(buffer));
                if (strcmp(buffer, "1") == 0)
                {
                    bzero(buffer, 1024);
                    sprintf(buffer, "%d", 1);
                    write(sockfd, buffer, sizeof(buffer));
                    bzero(buffer, 1024);
                    FILE *f = fopen("out_cl.txt", "w");
                    while (1)
                    {
                        read(sockfd, buffer, sizeof(buffer));
                        if (strcmp(buffer, "1") == 0)
                        {
                            // for (int i = 0; i < 2; i++)
                            // {
                            //     if (parse->file[i] != NULL)
                            //     {
                            //         fclose(parse->file[i]);
                            //     }
                            // }
                            fclose(f);
                            break;
                        }
                        fputs(buffer, f);
                        bzero(buffer, 1024);
                        sprintf(buffer, "%d", 1);
                        write(sockfd, buffer, sizeof(buffer));
                    }
                    bzero(buffer, 1024);
                }

                // reciving result
            }
            else
            {
                bzero(buffer, 1024);
                sprintf(buffer, "%d", -1);
                write(sockfd, buffer, max_size);
                continue;
            }
        }
    }

    close(sockfd);
}