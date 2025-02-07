#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_DOTS (2)
#define MAX_DOTS (7)

#define DELIM " "

#define CIRCLE "\u25CF"

#define DEFAULT "\033[0m"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define TERMINAL "\033[1m"

#define PROSET "\033[1;31mP\033[1;32mr\033[1;33mo\033[1;34mS\033[1;35me\033[1;36mt\033[0m"

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
 * https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
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
        if (card & (1 << i)) {
            printf("%s%s", COLORS[i], CIRCLE);
        } else {
            printf(" ");
        }
    }
    printf("%s", DEFAULT);
}

/*
 * prints the cards currently on the table as given by the table array
 */
void print_table(uint8_t* table_array, int table_size) {
    for (int i = 0; i < table_size; i++) {
        printf("[%d] ", i + 1);
        print_card(table_array[i]);
        printf("\n");
    }
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
                printf("Usage: ./proset [options]\n\n"
                       "This program allows you to play ProSet on the command line!\n\n"
                       "Optional Arguments:\n"
                       "%-15s shows help message\n"
                       "%-15s specify number of dots on each card (between 2 and 8 inclusive)\n", 
                       "-h", "-d NUM_DOTS");
                exit(0);
                break;
            case 'd': // specify number of dots on each card
                // get pointer to null terminator of operation argument (necessary for strtol)
                num = strtol(optarg, &optarg_endptr, 10);

                // handle strtol errors and input errors
                if (optarg == optarg_endptr || 
                    *optarg_endptr != '\0' || 
                    num < MIN_DOTS || 
                    num > MAX_DOTS) {
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

    printf("Initializing %s...\n", PROSET);

    // set the number of cards 
    num_cards = (1 << num_dots) - 1;
    num_table = num_dots + 1;

    uint8_t deck[num_cards];
    uint8_t table[num_table];
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

    int score = 0;

    printf("- type %s\"help\"%s for instructions on how to play.\n"
           "- type %s\"table\"%s to see the cards currently on the table.\n"
           "- type %s\"score\"%s for your current score.\n"
           "- type %s\"exit\"%s to end the game.\n",
           PURPLE, DEFAULT, YELLOW, DEFAULT, CYAN, DEFAULT, RED, DEFAULT);


    print_table(table, num_table);

    // main control loop
    while (1) {
        printf("\033[0;32m>>>%s ", DEFAULT);

        char *line = NULL;
        size_t len = 0;
        ssize_t characters = getline(&line, &len, stdin);

        if (characters == -1) { // ssize_t is signed so we can compare to -1
            fprintf(stderr, "There was an error reading the input or EOF was reached. Aborting.\n");
            if (line) {
                free(line);
            }
            exit(1);

        } else if (characters == 5 && strncmp("exit", line, 4) == 0) {
            printf("Thank you for playing %s! You ended with a score of %s%d%s.\n", PROSET, TERMINAL, score, DEFAULT);
            free(line);
            exit(0);

        } else if (characters == 5 && strncmp("help", line, 4) == 0) {
            printf("The goal of ProSet is to find \"sets\" of cards. A collection "
                   "of cards is a set if the number of dots of each color is "
                   "even (including zero).\n");
            printf("To play, List the numbers of the cards you believe form a "
                   "set separated by spaces. For example, if you think cards "
                   "4, 1, and 2 are a set, type \"4 1 2\" (note that the "
                   "order does not matter).\n");
            free(line);
            continue;

        } else if (characters == 6 && strncmp("score", line, 5) == 0) {
            printf("Your current score is %s%d%s.\n", TERMINAL, score, DEFAULT);
            free(line);
            continue;
        }
        
        // parse the card numbers 
        char *num_str = strtok(line, DELIM);
        uint8_t sum = 0;

        // selected_cards stores the numbers (not indices) of the cards on the
        // table selected by the user
        int selected_cards[num_table];
        int selected_cards_size = 0;

        int invalid_input = 0;

        while (num_str != NULL) {
            char *num_str_endptr = NULL;
            long card_num = strtol(num_str, &num_str_endptr, 10);

            if ((*num_str_endptr != '\0' && *num_str_endptr != '\n') ||
                card_num < 1 ||
                card_num > num_table) {
                printf("An invalid input was provided. Please try again.\n");
                invalid_input = 1; // ensure the sum is not zero before
                break;
            }

            // check that the card has not been selected previously
            for (int i = 0; i < selected_cards_size; i++) {
                if (selected_cards[i] == (int) card_num) {
                    printf("The same card was chosen twice. Please try again.\n");
                    invalid_input = 1;
                    break;
                }
            }
            if (invalid_input == 1) {
                break;
            }

            selected_cards[selected_cards_size] = (int) card_num;
            selected_cards_size++;

            sum = sum ^ table[card_num - 1];

            num_str = strtok(NULL, DELIM);
        }

        if (!invalid_input && sum > 0) { // valid input but cards do not form a set
            printf("The cards you selected are not a set. Please try again.\n");

        } else { // cards form a set
            score += selected_cards_size;

            switch (randint(4)) {
                case 0:
                    printf("%sNice job!%s ", RED, DEFAULT);
                    break;
                case 1:
                    printf("%sYee haw!%s ", GREEN, DEFAULT);
                    break;
                case 2:
                    printf("%sCool beans!%s ", BLUE, DEFAULT);
                    break;
                case 3:
                    printf("%sHooray!%s ", PURPLE, DEFAULT);
                    break;
                case 4:
                    printf("%sWowza!%s ", CYAN, DEFAULT);
                    break;
            }
            printf("You found a set! Your score is now %s%d%s.\n", TERMINAL, score, DEFAULT);

            // check that end of deck is not reached
            int last_index = next_card_index + selected_cards_size - 1;
            if (last_index >= num_cards) { // end of deck is reached
                shuffle(deck, num_cards);
                next_card_index = 0;

                // reset the table
                for (int i = 0; i < num_table; i++) {
                    table[i] = deck[i];
                    next_card_index++;
                }
            } else {
                for (int i = 0; i < selected_cards_size; i++) {
                    table[selected_cards[i] - 1] = deck[next_card_index];
                    next_card_index++;
                }
            }

            print_table(table, num_table);
        }

        free(line);
    }

    return 0;
}
