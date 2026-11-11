#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", port);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        char *client_ip = inet_ntoa(client_addr.sin_addr);
        printf("%s: Client connected\n", client_ip);

        srand(time(NULL));
        int secret = rand() % 100 + 1;
        int guess;
        
        while (1) {
            recv(client_fd, &guess, sizeof(guess), 0);
            guess = ntohl(guess);
            printf("%s: Guessing %d\n", client_ip, guess);

            char response[50];
            if (guess < secret) strcpy(response, "Higher");
            else if (guess > secret) strcpy(response, "Lower");
            else { strcpy(response, "Correct!"); send(client_fd, response, strlen(response)+1, 0); break; }
            
            send(client_fd, response, strlen(response)+1, 0);
        }
        printf("%s: Disconnected\n", client_ip);
        close(client_fd);
    }
    return 0;
}
