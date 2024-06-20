/*
 * Zack Puhl, 04727275
 *   June 19, 2024
 *   CIS-579: Artificial Intelligence
 *
 * Header file for genetic algorithm methods.
 */

#define MAX_GENOME_LENGTH   32
#define MAX_POPULATION_SIZE 64
#define MAX_GENERATIONS     30


/* Simple typedef for fitness. */
typedef
float
fitness_t;


/* Create a fixed-length genome structure of a max size,
   but including a length for the genome itself. */
typedef
struct _genome
{
    unsigned int length;
    unsigned int chromosome;
} __attribute__((packed)) genome_t;

/* Data structure representing a population.
   A group of genomes. */
typedef
struct _population
{
    unsigned int size;
    genome_t genomes[MAX_POPULATION_SIZE];
} __attribute__((packed)) population_t;


/* A grouping of fitness values for a population and its best. */
typedef
struct _fitness_metric
{
    fitness_t whole;
    fitness_t best;
} __attribute__((packed)) fitness_metric_t;


/* A grouping of two fitness-proportionate genomes. */
typedef
struct _genome_pair
{
    genome_t *left;
    genome_t *right;
} __attribute__((packed)) genome_pair_t;


/* Allocate a random genome of the specified length. */
genome_t *randomGenome(
    unsigned int length
);


/* Allocate a 'size' population of genomes at 'length'. */
population_t *makePopulation(
    unsigned int size,
    unsigned int genomeLength
);


/* Return the fitness value of the genome. */
fitness_t fitness(
    genome_t *genome
);


/* Return the fitness metric of a population. */
fitness_metric_t evaluateFitness(
    population_t *population
);


/* Select a genome pair by fitness-proportionate selection. */
genome_pair_t selectPair(
    population_t *population
);


/* Crossover between two genomes at a random point. */
void crossover(
    genome_pair_t pair
);


/* Mutates a genome. */
void mutate(
    genome_t *genome,
    float mutationRate
);


/* Run the genetic algorithm for a population with a
   crossover rate and a mutation rate. */
unsigned int runGA(
    unsigned int populationSize,
    unsigned int genomeLength,
    float crossoverRate,
    float mutationRate
);
