/*
 * server.c
 * Simple authenticated TCP chat server.
 *
 * Flow:
 *   1. Create socket, bind, listen, accept one client.
 *   2. Receive username + password, check credentials.
 *   3. Send "SUCCESS" or "FAILED".
 *   4. If successful, loop: receive a message from client, print it,
 *      then read a reply from the server's own console and send it back.
 *   5. Exit cleanly when client sends "exit" or disconnects.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char username[50];
    char password[50];
    ssize_t bytes;

    /* 1. Create TCP socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket Created Successfully\n");

    /* Allow quick restart of the server on the same port (avoids
       "Address already in use" errors while testing/re-running) */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* 2. Bind socket to address/port */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }
    printf("Bind Successful\n");

    /* 3. Listen for incoming connections */
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }
    printf("Server Listening on Port %d...\n", PORT);
    fflush(stdout);

    /* 4. Accept a client connection (blocks until one arrives) */
    client_socket = accept(server_fd, NULL, NULL);
    if (client_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }
    printf("Client Connected\n");
    fflush(stdout);

    /* 5. Receive credentials. Clear buffers first so leftover
          bytes can't corrupt the string comparison. */
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));

    bytes = recv(client_socket, username, sizeof(username) - 1, 0);
    if (bytes <= 0) {
        printf("Client disconnected before sending username\n");
        close(client_socket);
        close(server_fd);
        return 1;
    }

    bytes = recv(client_socket, password, sizeof(password) - 1, 0);
    if (bytes <= 0) {
        printf("Client disconnected before sending password\n");
        close(client_socket);
        close(server_fd);
        return 1;
    }

    /* 6. Check credentials (hardcoded for coursework demo purposes) */
    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        send(client_socket, "SUCCESS", strlen("SUCCESS") + 1, 0);
        printf("Authentication Successful\n");
    } else {
        send(client_socket, "FAILED", strlen("FAILED") + 1, 0);
        printf("Authentication Failed\n");
        close(client_socket);
        close(server_fd);
        return 0;
    }
    fflush(stdout);

    /* 7. Chat loop */
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            /* 0 = client closed connection, <0 = error */
            printf("Client Disconnected\n");
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Client Disconnected\n");
            break;
        }

        printf("Client: %s\n", buffer);

        printf("Server: ");
        fflush(stdout);              /* make sure prompt shows before fgets blocks */

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        send(client_socket, buffer, strlen(buffer) + 1, 0);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(client_socket);
    close(server_fd);

    return 0;
}
