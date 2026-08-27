#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket, client_socket;
    char message[100];

    struct sockaddr_in server, client;
    socklen_t client_size;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind socket
    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    // Wait for client
    listen(server_socket, 5);

    printf("Echo Server waiting for client...\n");

    // Accept client
    client_size = sizeof(client);
    client_socket = accept(server_socket,
                           (struct sockaddr *)&client,
                           &client_size);

    // Receive message
    recv(client_socket, message, sizeof(message), 0);

    printf("Client sent: %s\n", message);

    // Send same message back
    send(client_socket, message, strlen(message) + 1, 0);

    printf("Message echoed to client.\n");

    close(client_socket);
    close(server_socket);

    return 0;
}
