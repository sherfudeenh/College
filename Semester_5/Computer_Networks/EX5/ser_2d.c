#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_ROWS 10
#define COLS 7

int getParity(int count)
{
    return count % 2;
}

int main(int argc, char *argv[])
{
    int server_fd, client_fd;

    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    char data[MAX_ROWS][COLS + 1];
    int rowParity[MAX_ROWS];
    int colParity[COLS];

    int rows;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // -------------------------------
    // 1. Create TCP socket
    // -------------------------------
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    // -------------------------------
    // 2. Server address
    // -------------------------------
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // -------------------------------
    // 3. Bind socket
    // -------------------------------
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    // -------------------------------
    // 4. Listen
    // -------------------------------
    listen(server_fd, 1);

    printf("TCP 2D Parity Server waiting for client...\n");

    // -------------------------------
    // 5. Accept ONE client
    // -------------------------------
    len = sizeof(client_addr);

    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &len);

    if (client_fd < 0)
    {
        perror("accept");
        return 1;
    }

    printf("Client connected.\n");

    // -------------------------------
    // 6. Receive number of rows
    // -------------------------------
    recv(client_fd,
         &rows,
         sizeof(rows),
         0);

    // -------------------------------
    // 7. Receive binary data
    // -------------------------------
    recv(client_fd,
         data,
         sizeof(data),
         0);

    // -------------------------------
    // 8. Receive row parity
    // -------------------------------
    recv(client_fd,
         rowParity,
         sizeof(rowParity),
         0);

    // -------------------------------
    // 9. Receive column parity
    // -------------------------------
    recv(client_fd,
         colParity,
         sizeof(colParity),
         0);

    printf("\n========== RECEIVED DATA ==========\n");

    for (int i = 0; i < rows; i++)
    {
        printf("Row %d: ", i + 1);

        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", data[i][j]);
        }

        printf("| Parity = %d\n", rowParity[i]);
    }

    printf("-------------------------------\n");

    printf("Column Parity: ");

    for (int j = 0; j < COLS; j++)
    {
        printf("%d ", colParity[j]);
    }

    printf("\n");

    // -------------------------------
    // 10. Check Row Parity
    // -------------------------------
    int errorRow = -1;

    for (int i = 0; i < rows; i++)
    {
        int count = 0;

        for (int j = 0; j < COLS; j++)
        {
            if (data[i][j] == '1')
                count++;
        }

        int calculatedParity = getParity(count);

        if (calculatedParity != rowParity[i])
        {
            errorRow = i;

            printf("\nRow parity error at Row %d\n",
                   i + 1);
        }
    }

    // -------------------------------
    // 11. Check Column Parity
    // -------------------------------
    int errorCol = -1;

    for (int j = 0; j < COLS; j++)
    {
        int count = 0;

        for (int i = 0; i < rows; i++)
        {
            if (data[i][j] == '1')
                count++;
        }

        int calculatedParity = getParity(count);

        if (calculatedParity != colParity[j])
        {
            errorCol = j;

            printf("Column parity error at Column %d\n",
                   j + 1);
        }
    }

    // -------------------------------
    // 12. Detect and Correct Error
    // -------------------------------
    if (errorRow != -1 && errorCol != -1)
    {
        printf("\nERROR DETECTED!\n");

        printf("Error located at Row %d, Column %d\n",
               errorRow + 1,
               errorCol + 1);

        printf("Original bit: %c\n",
               data[errorRow][errorCol]);

        // Flip the bit
        if (data[errorRow][errorCol] == '0')
            data[errorRow][errorCol] = '1';
        else
            data[errorRow][errorCol] = '0';

        printf("Corrected bit: %c\n",
               data[errorRow][errorCol]);

        printf("\nError successfully corrected.\n");
    }
    else if (errorRow == -1 && errorCol == -1)
    {
        printf("\nNo error detected.\n");
    }
    else
    {
        printf("\nUnresolvable parity error.\n");
    }

    // -------------------------------
    // 13. Display corrected matrix
    // -------------------------------
    printf("\n========== FINAL DATA ==========\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", data[i][j]);
        }

        printf("\n");
    }

    // -------------------------------
    // 14. Close sockets
    // -------------------------------
    close(client_fd);
    close(server_fd);

    return 0;
}