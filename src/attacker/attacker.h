#ifndef ATTACKER
#define ATTACKER

#define NUM_THREADS 10000


// struct that contains values to be used in thread function (run_100_attacks)
struct thread_args {
    char *host;
    char *ip;
    int16_t port;
    char *payload;
    size_t requests_per_thread;
};


// Run the threads
void run_threads(struct thread_args t_args);

#endif // ATTACKER
