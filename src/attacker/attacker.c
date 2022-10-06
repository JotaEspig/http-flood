#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "attacker.h"


// Does the request to the web server
void attack(char *host, int16_t port) {
    int sock_id, client_id;
    struct sockaddr_in target_addr;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    target_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sock_fd, (struct sockaddr *)&target_addr, sizeof target_addr)
        != 0) {
        printf("Error connecting to the address: %s\n", host);
        exit(1);
    }

    printf("SUCESS!\nClosing the connection\n");

    close(sock_fd);
}
