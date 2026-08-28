#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8083
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    char regno[MAX];
    char response[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        printf("\n--- STUDENT LOOKUP ---\n");

        printf("Enter Registration Number: ");
        scanf("%s", regno);

        if (strcmp(regno, "exit") == 0)
            break;

        sendto(sockfd, regno, strlen(regno) + 1, 0,
               (struct sockaddr *)&server, len);

        recvfrom(sockfd, response, MAX, 0,
                 (struct sockaddr *)&server, &len);

        printf("\n--- SERVER RESPONSE ---\n");
        printf("%s\n", response);
    }

    close(sockfd);

    return 0;
}
