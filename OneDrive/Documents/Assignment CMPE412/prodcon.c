#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define SIZE 5 
typedef struct CircularBuffer {
    int buffer[SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mt;
    sem_t empty, full;
} cb;

void initialise(cb *cbuffer){
    cbuffer->head = 0;
    cbuffer->tail = 0;
    cbuffer->count = 0;
    pthread_mutex_init (&cbuffer->mt, NULL);
    sem_init(&cbuffer->full, 0, 0);
    sem_init(&cbuffer->empty, 0, 5);

}

int buffer_insert(cb *cbuffer, int data){
    cbuffer->buffer[cbuffer->tail] = data;
    cbuffer->tail = (cbuffer->tail + 1) % SIZE;
    cbuffer->count ++;
}

int buffer_remove(cb *cbuffer){
    int data = cbuffer->buffer[cbuffer->head];
    cbuffer->head = (cbuffer->head + 1) % SIZE;
    cbuffer->count --;

    return data;
}

int is_buffer_full(cb *buffer){
    return buffer->count == SIZE;
}
int is_buffer_empty(cb *buffer){
    return buffer->count == 0;
}


int main (){

}