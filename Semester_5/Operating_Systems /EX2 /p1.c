#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    pid_t pid;
    int num, temp;

    // Create Pipe
    if (pipe(fd) == -1)
    {
        perror("Pipe Failed");
        return 1;
    }

    // Create Child Process
    pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed");
        return 1;
    }

    // Child Process
    if (pid == 0)
    {
        close(fd[0]);   // Close read end

        printf("Child Process\n");
        printf("Enter a number: ");
        scanf("%d", &num);

        write(fd[1], &num, sizeof(num));

        close(fd[1]);   // Close write end
        exit(0);
    }

    // Parent Process
    else
    {
        close(fd[1]);   // Close write end

        read(fd[0], &num, sizeof(num));

        printf("\nParent Process\n");
        printf("Received Number = %d\n", num);

        temp = num;

        if (num <= 0)
        {
            printf("%d is NOT a Power of 3\n", temp);
        }
        else
        {
            while (num % 3 == 0)
            {
                num = num / 3;
            }

            if (num == 1)
                printf("%d is a Power of 3\n", temp);
            else
                printf("%d is NOT a Power of 3\n", temp);
        }

        close(fd[0]);   // Close read end
        wait(NULL);
    }

    return 0;
}
