#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv) {
    int port, num_attacks;
    port = 80;
    num_attacks = 100000000;
    if (argc < 2) {
        print_help();
        exit(1);
    }
    printf("host: %s\n", argv[1]);
    print_banner();
    return 0;
}
