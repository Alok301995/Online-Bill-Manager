#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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
    char buffer[max_size];
    int n;
    // Reading intial msg from server

    bzero(buffer, max_size);
    int r = read(sockfd, buffer, sizeof(buffer));
    printf("%s", buffer);
    if (r == 0)
    {
        close(sockfd);
        exit(1);
    }
    bzero(buffer, 1024);
    while (1)
    {
        printf("[COM:]");
        while ((buffer[n++] = getchar()) != '\n')
            ;
        printf("%s", buffer);
        write(sockfd, buffer, sizeof(buffer));
        sleep(2);
        bzero(buffer, 1024);
    }

    close(sockfd);
}