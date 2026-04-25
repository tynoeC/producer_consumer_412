#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>


#define SIZE 5 
typedef struct CircularBuffer {
    int buffer[SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mt;
    sem_t empty, full;
} cb;

cb cbuffer;

void initialise(cb *cirbuffer){
    cirbuffer->head = 0;
    cirbuffer->tail = 0;
    cirbuffer->count = 0;
    pthread_mutex_init (&cirbuffer->mt, NULL);
    sem_init(&cirbuffer->full, 0, 0);
    sem_init(&cirbuffer->empty, 0, 5);

}

int buffer_insert(cb *cirbuffer, int data){
    cirbuffer->buffer[cirbuffer->tail] = data;
    cirbuffer->tail = (cirbuffer->tail + 1) % SIZE;
    cirbuffer->count ++;
}

int buffer_remove(cb *cirbuffer){
    int data = cirbuffer->buffer[cirbuffer->head];
    cirbuffer->head = (cirbuffer->head + 1) % SIZE;
    cirbuffer->count --;

    return data;
}

int is_buffer_full(cb *buffer){
    return buffer->count == SIZE;
}

int is_buffer_empty(cb *buffer){
    return buffer->count == 0;
}

void *producer(void *arg){
    int id = *((int *)arg);

    for (int i = 0; i < 10; i++){
        int item = (rand() % 100) + 1;

        sem_wait(&cbuffer.empty);
        pthread_mutex_lock(&cbuffer.mt);

        int pos = cbuffer.tail;
        buffer_insert(&cbuffer, item);
        printf("Producer [%d] produced [%d] at buffer position [%d]\n", id, item, pos);

        pthread_mutex_unlock(&cbuffer.mt);
        sem_post(&cbuffer.full);
    }
    return NULL;
}

int main (){

}