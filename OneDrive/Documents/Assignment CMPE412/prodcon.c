/*
Name:Tinotenda Mupati
Student No: 22202963
Assignment: Producer-Consumer Problem
Description: Uses mutex and semaphores to synchronize threads
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5 
typedef struct CircularBuffer {
    int buffer[SIZE];//arrary to store buffer elements
    int head; //holds index where the next removal(consumer) will be performed
    int tail; //holds the index where the next insertiion(producer) wiil be performed
    int count; // number of occupied slots
    pthread_mutex_t mt; // protect the buffer from 2 thread touching it at the same time
    sem_t empty, full; // 2 semaphores one (empty) to prevent producer overfill and another(full) to prevent consumer from underflow
    int total_consumer;//total number of items consumed
    int total_producer; //total number of items produced
} cb;

cb cbuffer; //making cbuffer global for all the threads to have access to it directly

void initialise(cb *cirbuffer){// the starting point of each variables and the initialisation of synchronisation
    cirbuffer->head = 0; //start of buffer
    cirbuffer->tail = 0; //end of buffer
    cirbuffer->count = 0; // buffer is empty
    cirbuffer->total_consumer = 0;
    cirbuffer->total_producer = 0;
    pthread_mutex_init (&cirbuffer->mt, NULL); //mutex initialise
    sem_init(&cirbuffer->full, 0, 0); //3 arguments: the last on indicate how many full slot at start
    sem_init(&cirbuffer->empty, 0, SIZE); //3 arguments: the last on indicate how many empty slot at start
}

void buffer_insert(cb *cirbuffer, int data){
    cirbuffer->buffer[cirbuffer->tail] = data; //value generated in put on buffer[tail]
    cirbuffer->tail = (cirbuffer->tail + 1) % SIZE; //moving the tail forward and wrapping around when it hits the end
    cirbuffer->count ++; //incrementing the count after insertion
}

int buffer_remove(cb *cirbuffer){
    int data = cirbuffer->buffer[cirbuffer->head]; //get item from the buffer[head]
    cirbuffer->head = (cirbuffer->head + 1) % SIZE; // moving the head circularly
    cirbuffer->count --; //decrementing the occupied count

    return data;
}

int is_buffer_full(cb *buffer){ // checks if the buffer is full 
    return buffer->count == SIZE;
}

int is_buffer_empty(cb *buffer){ //checks if the buffer is empty
    return buffer->count == 0;
}

void *producer(void *arg){ // arg carries the producer ID by casting it to int id...
    int id = *((int *)arg);

    for (int i = 0; i < 10; i++){//each producer inserts 10 items 
        int item = (rand() % 100) + 1;//random integers between 1-100

        sem_wait(&cbuffer.empty); // wait if there are not empty slot available
        pthread_mutex_lock(&cbuffer.mt);//entering the critical section

        int pos = cbuffer.tail; //store position before insertion
        buffer_insert(&cbuffer, item);
        printf("Producer [%d] produced [%d] at buffer position [%d]\n", id, item, pos);
        cbuffer.total_producer ++;

        pthread_mutex_unlock(&cbuffer.mt);//exiting the critical section
        sem_post(&cbuffer.full);//signal that new item is available
    }
    return NULL;
}

void *consumer(void *arg){//each consumer removes items from the buffer
    int id = *(int *)arg;
    for (int i = 0; i < 10; i++){

        sem_wait(&cbuffer.full); //wait if the buffer is empty
        pthread_mutex_lock(&cbuffer.mt);//critical section

        int pos = cbuffer.head; //store position before removal
        int item = buffer_remove(&cbuffer);
        printf("Consumer [%d] consumed [%d] from buffer position [%d]\n", id, item, pos);
        cbuffer.total_consumer ++;

        pthread_mutex_unlock(&cbuffer.mt);//exit critical section
        sem_post(&cbuffer.empty); //signal that a slot is now free
    }
    return NULL;
}

int main (){
    srand(time(NULL)); // random generator

    pthread_t p1, p2, c1, c2; //thread variables
    int pid1 = 1 , pid2 = 2; //producer ids
    int cid1 = 1 , cid2 = 2; // consumer ids

    initialise(&cbuffer); //intialise buffer and synchronisation tools
    
    //creating producer and consumer threads
    pthread_create(&p1, NULL, &producer, &pid1);
    pthread_create(&p2, NULL, &producer, &pid2);
    pthread_create(&c1, NULL, &consumer, &cid1);
    pthread_create(&c2, NULL, &consumer, &cid2);

    //join and wait for all thread to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    pthread_join(c1, NULL);
    pthread_join(c2, NULL);

    printf("Final Statistics: \n");
    printf("Total produced: %d\n", cbuffer.total_producer);
    printf("Total consumed: %d\n", cbuffer.total_consumer);
    printf("Buffer final count: %d\n", cbuffer.count);

    //clean up resources
    sem_destroy(&cbuffer.empty);
    sem_destroy(&cbuffer.full);
    pthread_mutex_destroy(&cbuffer.mt);

    return 0;

}