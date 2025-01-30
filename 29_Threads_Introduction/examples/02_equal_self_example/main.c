/*
 * File: main.c
 * Author: Mohamed Ezzat
 * Description: A simple example demonstrating the use of POSIX threads (pthreads) in Linux.
 *              This program creates a demonstrates the usage of pthread_self() & pthread_equal() Syscalls.
 * Compile: gcc -o main.out main.c -lpthread
 * Run: ./main.out
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h> 

pthread_t thread_id;  /* global variable to store the id of the thread */

/* Function for thread 0 */
void *thread_func(void *num) {

    /* Print a message indicating which thread is running */
    printf("Hello from thread[%d]!!\n", (unsigned char)num);
    
    /* Get the thread id using pthread_self() Syscall */
    pthread_t tid = pthread_self();

    /* Check if the two ids are equal */
    if(pthread_equal(tid,thread_id) != 0){
        printf("The two IDs are equal!!\n");
        printf("Thread ID = %d\n",tid);
    }
}

int main() {
    int status;  /* variable to store the status of thread creation */
    
    /* Create thread */
    status = pthread_create(&thread_id, NULL, thread_func, (void *)0);
    
    /* Check if thread was created successfully */
    if (status != 0) {
        printf("Error in creating the thread!!\n");
    }
    
    /* Print a message from the main thread */
    printf("Hello From main thread!!\n\n");
    
    return 0;
}