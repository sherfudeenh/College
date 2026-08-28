#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8083
#define MAX 1024

struct Student
{
    char regno[20];
    char name[50];
    char department[30];
    int semester;
    float cgpa;
};

struct Student students[] =
{
    {"23CS001", "Arun", "CSE", 6, 8.5},
    {"23CS002", "Bala", "CSE", 6, 9.0},
    {"23IT001", "Kumar", "IT", 4, 8.2},
    {"23EC001", "Rahul", "ECE", 5, 7.8}
};

int main()
{
    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char regno[MAX];
    char response[MAX];

    int total = sizeof(students) /
                sizeof(students[0]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server,
         sizeof(server));

    printf("Student Server Started...\n");

    while (1)
    {
        recvfrom(sockfd, regno, MAX, 0,
                 (struct sockaddr *)&client, &len);

        int found = 0;

        for (int i = 0; i < total; i++)
        {
            if (strcmp(regno, students[i].regno) == 0)
            {
                sprintf(response,
                        "Registration No: %s\n"
                        "Name: %s\n"
                        "Department: %s\n"
                        "Semester: %d\n"
                        "CGPA: %.2f",
                        students[i].regno,
                        students[i].name,
                        students[i].department,
                        students[i].semester,
                        students[i].cgpa);

                found = 1;
                break;
            }
        }

        if (!found)
        {
            strcpy(response,
                   "Error: Student not found.");
        }

        sendto(sockfd, response,
               strlen(response) + 1, 0,
               (struct sockaddr *)&client, len);
    }

    close(sockfd);

    return 0;
}
