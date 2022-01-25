#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// *********Global **********

typedef struct _connected_client
{
    int client_fd[5];
    int con_client;
    int MAX_CLIENT;

} Client_info;

// *********************************

int main()
{
    int master_sock;
    int cd;
    int max_cd;
    int port = 6000;
    int addrlen;
    char buffer[1024];

    struct sockaddr_in addr;
    struct sockaddr_in cli_addr;
    // Initializing Client_fds structure

    Client_info *connected_client = (Client_info *)malloc(sizeof(Client_info));

    connected_client->MAX_CLIENT = 5;
    connected_client->con_client = 0;
    for (int i = 0; i < 5; i++)
    {
        connected_client->client_fd[i] = 0;
    }
    // Intialize fdsets
    fd_set readfds;
    FD_ZERO(&readfds);
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    // check whether socket is created or not
    if (master_sock < 0)
    {
        printf("socket creation failed\n");
        exit(1);
    }
    else
        printf("master socket created successfuly\n");

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the sock with the port;
    if (bind(master_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Bind failed\n");
        exit(1);
    }
    else
        printf("Bind success\n");

    if (listen(master_sock, 3) < 0)
    {
        printf("error listening to sockets\n");
        exit(1);
    }
    else
        printf("listening\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(master_sock, &readfds);
        max_cd = master_sock;

        for (int i = 0; i < 5; i++)
        {
            if (connected_client->client_fd[i] > 0)
            {
                FD_SET(connected_client->client_fd[i], &readfds);
                if (connected_client->client_fd[i] > max_cd)
                {
                    max_cd = connected_client->client_fd[i];
                }
            }
        }

        // select system call
        printf("[-->]");
        int s = select(max_cd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(master_sock, &readfds))
        {
            // check the no. of client connected if no. of client connected is greater than MAX client
            // reject it with the msg
            // else conneect it using accept system call and register the client to client_fd
            addrlen = sizeof(cli_addr);
            int confd = accept(master_sock, (struct sockaddr *)&cli_addr, &addrlen);
            if (confd < 0)
            {
                printf("error \n");
                exit(EXIT_FAILURE);
            }
            if (connected_client->con_client >= connected_client->MAX_CLIENT)
            {
                // open connection with new client
                // and send a msg that connection rejected
                bzero(buffer, 1024);
                sprintf(buffer, "%s", "Connection Rejected \n");
                write(confd, buffer, sizeof(buffer));
                close(confd);
            }
            else
            {
                bzero(buffer, 1024);
                sprintf(buffer, "%s", "Welcome to Online Bill Manager\n");
                write(confd, buffer, sizeof(buffer));
                bzero(buffer, 1024);
                for (int i = 0; i < 5; i++)
                {
                    if (connected_client->client_fd[i] == 0)
                    {
                        connected_client->client_fd[connected_client->con_client] = confd;
                        break;
                    }
                }
                connected_client->con_client += 1;
            }
        }
        for (int i = 0; i < connected_client->MAX_CLIENT; i++)
        {
            if (FD_ISSET(connected_client->client_fd[i], &readfds))
            {
                int n = read(connected_client->client_fd[i], buffer, sizeof(buffer));
                if (n == 0)
                {
                    // client disconneted
                    printf("CLIENT\n");
                    close(connected_client->client_fd[i]);
                    // update client structure
                    connected_client->client_fd[i] = 0;
                    connected_client->con_client -= 1;
                }
                else
                {
                    bzero(buffer, 1024);
                    read(connected_client->client_fd[i], buffer, sizeof(buffer));
                    puts(buffer);
                    bzero(buffer, 1024);
                }
            }
        }
    }
}