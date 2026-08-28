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
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char buffer[MAX];
    char result[MAX];

    float a, b, answer;
    char operation;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Calculator Server Started...\n");

    while (1)
    {
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&client, &len);

        /*
           Format:
           number1 number2 operation
        */

        sscanf(buffer, "%f %f %c",
               &a, &b, &operation);

        switch (operation)
        {
            case '+':
                answer = a + b;
                sprintf(result, "Result = %.2f", answer);
                break;

            case '-':
                answer = a - b;
                sprintf(result, "Result = %.2f", answer);
                break;

            case '*':
                answer = a * b;
                sprintf(result, "Result = %.2f", answer);
                break;

            case '/':
                if (b == 0)
                {
                    strcpy(result,
                           "Error: Division by zero.");
                }
                else
                {
                    answer = a / b;
                    sprintf(result,
                            "Result = %.2f", answer);
                }
                break;

            default:
                strcpy(result, "Invalid operation.");
        }

        sendto(sockfd, result, strlen(result) + 1, 0,
               (struct sockaddr *)&client, len);
    }

    return 0;
}
