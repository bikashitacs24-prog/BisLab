#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PARTICLES 5     // Small swarm size so it's easy to track
#define TASKS 3         // Task 1, Task 2, Task 3
#define ITERATIONS 50   // Number of times the swarm looks for a better answer

int main() {
    srand(time(NULL));

    // 1. Swarm Memory Arrays
    double position[PARTICLES][TASKS];
    double velocity[PARTICLES][TASKS];
    
    double pbest_position[PARTICLES][TASKS]; // Best position found by EACH particle
    double pbest_fitness[PARTICLES];         // Best profit found by EACH particle
    
    double gbest_position[TASKS];            // Best position found by the WHOLE swarm
    double gbest_fitness = -1.0;             // Start at -1 because we want to maximize profit

    // --- STEP 1: INITIALIZE THE SWARM ---
    for (int p = 0; p < PARTICLES; p++) {
        double sum = 0.0;
        
        // Give random resource amounts to each task
        for (int t = 0; t < TASKS; t++) {
            position[p][t] = (double)(rand() % 50 + 1); 
            velocity[p][t] = 0.0; // Start particles at rest
            sum += position[p][t];
        }
        
        // Normalize: Scale the random numbers so they add up to exactly 100 units
        for (int t = 0; t < TASKS; t++) {
            position[p][t] = (position[p][t] / sum) * 100.0;
            pbest_position[p][t] = position[p][t]; // First position is its initial best
        }

        // Calculate Profit: Profit = 10*sqrt(t1) + 15*sqrt(t2) + 12*sqrt(t3)
        pbest_fitness[p] = (10.0 * sqrt(position[p][0])) + 
                           (15.0 * sqrt(position[p][1])) + 
                           (12.0 * sqrt(position[p][2]));

        // Check if this particle has the best configuration in the entire swarm
        if (pbest_fitness[p] > gbest_fitness) {
            gbest_fitness = pbest_fitness[p];
            for (int t = 0; t < TASKS; t++) {
                gbest_position[t] = position[p][t];
            }
        }
    }

    // --- STEP 2: THE PSO MAIN LOOP ---
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int p = 0; p < PARTICLES; p++) {
            
            double sum = 0.0;

            // Update Velocity and Position for each task
            for (int t = 0; t < TASKS; t++) {
                double r1 = (double)rand() / RAND_MAX; // Random number between 0 and 1
                double r2 = (double)rand() / RAND_MAX;

                // The Core PSO Formula: inertia + cognitive (self) + social (group)
                velocity[p][t] = (0.7 * velocity[p][t]) + 
                                 (1.5 * r1 * (pbest_position[p][t] - position[p][t])) + 
                                 (1.5 * r2 * (gbest_position[t] - position[p][t]));

                position[p][t] += velocity[p][t];
                
                if (position[p][t] < 0) position[p][t] = 0; // Prevent negative resources
                sum += position[p][t];
            }

            // Re-normalize positions so they still total exactly 100 units
            if (sum == 0) sum = 1.0;
            for (int t = 0; t < TASKS; t++) {
                position[p][t] = (position[p][t] / sum) * 100.0;
            }

            // Calculate new profit for the adjusted positions
            double current_fitness = (10.0 * sqrt(position[p][0])) + 
                                     (15.0 * sqrt(position[p][1])) + 
                                     (12.0 * sqrt(position[p][2]));

            // Update Personal Best if the new profit is higher
            if (current_fitness > pbest_fitness[p]) {
                pbest_fitness[p] = current_fitness;
                for (int t = 0; t < TASKS; t++) {
                    pbest_position[p][t] = position[p][t];
                }
            }

            // Update Global Best if this is the best profit seen by anyone so far
            if (current_fitness > gbest_fitness) {
                gbest_fitness = current_fitness;
                for (int t = 0; t < TASKS; t++) {
                    gbest_position[t] = position[p][t];
                }
            }
        }
    }

    // --- STEP 3: PRINT THE FINAL ANSWER ---
    printf("--- Optimal Resource Allocation ---\n");
    printf("Task 1 Allocation: %.2f units\n", gbest_position[0]);
    printf("Task 2 Allocation: %.2f units\n", gbest_position[1]);
    printf("Task 3 Allocation: %.2f units\n", gbest_position[2]);
    printf("Total Profit Achieved: %.4f\n", gbest_fitness);

    return 0;
}
