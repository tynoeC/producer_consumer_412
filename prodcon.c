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


int buffer_insert(){

}

int buffer_remove(){

}

int is_buffer_full(cb *buffer){
    return buffer->count == SIZE;
}
int is_buffer_empty(cb *buffer){
    return buffer->count == 0;
}


int main (){
    pthread_t prod1, prod2;
    pthread_t con1, con2;


}