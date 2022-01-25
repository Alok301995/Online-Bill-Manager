#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SIZE 30000;

// ************ Recieve from client*********
void _process_client_request(char *buff)
{
    FILE *fp = fopen("out.txt", "w");
    int count = fwrite(buff, sizeof(char), strlen(buff), fp);
    printf("write count %d", count);
    if (count == 30000)
    {
        printf("error writing file");

        bzero(buff, 30000);
        fclose(fp);
        return;
    }
    printf("write success\n");
    fclose(fp);
    bzero(buff, 30000);
    return;
}

// *****************************************

// ***************Server***********************
// ********************************************
int main(int argc, char *argv[])
{
    int PORT = atoi(argv[1]);
    int sockfd, newfd, len;
    struct sockaddr_in addr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&addr, sizeof(addr));

    // assign IP, PORT
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    if ((bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);
    newfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (newfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
    char buff[30000];
    int count = 0;
    while (1)
    {
        printf("server %d\n", count);
        bzero(buff, 30000);
        int n = read(newfd, buff, sizeof(buff));
        if (n < 0)
        {
            printf("error reading input from client");
        }

        if (strcmp(buff, "/sort") == 0)
        {
            bzero(buff, 30000);
            read(newfd, buff, sizeof(buff));
            _process_client_request(buff);
            bzero(buff, 30000);
            snprintf(buff, 30000, "%s", "success");
            write(newfd, buff, sizeof(buff));
        }

        count++;
    }
    close(newfd);
    close(sockfd);
}
