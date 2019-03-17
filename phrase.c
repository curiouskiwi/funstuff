/* Basic Genetic programming example
** Takes a string and number of generations
** and attempts to create a matching string
**
** based on CS50 Twitch stream which was demoed
** in python.  This is my C version
**
** Brenda Anderson 17/3/2019
**/

#include <stdio.h>
#include <math.h>
#include <cs50.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// structure of phrase
typedef struct phrase
{
    int fitness;
    char letters[];
}
PHRASE;

// target string to be matched
char *target;

// debug mutations made
int mutations;

// prototype functions
int choice(int min, int max);
void create_phrase(PHRASE *p, int length);
void calculate_fitness(PHRASE *p);
bool heads();
void crossover(PHRASE *x, PHRASE *y, PHRASE *child, int length);
void mutate(PHRASE *p, int length, int generation);
void colorize(char *s);
void summarize(int gen, char *p, int fit, int len);


int main(void)
{
    // initialise the random seed
    srand(time(NULL));

    // get variables from user
    target = get_string("what's the phrase? ");
    int pop_size = get_int("How many individuals in each generation? ");

    int best_score = 0;
    int generation = 1;
    int len = strlen(target);

    // calculate memory needed for each phrase
    // the struct has an undeclared char array so we add the len
    int phrase_size = sizeof(PHRASE) + len + 1;

    // make sure the struct amount is aligned to 4-byte
    phrase_size = (phrase_size + 3) & ~3;

    // allocate 2 arrays of phrases which will alternate each generation
    char *pop1 = malloc(phrase_size * pop_size);
    char *pop2 = malloc(phrase_size * pop_size);

    // set initial population to first block and parents to second block
    char *population = pop1;
    char *parents = pop2;

    PHRASE *ptr;

    // fill population array with pop_size number of phrases
    for (int i = 0; i < pop_size; i++)
    {
        ptr = (PHRASE *)&population[i * phrase_size];
        create_phrase(ptr, len);
    }

    // create new generations until all chars are matched
    while (best_score < len)
    {
        int totalfitness = 0;
        for (int i = 0; i < pop_size; i++)
        {
            ptr = (PHRASE *)&population[i * phrase_size];
            calculate_fitness(ptr);
            totalfitness += ptr->fitness;


            if (ptr->fitness > best_score)
            {
                best_score = ptr->fitness;
                summarize(generation, ptr->letters, best_score, len);
            }
            else
            {
                // running counter to follow progress
                printf("\r%i", generation);
            }
        }
        // if there were no matching chars in first generation, end program
        if (totalfitness == 0)
        {
            printf("Oops! Try a larger population size!\n");
            return 1;
        }

        //swap population to parents
        char *tmp = population;
        population = parents;
        parents = tmp;

        // use totalfitness to get weighting for parents
        for (int i = 0; i < pop_size; i++)
        {
            // choose parents
            int pick1 = rand() / (RAND_MAX / (totalfitness));
            int pick2 = rand() / (RAND_MAX / (totalfitness));
            int cumulative = 0;
            PHRASE *ptr1 = NULL, *ptr2 = NULL;
            for (int j = 0; j < pop_size; j++)
            {
                ptr = (PHRASE *)&parents[j * phrase_size];
                cumulative += ptr->fitness;
                if (cumulative >= pick1)
                {
                    ptr1 = ptr;
                    // if we've found both parents, break
                    if (ptr2)
                    {
                        break;
                    }
                }
                if (cumulative >= pick2)
                {
                    ptr2 = ptr;
                    // if we've found both parents, break
                    if (ptr1)
                    {
                        break;
                    }
                }
            }

            // put the child into the population array
            PHRASE *child = (PHRASE *)&population[i * phrase_size];
            crossover(ptr1, ptr2, child, len);
            
            // mutate that child (probability based on generation)
            mutate(child, len, generation);
        }
        generation++;
    }
    
    // free the two arrays
    free(pop1);
    free(pop2);

    // debug
    printf(" generations.  mutations: %i  (avg per gen: %f\n", mutations, mutations * 1.0 / generation);

    // done
    return 0;
}



// return a random int between min/max
int choice(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}


// initialize a new phrase struct
void create_phrase(PHRASE *p, int length)
{
    p->fitness = 0;
    for (int i = 0; i < length; i++)
    {
        p->letters[i] = choice(32, 126);
    }
    p->letters[length] = '\0';
}


// takes a pointer to a PHRASE and updates its fitness
void calculate_fitness(PHRASE *p)
{
    if (!target)
    {
        printf("you don't have a target!\n");
        return;
    }

    p->fitness = 0;
    for (int i = 0; p->letters[i]; i++)
    {
        if (p->letters[i] == target[i])
        {
            p->fitness++;
        }
    }
}


// coin toss for parent choice
bool heads()
{
    return (rand() >> 16) & 1;
}


// initialize the child phrase from two parents
void crossover(PHRASE *x, PHRASE *y, PHRASE *child, int length)
{
    for (int i = 0; i < length; i++)
    {
        // randomly assign chars from 2 phrases
        if (heads())
        {
            child->letters[i] = x->letters[i];
        }
        else
        {
            child->letters[i] = y->letters[i];
        }
    }
    child->letters[length] = '\0';
}


// cause a random char in a phrase to be changed
// probability based on the generation
void mutate(PHRASE *p, int length, int generation)
{
    int factor = 1000 / generation;
    int max = factor < 20 ? 50 : factor;
    for (int i = 0; i < length; i++)
    {
        if (choice(0, max) == 0)
        {
            mutations++;
            p->letters[i] = choice(32, 126);
        }
    }
}


// helper function to display correct letters in green, otherwise red
void colorize(char *s)
{
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == target[i])
        {
            printf("\x1B[32m%c", s[i]);
        }
        else
        {
            printf("\x1B[31m%c", s[i]);
        }
    }
    printf("\x1B[37m");
}


// helper print results function
void summarize(int gen, char *p, int fit, int len)
{
    printf("\rGeneration #%3i: ", gen);
    colorize(p);
    printf("  score: %i/%i (%i)\n", fit, len, mutations);
}
