#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_DOTS (2)
#define MAX_DOTS (7)

#define CIRCLE "\u25CF"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define TERMINAL "\033[0m"

char* COLORS[] = {RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, TERMINAL};

int num_dots = 6; // default is 6
int num_cards = (1 << (6 + 1)) - 1;
int num_table = 7;


/*
 * helper function to exit when incorrect argument is specified with -d option
 */
void print_d_usage() {
    fprintf(stderr, "Option -d requires an integer argument between 2 and 7 inclusive.\n");
    exit(1);
}

/*
 * helper function to generate random integers between 0 and stop inclusive
 * src: https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
 */
int randint(int stop) {
    int divisor = RAND_MAX/(stop + 1);
    int res;

    do {
        res = rand() / divisor; 
    } while (res > stop);

    return res;
}

/*
 * implementation of modern Fisher-Yates shuffling algorithm; modifies the provided array
 */
void shuffle(uint8_t* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = randint(i);

        // swap values at index i and j
        uint8_t tmp =  array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

/*
 * prints a card with correct colors; does not print a newline
 */
void print_card(uint8_t card) {
    for (int i = 0; i < num_dots; i++) {
        if ((card & (1 << i)) > 0) {
            printf("%s%s", COLORS[i], CIRCLE);
        } else {
            printf(" ");
        }
    }
    printf("%s", TERMINAL);
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

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
                if (optarg == optarg_endptr || *optarg_endptr != '\0' || num < MIN_DOTS || num > MAX_DOTS) {
                    print_d_usage();
                }
                
                num_dots = (int) num;

                break;
            case '?':
                if (optopt == 'd') {
                    print_d_usage();
                } else {
                    fprintf(stderr, "An invalid argument was specified.");
                    exit(1);
                }
                break;
            default:
                exit(1);
                break;
        }
    }

    // set the number of cards 
    num_cards = (1 << num_dots) - 1;
    num_table = num_dots + 1;

    uint8_t deck[num_cards];
    uint8_t table[num_dots + 1];
    int next_card_index = 0;

    // populate the deck
    for (uint8_t i = 0; i < num_cards; i++) { 
        deck[i] = i + 1;
    }

    shuffle(deck, num_cards);

    // populate the table
    for (int i = 0; i < num_table; i++) {
        table[i] = deck[i];
        next_card_index++;
    }

    /*
    for (int i = 0; i < num_table; i++) {
        printf("[%d] ", i + 1);
        print_card(table[i]);
        printf("\n");
    }
    */

    return 0;
}
