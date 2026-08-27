#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int client_socket;
    char word[100];
    char result[100];

    struct sockaddr_in server;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(client_socket,
            (struct sockaddr *)&server,
            sizeof(server));

    printf("Enter a word: ");
    fgets(word, sizeof(word), stdin);

    // Send word
    send(client_socket, word, strlen(word) + 1, 0);

    // Receive result
    recv(client_socket, result, sizeof(result), 0);

    printf("Server: %s\n", result);

    close(client_socket);

    return 0;
}
