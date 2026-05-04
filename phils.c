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

void *philosopher(void *arg){ // must be a pointer when working with threading
                     // determines first action of a philospher when thread is created
    // Get Specific Philospher data
    int id = *(int *)arg; // get phil ID
    int eatCount = 0; //Ensure one is not eating more than the other so we count

    //Show user the state
    if (state[id] == THINKING) {
        printf("Philosopher %d is thinking...\n", id);
    } else if (state[id] == HUNGRY) {
        printf("Philosopher %d is hungry...\n", id);
    }

    while (eatCount < numOfTimesToEat) {
        //If thinking must become hungry
        if (state[id] == THINKING){
            sleep(1 + rand() % 2);
            state[id] = HUNGRY;
            printf("Philosopher %d is hungry...\n", id);
        }

        // If hungry try to eat
        pickupChopsticks(id);

        //If eating update
        state[id] = EATING;
        printf("Philosopher %d is eating...\n", id);
        sleep(1 + rand() % 2);
        eatCount++;

        // Done eating 
        putDownChopsticks(id);

        //think before ext cycle
        sleep(1 + rand() % 2);
    }

    pthread_exit(NULL);

}


// --- Test helper ---
void printStates() {
    printf("States: ");
    for (int i = 0; i < numOfPhils; i++) {
        printf("%d ", state[i]);
    }
    printf("\n");
}

// --- MAIN TESTING VERSION ---
int main(int argc, char *argv[]) {
    printf("=== Dining Philosophers Mock Test ===\n");

    // Check command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Number Of Philosophers> <Number Of Times To Eat>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    numOfPhils = atoi(argv[1]);
    numOfTimesToEat = atoi(argv[2]);

    pthread_t threads[numOfPhils];

    // Allocate memory
    chopsticks = malloc(numOfPhils * sizeof(sem_t));
    state = malloc(numOfPhils * sizeof(int));
    phils = malloc(numOfPhils * sizeof(int));

    // Initialize semaphores and states
    for (int i = 0; i < numOfPhils; i++) {
        sem_init(&chopsticks[i], 0, 1);
        state[i] = THINKING;
    }

    printStates();

    // TEST 1: Even philosopher picks up left then right
    printf("\nTEST 1: Philosopher 2 pickup\n");
    pickupChopsticks(2);
    printStates();

    // TEST 2: Odd philosopher picks up right then left
    printf("\nTEST 2: Philosopher 3 pickup\n");
    pickupChopsticks(3);
    printStates();

    // TEST 3: Put down chopsticks
    printf("\nTEST 3: Philosopher 2 put down\n");
    putDownChopsticks(2);
    printStates();

    // TEST 4: Wrap-around test (philosopher 4 right chopstick = 0)
    printf("\nTEST 4: Philosopher 4 pickup (wrap-around)\n");
    pickupChopsticks(4);
    printStates();

    // Cleanup
    free(chopsticks);
    free(state);
    printf("\n=== TEST COMPLETE ===\n");
    return 0;
}

/*
int main(int argc, char *argv[]) {

    // thread usage
    pthread_t threads[numOfPhils];
    // for threads you will need to incorporate the concept of creating and
    // joining to solve this problem

    // initialize the threads and the philosphers
    pthread_create(&threads[i], NULL, philosopher, &phils[i]);



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
*/
