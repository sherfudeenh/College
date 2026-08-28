#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define MAX 1024

struct Book
{
    int id;
    char name[50];
    int available;
};

void logActivity(char *msg)
{
    FILE *fp;
    time_t t;

    fp = fopen("library_log.txt", "a");
    t = time(NULL);

    fprintf(fp, "%s - %s", msg, ctime(&t));
    fclose(fp);
}

void createDatabase()
{
    FILE *fp = fopen("library.txt", "r");

    if (fp == NULL)
    {
        fp = fopen("library.txt", "w");

        fprintf(fp, "101 C_Programming 1\n");
        fprintf(fp, "102 Computer_Networks 1\n");
        fprintf(fp, "103 Database_System 1\n");
        fprintf(fp, "104 Operating_System 1\n");

        fclose(fp);
    }
}

void showBooks(char *response)
{
    FILE *fp;
    int id, available;
    char name[50];

    fp = fopen("library.txt", "r");

    strcpy(response, "AVAILABLE BOOKS:\n");

    while (fscanf(fp, "%d %s %d", &id, name, &available) == 3)
    {
        if (available == 1)
        {
            char line[100];

            sprintf(line, "ID: %d  Name: %s\n", id, name);
            strcat(response, line);
        }
    }

    fclose(fp);
}

void transaction(int id, int action, char *response)
{
    FILE *fp, *temp;
    int bid, available;
    char name[50];
    int found = 0;

    fp = fopen("library.txt", "r");
    temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d %s %d", &bid, name, &available) == 3)
    {
        if (bid == id)
        {
            found = 1;

            if (action == 1)       /* Issue */
            {
                if (available == 1)
                {
                    available = 0;
                    sprintf(response, "Book %d issued successfully.", id);
                }
                else
                    sprintf(response, "Book is already issued.");
            }
            else                   /* Return */
            {
                if (available == 0)
                {
                    available = 1;
                    sprintf(response, "Book %d returned successfully.", id);
                }
                else
                    sprintf(response, "Book is already available.");
            }
        }

        fprintf(temp, "%d %s %d\n", bid, name, available);
    }

    fclose(fp);
    fclose(temp);

    remove("library.txt");
    rename("temp.txt", "library.txt");

    if (!found)
        sprintf(response, "Book ID not found.");
}

int main()
{
    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char request[MAX];
    char response[MAX];

    createDatabase();

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Library Server Started...\n");

    while (1)
    {
        recvfrom(sockfd, request, MAX, 0,
                 (struct sockaddr *)&client, &len);

        /*
           Request format:
           sequence|operation|bookid
        */

        int seq, bookid;
        char operation[20];

        sscanf(request, "%d|%s|%d",
               &seq, operation, &bookid);

        /*
           Send ACK first.
        */
        char ack[50];
        sprintf(ack, "ACK|%d", seq);

        sendto(sockfd, ack, strlen(ack) + 1, 0,
               (struct sockaddr *)&client, len);

        /*
           Duplicate protection:
           Sequence number identifies a transaction.
        */
        static int lastSeq = -1;

        if (seq == lastSeq)
        {
            strcpy(response, "Duplicate request ignored.");
        }
        else
        {
            lastSeq = seq;

            if (strcmp(operation, "SEARCH") == 0)
            {
                FILE *fp = fopen("library.txt", "r");
                int id, available;
                char name[50];
                int found = 0;

                strcpy(response, "Book not found.");

                while (fscanf(fp, "%d %s %d",
                              &id, name, &available) == 3)
                {
                    if (id == bookid)
                    {
                        sprintf(response,
                                "Book ID: %d\nName: %s\nStatus: %s",
                                id, name,
                                available ? "Available" : "Issued");

                        found = 1;
                    }
                }

                fclose(fp);
            }
            else if (strcmp(operation, "VIEW") == 0)
            {
                showBooks(response);
            }
            else if (strcmp(operation, "ISSUE") == 0)
            {
                transaction(bookid, 1, response);
            }
            else if (strcmp(operation, "RETURN") == 0)
            {
                transaction(bookid, 2, response);
            }
            else
            {
                strcpy(response, "Invalid operation.");
            }

            logActivity(request);
        }

        /*
           Send response.
           Client will ACK the response.
        */
        sendto(sockfd, response, strlen(response) + 1, 0,
               (struct sockaddr *)&client, len);

        printf("Request: %s\n", request);
    }

    close(sockfd);
    return 0;
}
