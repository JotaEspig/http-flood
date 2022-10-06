#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "attacker.h"


// Sends the payload
static void send_payload(int sock_fd) {
    char payload[] = "GET /attack HTTP/1.1\nHost: \n\n";
    // TODO Make payload be a random path instead of just "attack"
    write(sock_fd, payload, sizeof(payload));
}


// Does the request to the web server
void attack(char *host, char *ip, int16_t port) {
    int sock_fd, i;
    struct sockaddr_in target_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    target_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock_fd, (struct sockaddr *)&target_addr, sizeof target_addr)
        != 0) {
        printf("Error connecting to the address: %s\n", host);
        exit(1);
    }

    send_payload(sock_fd);

    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}
