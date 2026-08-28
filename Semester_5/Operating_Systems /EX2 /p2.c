#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;

    char str[100];
    char status[20];

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }

    // Parent Process
    if (pid > 0)
    {
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2

        printf("Parent Process\n");
        printf("Enter a string: ");
        scanf("%s", str);

        // Send string to child
        write(pipe1[1], str, sizeof(str));
        close(pipe1[1]);

        // Read status from child
        read(pipe2[0], status, sizeof(status));

        printf("\nParent Process\n");
        printf("Status received from Child : %s\n", status);

        if(strcmp(status, "VALID") == 0)
            printf("The entered string is VALID.\n");
        else
            printf("The entered string is INVALID.\n");

        close(pipe2[0]);
        wait(NULL);
    }

    // Child Process
    else
    {
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2

        read(pipe1[0], str, sizeof(str));

        int hasDigit = 0;
        int hasSpecial = 0;
        int len = strlen(str);

        for(int i = 0; str[i] != '\0'; i++)
        {
            if(isdigit(str[i]))
                hasDigit = 1;

            else if(!isalpha(str[i]))
                hasSpecial = 1;
        }

        if(len > 7 && hasDigit && hasSpecial)
            strcpy(status, "VALID");
        else
            strcpy(status, "INVALID");

        write(pipe2[1], status, sizeof(status));

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    return 0;
}
