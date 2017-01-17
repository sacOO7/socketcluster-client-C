/*
* synch.c
*
* Created on: Apr 12, 2012
* Author: anchu
*/


#include <semaphore.h>

typedef void * semaphore_t;

struct semaphore {
sem_t mutex;
};

semaphore_t semaphore_create() {
struct semaphore * sem = (struct semaphore *) malloc(
sizeof(struct semaphore));
return sem;
}
void semaphore_initialize(semaphore_t p_semaphore, int count) {
struct semaphore * sem = (struct semaphore *) p_semaphore;
if (p_semaphore != NULL)
sem_init(&sem->mutex, 0, count);
}
void semaphore_P(semaphore_t p_semaphore) {
struct semaphore * sem = (struct semaphore *) p_semaphore;
if (p_semaphore != NULL) {
sem_wait(&sem->mutex);
}
}

void semaphore_V(semaphore_t p_semaphore) {
struct semaphore * sem = (struct semaphore *) p_semaphore;
if (p_semaphore != NULL) {
sem_post(&sem->mutex);
}
}

void semaphore_destroy(semaphore_t p_semaphore) {
struct semaphore * sem = (struct semaphore *) p_semaphore;
if (p_semaphore != NULL) {
sem_destroy(&sem->mutex);
}
}