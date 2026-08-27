#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket, client_socket;
    char word[100];
    char result[100];

    struct sockaddr_in server, client;
    socklen_t client_size;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind socket
    bind(server_socket,
         (struct sockaddr *)&server,
         sizeof(server));

    // Listen
    listen(server_socket, 5);

    printf("Palindrome Server waiting for client...\n");

    // Accept client
    client_size = sizeof(client);

    client_socket = accept(server_socket,
                            (struct sockaddr *)&client,
                            &client_size);

    // Receive word
    recv(client_socket, word, sizeof(word), 0);

    printf("Word received: %s\n", word);

    // Remove newline
    word[strcspn(word, "\n")] = '\0';

    // Check palindrome
    int i = 0;
    int j = strlen(word) - 1;
    int palindrome = 1;

    while (i < j)
    {
        if (word[i] != word[j])
        {
            palindrome = 0;
            break;
        }

        i++;
        j--;
    }

    if (palindrome)
        strcpy(result, "Palindrome");
    else
        strcpy(result, "Not Palindrome");

    // Send result
    send(client_socket, result, strlen(result) + 1, 0);

    close(client_socket);
    close(server_socket);

    return 0;
}
