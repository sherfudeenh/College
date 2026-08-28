#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define MAX 1024
#define TIMEOUT 2

int main()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    char request[MAX];
    char response[MAX];
    char ack[50];

    int sequence = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        int choice, id;

        printf("\n--- LIBRARY MENU ---\n");
        printf("1. Search Book\n");
        printf("2. Issue Book\n");
        printf("3. Return Book\n");
        printf("4. View Available Books\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 5)
            break;

        id = 0;

        if (choice != 4)
        {
            printf("Enter Book ID: ");
            scanf("%d", &id);
        }

        if (choice == 1)
            sprintf(request, "%d|SEARCH|%d", sequence, id);
        else if (choice == 2)
            sprintf(request, "%d|ISSUE|%d", sequence, id);
        else if (choice == 3)
            sprintf(request, "%d|RETURN|%d", sequence, id);
        else if (choice == 4)
            sprintf(request, "%d|VIEW|0", sequence);
        else
        {
            printf("Invalid choice.\n");
            continue;
        }

        /*
           Send request until ACK is received.
        */
        while (1)
        {
            sendto(sockfd, request, strlen(request) + 1, 0,
                   (struct sockaddr *)&server, len);

            fd_set set;
            struct timeval timeout;

            FD_ZERO(&set);
            FD_SET(sockfd, &set);

            timeout.tv_sec = TIMEOUT;
            timeout.tv_usec = 0;

            if (select(sockfd + 1, &set, NULL, NULL, &timeout) > 0)
            {
                recvfrom(sockfd, ack, sizeof(ack), 0,
                         (struct sockaddr *)&server, &len);

                if (strncmp(ack, "ACK", 3) == 0)
                    break;
            }
            else
            {
                printf("Timeout! Retransmitting...\n");
            }
        }

        /*
           Receive server response.
        */
        recvfrom(sockfd, response, MAX, 0,
                 (struct sockaddr *)&server, &len);

        printf("\nServer Response:\n%s\n", response);

        /*
           ACK the server response.
        */
        sprintf(ack, "ACK|%d", sequence);

        sendto(sockfd, ack, strlen(ack) + 1, 0,
               (struct sockaddr *)&server, len);

        sequence++;
    }

    close(sockfd);
    return 0;
}
