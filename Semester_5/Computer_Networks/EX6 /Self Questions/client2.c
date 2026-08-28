#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8081
#define MAX 1024
#define TIMEOUT 2

int checksum(char *str)
{
    int sum = 0;

    while (*str)
    {
        sum += *str;
        str++;
    }

    return sum % 1000;
}

int main()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    char buffer[MAX];
    char ack[MAX];

    int sequence = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        int choice;

        printf("\n--- WEATHER CLIENT ---\n");
        printf("1. Send Weather Data\n");
        printf("2. Get Weather Statistics\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 3)
            break;

        if (choice == 1)
        {
            char location[50];
            float temp, humidity, pressure, wind;
            char dataWithoutChecksum[MAX];
            int check;

            printf("Enter location: ");
            scanf("%s", location);

            printf("Temperature: ");
            scanf("%f", &temp);

            printf("Humidity: ");
            scanf("%f", &humidity);

            printf("Pressure: ");
            scanf("%f", &pressure);

            printf("Wind Speed: ");
            scanf("%f", &wind);

            sprintf(dataWithoutChecksum,
                    "DATA|%s|%d|%.2f|%.2f|%.2f|%.2f",
                    location, sequence,
                    temp, humidity, pressure, wind);

            check = checksum(dataWithoutChecksum);

            sprintf(buffer, "%s|%d",
                    dataWithoutChecksum, check);

            /*
               Retransmission until ACK.
            */
            while (1)
            {
                sendto(sockfd, buffer, strlen(buffer) + 1, 0,
                       (struct sockaddr *)&server, len);

                fd_set set;
                struct timeval timeout;

                FD_ZERO(&set);
                FD_SET(sockfd, &set);

                timeout.tv_sec = TIMEOUT;
                timeout.tv_usec = 0;

                if (select(sockfd + 1, &set,
                           NULL, NULL, &timeout) > 0)
                {
                    recvfrom(sockfd, ack, MAX, 0,
                             (struct sockaddr *)&server, &len);

                    if (strncmp(ack, "ACK", 3) == 0)
                        break;
                }
                else
                {
                    printf("Timeout! Retransmitting...\n");
                }
            }

            printf("Weather data sent successfully.\n");

            sequence++;
        }

        else if (choice == 2)
        {
            char location[50];

            printf("Enter location: ");
            scanf("%s", location);

            sprintf(buffer, "QUERY|%s", location);

            sendto(sockfd, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&server, len);

            recvfrom(sockfd, buffer, MAX, 0,
                     (struct sockaddr *)&server, &len);

            printf("\n--- WEATHER STATISTICS ---\n");
            printf("%s\n", buffer);
        }

        else
        {
            printf("Invalid choice.\n");
        }
    }

    close(sockfd);

    return 0;
}
