#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    int guess;
    char buffer[50];
    while (1) {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        int net_guess = htonl(guess);
        send(sock, &net_guess, sizeof(net_guess), 0);

        recv(sock, buffer, 50, 0);
        printf("Server: %s\n", buffer);
        if (strcmp(buffer, "Correct!") == 0) break;
    }

    close(sock);
    return 0;
}
