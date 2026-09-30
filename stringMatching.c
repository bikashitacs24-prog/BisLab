#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define POP_SIZE 50
#define MUTATION_RATE 0.15 // 15% chance to mutate a character

int main() {
    const char* TARGET = "SECRET123";
    int len = strlen(TARGET);
    char pop[POP_SIZE][30]; // Array to hold our population of strings
    char best_str[30];
    int best_fitness = 0, gen = 0;

    srand(time(NULL));

    // 1. Initialize: Fill the population with random uppercase letters and numbers
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < len; j++) {
            pop[i][j] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[rand() % 36];
        }
        pop[i][len] = '\0';
    }

    // 2. Main Evolution Loop
    while (best_fitness < len) {
        gen++;
        best_fitness = -1;

        // Step A: Evaluate Fitness (Count exact matching characters)
        for (int i = 0; i < POP_SIZE; i++) {
            int current_fitness = 0;
            for (int j = 0; j < len; j++) {
                if (pop[i][j] == TARGET[j]) current_fitness++;
            }

            // Keep track of the absolute best string found so far
            if (current_fitness > best_fitness) {
                best_fitness = current_fitness;
                strcpy(best_str, pop[i]);
            }
        }

        // Print progress
        printf("Generation %d | Best Guess: [%s] | Progress: %d/%d\n", gen, best_str, best_fitness, len);
        if (best_fitness == len) break; // Stop if we guessed it perfectly

        // Step B: Create Next Generation based on the best string
        for (int i = 0; i < POP_SIZE; i++) {
            for (int j = 0; j < len; j++) {
                // If random roll hits the mutation rate, change the character. 
                // Otherwise, copy the character from the best string.
                if (((double)rand() / RAND_MAX) < MUTATION_RATE) {
                    pop[i][j] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[rand() % 36];
                } else {
                    pop[i][j] = best_str[j];
                }
            }
            pop[i][len] = '\0';
        }
    }

    printf("\n🎉 Target matched successfully in %d generations!\n", gen);
    return 0;
}
