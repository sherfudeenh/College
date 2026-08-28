#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8082
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    char buffer[MAX];
    char result[MAX];

    float a, b;
    char operation;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        printf("\n--- CALCULATOR ---\n");

        printf("Enter first number: ");
        scanf("%f", &a);

        printf("Enter operation (+ - * /): ");
        scanf(" %c", &operation);

        printf("Enter second number: ");
        scanf("%f", &b);

        if (operation == '/' && b == 0)
        {
            printf("Cannot divide by zero.\n");
            continue;
        }

        sprintf(buffer, "%.2f %.2f %c",
                a, b, operation);

        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&server, len);

        recvfrom(sockfd, result, MAX, 0,
                 (struct sockaddr *)&server, &len);

        printf("Server: %s\n", result);

        printf("\nDo you want another operation? (y/n): ");

        char choice;
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N')
            break;
    }

    close(sockfd);

    return 0;
}
