#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081
#define MAX 1024

struct Weather
{
    char location[50];

    float temperature;
    float humidity;
    float pressure;
    float wind;

    int count;
    int lastSeq;
};

struct Weather data[20];
int totalLocations = 0;

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

int findLocation(char *location)
{
    int i;

    for (i = 0; i < totalLocations; i++)
    {
        if (strcmp(data[i].location, location) == 0)
            return i;
    }

    strcpy(data[totalLocations].location, location);

    data[totalLocations].count = 0;
    data[totalLocations].lastSeq = 0;

    return totalLocations++;
}

int main()
{
    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Weather Server Started...\n");

    while (1)
    {
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&client, &len);

        /*
           DATA format:
           DATA|location|seq|temperature|humidity|pressure|wind|checksum

           QUERY|location
        */

        if (strncmp(buffer, "DATA", 4) == 0)
        {
            char location[50];
            int seq, check, calculated;
            float temp, hum, pressure, wind;

            sscanf(buffer,
                   "DATA|%[^|]|%d|%f|%f|%f|%f|%d",
                   location, &seq,
                   &temp, &hum, &pressure,
                   &wind, &check);

            /*
               Calculate checksum again.
            */
            char tempBuffer[MAX];

            sprintf(tempBuffer,
                    "DATA|%s|%d|%.2f|%.2f|%.2f|%.2f",
                    location, seq,
                    temp, hum, pressure, wind);

            calculated = checksum(tempBuffer);

            if (calculated != check)
            {
                sendto(sockfd, "ERROR", 6, 0,
                       (struct sockaddr *)&client, len);

                printf("Error detected.\n");
                continue;
            }

            int index = findLocation(location);

            /*
               Detect duplicate/missing packets.
            */
            if (seq == data[index].lastSeq)
            {
                printf("Duplicate packet received.\n");
            }
            else
            {
                if (seq > data[index].lastSeq + 1 &&
                    data[index].lastSeq != 0)
                {
                    printf("Missing packet detected.\n");
                }

                data[index].lastSeq = seq;

                data[index].temperature += temp;
                data[index].humidity += hum;
                data[index].pressure += pressure;
                data[index].wind += wind;

                data[index].count++;
            }

            char ack[50];

            sprintf(ack, "ACK|%d", seq);

            sendto(sockfd, ack, strlen(ack) + 1, 0,
                   (struct sockaddr *)&client, len);
        }

        else if (strncmp(buffer, "QUERY", 5) == 0)
        {
            char location[50];
            int index;

            sscanf(buffer, "QUERY|%s", location);

            index = findLocation(location);

            char response[MAX];

            if (data[index].count == 0)
            {
                sprintf(response, "No weather data available.");
            }
            else
            {
                sprintf(response,
                        "Location: %s\n"
                        "Average Temperature: %.2f\n"
                        "Average Humidity: %.2f\n"
                        "Average Pressure: %.2f\n"
                        "Average Wind Speed: %.2f",
                        location,
                        data[index].temperature / data[index].count,
                        data[index].humidity / data[index].count,
                        data[index].pressure / data[index].count,
                        data[index].wind / data[index].count);
            }

            sendto(sockfd, response, strlen(response) + 1, 0,
                   (struct sockaddr *)&client, len);
        }
    }

    close(sockfd);

    return 0;
}
