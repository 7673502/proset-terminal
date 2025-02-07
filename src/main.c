#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>


int NUM_DOTS = 6; // default is 6
int NUM_CARDS = (1 << (6 + 1)) - 1;

/*
 * helper function to exit when incorrect argument is specified with -d option
 */
void print_d_usage() {
    fprintf(stderr, "Option -d requires an integer argument between 2 and 8 inclusive.\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    int opt;
    char *optarg_endptr = NULL;
    long num;

    // parse command line arguments
    while ((opt = getopt(argc, argv, "d:h")) != -1) {
        switch(opt) {
            case 'h': // print help message
                printf("help!\n");
                exit(0);
                break;
            case 'd': // specify number of dots on each card
                // get pointer to null terminator of operation argument (necessary for strtol)
                num = strtol(optarg, &optarg_endptr, 10);

                // handle strtol errors
                if (optarg == optarg_endptr || *optarg_endptr != '\0' || NUM_DOTS < 2 || NUM_DOTS > 8) {
                    print_d_usage();
                }
                
                NUM_DOTS = (int) num;

                break;
            case '?':
                if (optopt == 'd') {
                    print_d_usage();
                }
                break;
            default:
                exit(1);
                break;
        }
    }

    return 0;
}
