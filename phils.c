/*
*
* Name: Hayden Mersky & Antonio Hernandez
* Date: 04/28/2026
* Description: Simulates Dining Philosophers problem using 
* threads and semaphores.
* 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// #define will go through and replace the word with the value before compilation
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int numOfPhils; // number of phils, passed in as argument[1]
int numOfTimesToEat; // number of times to eat each, passed in as argument[2]
sem_t *chopsticks; // array of semaphores for each chopstick (calling them forks is confusing)
int *state;
int *phils;

// functions that may be helpful to create
void test(); // used to check state of philsopher and state of each Chopstick
             // if philospher is hungry and both left and right are satisifed
             // then they should be able to eat now

void pickupChopsticks(int num){ // waits to grab chopsticks for philospher (denotes when philospher is hungry)
    printf("Philosopher %d is hungry...\n", num);
    state[num] = HUNGRY; // set state

    // Implement asymmetric solution
    if (num % 2 == 0) { // even numbered philosopher
        sem_wait(&chopsticks[num]); // wait for left chopstick
        sem_wait(&chopsticks[(num + 1) % numOfPhils]); // wait for right chopstick
    } else { // odd numbered philosopher
        sem_wait(&chopsticks[(num + 1) % numOfPhils]); // wait for right chopstick
        sem_wait(&chopsticks[num]); // wait for left chopstick
    }

}

void putDownChopsticks(int num){ // puts chopsticks back down (denotes when philospher is thinking)
    printf("Philosopher %d is thinking...\n", num);
    state[num] = THINKING; // set state

    sem_post(&chopsticks[num]);
    sem_post(&chopsticks[(num + 1) % numOfPhils]);
}

void *philosopher(); // must be a pointer when working with threading
                     // determines first action of a philospher when thread is created

int main(int argc, char *argv[]) {

    // thread usage
    pthread_t threads[numOfPhils];
    // for threads you will need to incorporate the concept of creating and
    // joining to solve this problem

    // memory allocation for chopsticks, state, and philosphers
    chopsticks = malloc(numOfPhils * sizeof(sem_t));
    state = malloc(numOfPhils * sizeof(int));
    phils = malloc(numOfPhils * sizeof(int));

    // create philosphers and give them a state based on numOfPhils
    // then create a thread for each using


    free(forks);
    free(state);
    free(phils);
    return 0;
}