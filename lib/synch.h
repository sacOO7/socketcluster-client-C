/*
* synch.h
*
* Created on: Apr 12, 2012
* Author: anchu
*/

#ifndef __SYNCH_H__
#define __SYNCH_H__

typedef void * semaphore_t;


semaphore_t semaphore_create();
void semaphore_initialize(semaphore_t p_semaphore, int count);
void semaphore_P(semaphore_t p_semaphore);
void semaphore_V(semaphore_t p_semaphore);
void semaphore_destroy(semaphore_t p_semaphore);

#endif /* SYNCH_H_ */

