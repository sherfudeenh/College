[?1034h[24bcs181@mepcolinux EX1]$cat 1a.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t p2;

    // Root process (P1)
    printf("P1 (Root Process)\n");
    printf("PID        : %d\n", getpid());
    printf("Parent PID : %d\n\n", getppid());

    // Create child process P2
    p2 = fork();

    if (p2 < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if (p2 == 0)
    {
        // Child process (P2)
        printf("P2 (Child of P1)\n");
        printf("PID        : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());
    }
    else
    {
        // Parent waits until child finishes
        wait(NULL);
    }

    return 0;
}
[24bcs181@mepcolinux EX1]$cc 1a.c
[24bcs181@mepcolinux EX1]$./a.out
P1 (Root Process)
PID        : 13052
Parent PID : 12803

P2 (Child of P1)
PID        : 13053
Parent PID : 13052
[24bcs181@mepcolinux EX1]$cat 1b.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t p2, p3;

    // Root process (P1)
    printf("P1 (Root Process)\n");
    printf("PID        : %d\n", getpid());
    printf("Parent PID : %d\n\n", getppid());

    // Create P2
    p2 = fork();

    if (p2 < 0)
    {
        perror("First fork failed");
        return 1;
    }
    else if (p2 == 0)
    {
        // Child process P2
        printf("P2 (Child of P1)\n");
        printf("PID        : %d\n", getpid());
        printf("Parent PID : %d\n\n", getppid());

        // Create P3
        p3 = fork();

        if (p3 < 0)
        {
            perror("Second fork failed");
            return 1;
        }
        else if (p3 == 0)
        {
            // Child process P3
            printf("P3 (Child of P2)\n");
            printf("PID        : %d\n", getpid());
            printf("Parent PID : %d\n", getppid());
        }
        else
        {
            // P2 waits for P3
            wait(NULL);
        }
    }
    else
    {
        // P1 waits for P2
        wait(NULL);
    }

    return 0;
}
[24bcs181@mepcolinux EX1]$cc 1b.c
[24bcs181@mepcolinux EX1]$./a.out
P1 (Root Process)
PID        : 13479
Parent PID : 12803

P2 (Child of P1)
PID        : 13480
Parent PID : 13479

P3 (Child of P2)
PID        : 13481
Parent PID : 13480
[24bcs181@mepcolinux EX1]$cat 1c.c
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
   pid_t p2,p3,p4;
   printf("P1 (Root Process) \n");
   printf("PID : %d\n",getpid());
   printf("Parent PID: %d\n\n",getppid());
   p2=fork();
   if(p2<0)
   {
      perror("First fork failed");
      return 1;
   }
   else if(p2==0)
   {
      printf("P2 (Child of P1) \n");
      printf("PID: %d\n",getpid());
      printf("Parent PID : %d\n\n",getppid());
      p3=fork();
      if (p3<0)
      {
         perror("Second fork failed");
         return 1;
      }
      else if(p3==0)
      {
         printf("P3 (Child of P2) \n");
         printf("PID : %d\n",getpid());
         printf("Parent PID: %d\n",getppid());
      }
      else
      {
       wait(NULL);
      }
   }
   else
   {
     wait(NULL);
     p4=fork();
     if(p4<0)
     {
       perror("Third fork failed");
       return 1;
     }
     else if(p4==0)
     {
       printf("P4 (Child of P1) \n");
       printf("PID: %d\n",getpid());
       printf("Parent PID : %d\n",getppid());
     }
     else
     {
        wait(NULL);
     }
   }
         return 0;
}



[24bcs181@mepcolinux EX1]$cc 1c.c
[24bcs181@mepcolinux EX1]$./a.out
P1 (Root Process)
PID : 13673
Parent PID: 12803

P2 (Child of P1)
PID: 13674
Parent PID : 13673

P3 (Child of P2)
PID : 13675
Parent PID: 13674
P4 (Child of P1)
PID: 13676
Parent PID : 13673
[24bcs181@mepcolinux EX1]$cat 3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int n, i, sum = 0;
    pid_t pid;

    printf("Enter a positive integer: ");
    scanf("%d", &n);

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("\n----- Child Process -----\n");

        for (i = 1; i <= n; i++)
        {
            sum += i;
        }

        printf("Sum of first %d natural numbers = %d\n", n, sum);
    }
    else
    {
        // Parent waits for child to finish
        wait(NULL);

        printf("\n----- Parent Process -----\n");
        printf("Collatz Sequence: ");

        while (n != 1)
        {
            printf("%d ", n);

            if (n % 2 == 0)
                n = n / 2;
            else
                n = 3 * n + 1;
        }

        printf("1\n");
    }

    return 0;
}
[24bcs181@mepcolinux EX1]$cc 3.c
[24bcs181@mepcolinux EX1]$./a.out
Enter a positive integer: 6

