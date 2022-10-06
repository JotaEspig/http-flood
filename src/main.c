#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attacker/attacker.h"


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


int main(int argc, char *argv[]) {
    char *host;
    int port, num_attacks;
    port = 80;
    num_attacks = 100000000;
    if (argc <= 1) {
        print_help();
        exit(1);
    }

    host = (char *)malloc(sizeof(argv[1]));
    strcpy(host, argv[1]);

    if (argc > 2) {
        port = atoi(argv[2]);
    }
    if (argc > 3) {
        num_attacks = atoi(argv[3]);
    }

    print_banner();
    printf("host: %s\nport: %d\nNumber of attacks: %d\n", host, port, num_attacks);

    attack(host, port);

    free((void *)host);
    return 0;
}
