#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int client_socket;
    char message[100];

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

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    // Send message
    send(client_socket, message, strlen(message) + 1, 0);

    // Receive echoed message
    recv(client_socket, message, sizeof(message), 0);

    printf("Echo from server: %s\n", message);

    close(client_socket);

    return 0;
}
