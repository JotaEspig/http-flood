#include <stdio.h>

// Gets the banner of the tool
char *get_banner() {
    return  "= = = = = = = = = = = = = = = = = = = =\n" \
            "              HTTP Flood               \n" \
            "= = = = = = = = = = = = = = = = = = = =\n" ;
}

int main() {
    char *banner = get_banner();
    printf("Começo do projeto\n\n%s\n", banner);
    return 0;
}
