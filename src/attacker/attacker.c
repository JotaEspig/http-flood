#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "attacker.h"


// Generates the payload to send to the server
static char *generate_payload() {
    // TODO Make payload be a random path instead of just "attack"
    return "GET /attack HTTP/1.1\nHost: \n\n";
}


// Sends the payload
static void send_payload(int sock_fd, char *payload) {
    if (payload == NULL) {
        payload = generate_payload();
    }
    write(sock_fd, payload, strlen(payload));
}


// Does the request to the web server
void attack(char *host, char *ip, int16_t port, char *payload) {
    int sock_fd, i;
    struct sockaddr_in target_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    target_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock_fd, (struct sockaddr *)&target_addr, sizeof target_addr)
        != 0) {
        return;
    }

    send_payload(sock_fd, payload);

    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}
