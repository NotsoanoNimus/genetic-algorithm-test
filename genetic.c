/*
 * Zack Puhl, 04727275
 *   June 19, 2024
 *   CIS-579: Artificial Intelligence
 *
 * Implementation file for genetic algorithm methods.
 */

#include "genetic.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Allocate a random genome of the specified length. */
genome_t *randomGenome(unsigned int length)
{
    if (length > MAX_GENOME_LENGTH) return NULL;

    genome_t *g = (genome_t *)calloc(1, sizeof(genome_t));
    g->length = length;
    g->chromosome = rand() & ((1 << length) - 1);

    return g;
}


/* Allocate a 'size' population of genomes at 'length'. */
population_t *makePopulation(unsigned int size,
                             unsigned int genomeLength)
{
    if (size > MAX_POPULATION_SIZE) return NULL;

    population_t *p = (population_t *)calloc(1, sizeof(population_t));
    p->size = size;

    for (int i = 0; i < size; ++i) {
        genome_t *g = randomGenome(genomeLength);
        memcpy(&(p->genomes[i]), g, sizeof(genome_t));
        free(g);
    }

    return p;
}


/* Return the fitness value of the genome. */
fitness_t fitness(genome_t *genome)
{
    /* Use Brian Kernighan's algorithm to determine the
       number of '1' bits. */
    fitness_t f = 0;

    unsigned int c = genome->chromosome & ((1 << genome->length) - 1);

    while (c) {
        c &= (c - 1);
        f++;
    }

    return f;
}


/* Return the fitness metric of a population. */
fitness_metric_t evaluateFitness(population_t *population)
{
    fitness_metric_t m = {0};

    for (int i = 0; i < population->size; ++i) {
        fitness_t f = fitness(&(population->genomes[i]));

        m.best = f > m.best ? f : m.best;
        m.whole += f;
    }

    m.whole /= (float)population->size;

    return m;
}


/* Select a genome pair by fitness-proportionate selection. */
genome_pair_t selectPair(population_t *population)
{
    genome_pair_t pair = {0};
    fitness_t totalFitness = 0.0;
    float cumulativeProbability = 0.0;
    float someRand = 0.0;

    /* Gather the cumulative fitness of the entire population. */
    for (int i = 0; i < population->size; i++)
        totalFitness += fitness(&(population->genomes[i]));

    /* Spin the roulette wheel! */
    /* A probability accumulates here over time according to fitness level
       to give all genomes a chance to be selected while preferring high fitness. */
    someRand = (float)rand() / RAND_MAX;
    for (int j = 0; j < population->size; j++) {
        cumulativeProbability += fitness(&(population->genomes[j])) / totalFitness;
        if (someRand < cumulativeProbability) {
            pair.left = &(population->genomes[j]);
            break;
        }
    }

    /* Reset the wheel and spin it again! */
    /* Two parents should never be the same (that would be weird...). */
    pair.right = &(population->genomes[0]);
    someRand = (float)rand() / RAND_MAX;
    cumulativeProbability = 0;
    for (int k = 0; k < population->size; k++) {
        cumulativeProbability += fitness(&(population->genomes[k])) / totalFitness;
        if (pair.left != &(population->genomes[k])
                && (someRand < cumulativeProbability
                || k == population->size - 1)) {
            pair.right = &(population->genomes[k]);
            break;
        }
    }

    return pair;
}


/* Crossover between two genomes at a random point. */
void crossover(genome_pair_t pair)
{
    /* The crossover point is randomly selected in the length of the genome. */
    int genomeLength = 10;
    int crossoverPoint = (rand() % genomeLength) + 1;

    /* Each genome inherits the other parents' details at the crossover point. */
    unsigned int left = 0;
    unsigned int right = 0;

    left |= (pair.left->chromosome & ((1 << crossoverPoint) - 1));
    left |= (pair.right->chromosome & ~((1 << crossoverPoint) - 1));

    right |= (pair.right->chromosome & ((1 << crossoverPoint) - 1));
    right |= (pair.left->chromosome & ~((1 << crossoverPoint) - 1));

    pair.left->chromosome = left;
    pair.right->chromosome = right; 
}


/* Mutates a genome. */
void mutate(genome_t *genome,
            float mutationRate)
{
    for (int i = 0; i < genome->length; i++) {
        if (((float)rand() / RAND_MAX) >= mutationRate) continue;
        genome->chromosome ^= (1 << i); /* mutations flip the bit */
    }
}


/* Run the genetic algorithm for a population with a
   crossover rate and a mutation rate. */
unsigned int runGA(unsigned int populationSize,
                   unsigned int genomeLength,
                   float crossoverRate,
                   float mutationRate)
{
    unsigned int generation = 1;
    population_t *p = makePopulation(populationSize, genomeLength);

    printf("\tRunning simulation with crossover '%f' and mutation rate '%f'\n",
        crossoverRate, mutationRate);

    for (; generation <= MAX_GENERATIONS; generation++) {
        for (int i = 0; i < populationSize; i++) {
            mutate(&(p->genomes[i]), mutationRate);

            if (crossoverRate > 0.0
                    && ((float)rand() / RAND_MAX) < crossoverRate) {
                genome_pair_t somePair = selectPair(p);
                crossover(somePair);
            }
        }

        fitness_metric_t popFit = evaluateFitness(p);
        
        printf("\t\tGeneration %02d:\tavg fitness '%f'\tbest '%f'\n",
            generation, popFit.whole, popFit.best);

        if (popFit.best >= 10) break;
    }

    free(p);
    printf("\n\n");
    return generation > 30 ? 30 : generation;
}
