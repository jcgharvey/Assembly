/* 
 * File:   a1.0.c
 * Author: Robert Sheehan
 *
 * Created on 11 July 2012, 2:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define TICKETS 100000000
#define NUMTHREADS 16

int ticket = -1;
int usedTickets[TICKETS];
int lockValue = 0;

pthread_mutex_t mutex;
// all threads try to get some of the tickets

void *lock(void)
{
    pthread_mutex_lock(&mutex);
}

void *unlock(void)
{
    pthread_mutex_unlock(&mutex);   
}

void *grabATicket(void *unused) {
    lock();
    int i;
    bool ticketsReused = false;
    for (i = 0; i < TICKETS / NUMTHREADS; i++) {
        ticket++;
        int localTicket = ticket;
        if (usedTickets[localTicket])
            ticketsReused = true;
        usedTickets[localTicket] = 1;
    }
    if (ticketsReused) puts("At least one ticket was reused.");
    unlock();
    return NULL;
}

int main(void) {
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[NUMTHREADS];
    pthread_t thread1, thread2;
    int i;
    for (i = 0; i < NUMTHREADS; i++)
        pthread_create(&threads[i], NULL, grabATicket, NULL);
    for (i = 0; i < NUMTHREADS; i++)
        pthread_join(threads[i], NULL);
    printf("The number of tickets grabbed = %d\n", ticket + 1);
    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}
