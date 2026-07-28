/*
 * client.c
 * Simple authenticated TCP chat client.
 *
 * Flow:
 *   1. Create socket and connect to server.
 *   2. Send username + password.
 *   3. Receive "SUCCESS" or "FAILED" from server.
 *   4. If successful, loop: send a message, then wait for the
 *      server's reply and print it. Typing "exit" ends the chat.
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
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char username[50];
    char password[50];
    ssize_t bytes;

    /* 1. Create TCP socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket Created Successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return 1;
    }

    /* 2. Connect to server */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }
    printf("Connected to Server\n");

    /* 3. Get credentials from user. Flush after each prompt since
          printf without '\n' may otherwise stay in the output buffer. */
    printf("Username: ");
    fflush(stdout);
    scanf("%49s", username);

    printf("Password: ");
    fflush(stdout);
    scanf("%49s", password);

    send(sock, username, strlen(username) + 1, 0);
    send(sock, password, strlen(password) + 1, 0);

    /* 4. Wait for server's authentication response */
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        printf("Server closed connection unexpectedly\n");
        close(sock);
        return 1;
    }

    if (strcmp(buffer, "SUCCESS") != 0) {
        printf("Authentication Failed\n");
        close(sock);
        return 0;
    }

    printf("Authentication Successful\n");

    /* Consume the leftover newline left in stdin by scanf,
       so it doesn't get read immediately by fgets below */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    /* 5. Chat loop */
    while (1) {
        printf("\nClient: ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        send(sock, buffer, strlen(buffer) + 1, 0);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            printf("Server Disconnected\n");
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Server ended the chat.\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
