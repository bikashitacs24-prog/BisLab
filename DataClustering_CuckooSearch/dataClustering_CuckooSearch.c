#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10        // Number of data points
#define K 2         // Number of clusters
#define NESTS 10    // Population size
#define ITER 50     // Max iterations
#define PA 0.25     // Discovery probability

double data[N] = {1.0, 1.2, 0.8, 1.5, 1.1, 8.0, 8.5, 7.9, 8.2, 8.1};

typedef struct {
    double centers[K];
    double fitness;
} Nest;

// Helper for random double [min, max]
double rnd(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Fitness: Sum of distances to nearest cluster center
double get_fitness(double c[K]) {
    double total = 0;
    for (int i = 0; i < N; i++) {
        double d1 = fabs(data[i] - c[0]);
        double d2 = fabs(data[i] - c[1]);
        total += (d1 < d2) ? d1 : d2;
    }
    return total;
}

int main() {
    srand(time(NULL));
    Nest nests[NESTS], best;
    best.fitness = 1e9;

    // 1. Initialize nests randomly
    for (int i = 0; i < NESTS; i++) {
        for (int k = 0; k < K; k++) nests[i].centers[k] = rnd(0.0, 10.0);
        nests[i].fitness = get_fitness(nests[i].centers);
        if (nests[i].fitness < best.fitness) best = nests[i];
    }

    // 2. Main Cuckoo Search Loop
    for (int t = 0; t < ITER; t++) {
        for (int i = 0; i < NESTS; i++) {
            // Levy flight step
            Nest cuckoo;
            for (int k = 0; k < K; k++) {
                double step = 0.1 * rnd(-1, 1) * pow(rnd(0.01, 1.0), -1.0 / 1.5);
                cuckoo.centers[k] = nests[i].centers[k] + step;
            }
            cuckoo.fitness = get_fitness(cuckoo.centers);

            // Replace a random nest if better
            int j = rand() % NESTS;
            if (cuckoo.fitness < nests[j].fitness) nests[j] = cuckoo;
        }

        // Abandon worse nests with probability PA
        for (int i = 0; i < NESTS; i++) {
            if (rnd(0, 1) < PA) {
                for (int k = 0; k < K; k++) nests[i].centers[k] = rnd(0.0, 10.0);
                nests[i].fitness = get_fitness(nests[i].centers);
            }
            if (nests[i].fitness < best.fitness) best = nests[i];
        }
    }

    printf("Cluster Centers: %.2f, %.2f\n", best.centers[0], best.centers[1]);
    return 0;
}
