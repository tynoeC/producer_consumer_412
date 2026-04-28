# Assignment 412: System Programming
## Producer-Consumer Problem in C

This project implements the Producer-Consumer problem using:

* POSIX Threads (pthread)
* Mutex locks
* Semaphores

### Concepts Used

* Thread synchronization
* Race condition prevention
* Circular buffer

### How to Run

```bash
gcc prodcon.c -lpthread -o prodcon
./prodcon
```

### Description

Producers add items to a shared buffer while consumers remove them.
Synchronization is handled using semaphores and mutex to ensure safe access.
