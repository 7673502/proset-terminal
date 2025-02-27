# *ProSet* Terminal Edition for Linux

Working on a Linux machine without a GUI and drooping down wearily because you can't play *ProSet*? Well, droop no more! *ProSet* can now be played from the command line!

Getting *ProSet* up and running in your terminal is easy! First, build the *ProSet* executable from `/src/main.c` using your compiler of choice or follow the build instructions at [build/README.md](build/README.md) to build using the provided CMakeLists.txt file.

Once built, you can run *ProSet* using
```bash
./ProSet [options]
```

Below are some of the optional arguments that can be provided to the *ProSet* executable.
```bash
Optional Arguments:
-h              shows help message
-d NUM_DOTS     specify number of uniquely colored dots (between 2 and 8 inclusive)
```
If the `-d` flag is not specified, then the game will default to 6 uniquely colored dots akin to the standard game.

Happy setting!

## How to Play

The game of *ProSet* is conventionally played using a deck of 63 cards with 6 uniquely colored dots. To begin, 7 cards are laid on the table face up. The goal of the game is to find a "set" of cards. A set is a collection of cards with an even number of each colored dot. Every collection of 7 cards is **guaranteed** to contain a set.

*ProSet* Terminal Edition adds additional functionality to play *ProSet* with 2 to 8 uniquely colored dots.

## Why is every collection of 7 cards is guaranteed to have a set?

Specifically, in a game where there are $6$ uniquely colored dots, why is every collection of $7$ cards guaranteed to have a set?

Consider an arbitrary collection of cards. There is always some card that we can add to this collection to get an even number of each colored dot. We will call this card the "complementary card." In the case where the collection already has an even number of each dot, we consider the complementary card to be empty. Importantly, note that collections with the same complementary card form a set when combined. If the combined collection contains the same card twice, then both cards can be removed altogether and the collection will retain the property of being a set.

Now note that in a collection of $7$ cards, we have $2^7$ possible subsets. Because there are $2^6$ possible complementary cards for each subset, there must be at least two subsets with the same complementary card by the pigeonhole principle. As a result, every group of 7 cards is guaranteed to have a set.

An alternative proof is to view the deck of cards as elements of the vector space $\mathbb{F}_2^6$. Then, a "set" corresponds to a collection of elements that sum to $0$. Because every collection of $7$ nonzero elements in $\mathbb{F}_2^6$ is guaranteed to be linearly dependent, there must be some nontrivial relation between the $7$ elements so that a linear combination of them sums to $0$.

Both of these proofs can be applied more generally to show that every collection of $n+1$ cards with $n$ uniquely colored dots is guaranteed to have a set.

## Online Implementations

There are many implementations of *ProSet* available online including:
- A [Scratch implementation](https://scratch.mit.edu/projects/992397812/)
- A [JavaScript implementation](https://www.prosetgame.com/)
- Another [JavaScript implementation](https://www.mathcamp.org/2015/proset/)

You can find various other web implementations and [physical versions](https://www.thegamecrafter.com/games/proset) of *ProSet* at its [Wikipedia page](https://en.wikipedia.org/wiki/Projective_Set_(game)).

## License
[MIT License](LICENSE)