----- Child Process -----
Sum of first 6 natural numbers = 21

----- Parent Process -----
Collatz Sequence: 6 3 10 5 16 8 4 2 1
[24bcs181@mepcolinux EX1]$cat own.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t p2, p3, p4, p5, p6, p7, p8;

    // P1: Root Process
    printf("P1 (Root Process) \n");
    printf("PID : %d\n", getpid());
    printf("Parent PID: %d\n\n", getppid());

    // Fork P2 from P1
    p2 = fork();
    if (p2 < 0) {
        perror("Fork for P2 failed");
        return 1;
    }
    else if (p2 == 0) {
        // Inside P2 (Child of P1)
        printf("P2 (Child of P1) \n");
        printf("PID: %d\n", getpid());
        printf("Parent PID : %d\n\n", getppid());

        // Fork P4 from P2
        p4 = fork();
        if (p4 < 0) {
            perror("Fork for P4 failed");
            return 1;
        }
        else if (p4 == 0) {
            // Inside P4 (Child of P2)
            printf("P4 (Child of P2) \n");
            printf("PID: %d\n", getpid());
            printf("Parent PID : %d\n\n", getppid());

            // Fork P8 from P4
            p8 = fork();
            if (p8 < 0) {
                perror("Fork for P8 failed");
                return 1;
            }
            else if (p8 == 0) {
                // Inside P8 (Child of P4)
                printf("P8 (Child of P4) \n");
                printf("PID: %d\n", getpid());
                printf("Parent PID : %d\n\n", getppid());
                return 0;
            }
            else {
                wait(NULL); // P4 waits for P8
            }
            return 0;
        }
        else {
            wait(NULL); // P2 waits for P4
        }

        // Fork P5 from P2
        p5 = fork();
        if (p5 < 0) {
            perror("Fork for P5 failed");
            return 1;
        }
        else if (p5 == 0) {
            // Inside P5 (Child of P2)
            printf("P5 (Child of P2) \n");
            printf("PID: %d\n", getpid());
            printf("Parent PID : %d\n\n", getppid());
            return 0;
        }
        else {
            wait(NULL); // P2 waits for P5
        }
        return 0;
    }
    else {
        wait(NULL); // P1 waits for P2 branch to complete
    }

    // Fork P3 from P1
    p3 = fork();
    if (p3 < 0) {
        perror("Fork for P3 failed");
        return 1;
    }
    else if (p3 == 0) {
        // Inside P3 (Child of P1)
        printf("P3 (Child of P1) \n");
        printf("PID: %d\n", getpid());
        printf("Parent PID : %d\n\n", getppid());

        // Fork P7 from P3
        p7 = fork();
        if (p7 < 0) {
            perror("Fork for P7 failed");
            return 1;
        }
        else if (p7 == 0) {
            // Inside P7 (Child of P3)
            printf("P7 (Child of P3) \n");
            printf("PID: %d\n", getpid());
            printf("Parent PID : %d\n\n", getppid());
            return 0;
        }
        else {
            wait(NULL); // P3 waits for P7
        }

        // Fork P6 from P3
        p6 = fork();
        if (p6 < 0) {
            perror("Fork for P6 failed");
            return 1;
        }
        else if (p6 == 0) {
            // Inside P6 (Child of P3)
            printf("P6 (Child of P3) \n");
            printf("PID: %d\n", getpid());
            printf("Parent PID : %d\n\n", getppid());
            return 0;
        }
        else {
            wait(NULL); // P3 waits for P6
        }
        return 0;
    }
    else {
        wait(NULL); // P1 waits for P3 branch to complete
    }

    return 0;
}

[24bcs181@mepcolinux EX1]$cc own.c
[24bcs181@mepcolinux EX1]$./a.out
P1 (Root Process)
PID : 14672
Parent PID: 12803

P2 (Child of P1)
PID: 14673
Parent PID : 14672

P4 (Child of P2)
PID: 14674
Parent PID : 14673

P8 (Child of P4)
PID: 14675
Parent PID : 14674

P5 (Child of P2)
PID: 14676
Parent PID : 14673

P3 (Child of P1)
PID: 14677
Parent PID : 14672

P7 (Child of P3)
PID: 14678
Parent PID : 14677

P6 (Child of P3)
PID: 14679
Parent PID : 14677

[24bcs181@mepcolinux EX1]$exit
