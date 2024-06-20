/*
 * Zack Puhl, 04727275
 *   June 19, 2024
 *   CIS-579: Artificial Intelligence
 *
 * Main program file.
 */

#include "genetic.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


static int _run()
{
    float avgGeneration = 0.0;
    float withCrossover = 0.0;
    float noCrossover = 0.0;

    srand(time(0U));
    printf("\n\n=== Genetic Algorithm Program ===\n");


    printf("\n\n>> WITH CROSSOVER AT 0.7...\n");
    for (int i = 0; i < MAX_GENERATIONS; ++i) {
        /* Population 50, GenomeLength 10, Crossover 0.7, Mutation 0.001 */
        avgGeneration += runGA(50, 10, 0.7, 0.001);
    }

    /* Get the average terminal generation of all 30 runs. */
    avgGeneration /= (float)MAX_GENERATIONS;
    withCrossover = avgGeneration;
    printf("---> Average terminal generation: %f\n\n", withCrossover);

    printf("\n\n>> WITHOUT CROSSOVER...\n");
    avgGeneration = 0.0;
    for (int j = 0; j < MAX_GENERATIONS; ++j) {
        /* Same thing, but no crossover. */
        avgGeneration += runGA(50, 10, 0.0, 0.001);
    }

    avgGeneration /= (float)MAX_GENERATIONS;
    noCrossover = avgGeneration;
    printf("---> Average terminal generation: %f\n\n", noCrossover);


    printf("\n\nCROSSOVER: %f   ///   NONE: %f\n", withCrossover, noCrossover);
    printf("csvme:Crossover,%f\n", withCrossover);
    printf("csvme:None,%f\n", noCrossover);
    printf("\n\n=== ALL DONE ===\n\n");
    return 0;
}


int main()
{
    printf("csvme:Type,Generations Taken\n");

    for (int i = 0; i < 100; ++i)
        _run();

    return 0;
}