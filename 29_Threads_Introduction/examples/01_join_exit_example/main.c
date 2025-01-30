/*
 * File: main.c
 * Author: Mohamed Ezzat
 * Description: A simple example demonstrating the use of POSIX threads (pthreads) in Linux.
 *              This program creates two threads, synchronizes them, and demonstrates thread communication.
 * Compile: gcc -o main.out main.c -lpthread
 * Run: ./main.out
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h> 

#define THREAD_0 0    /* Define a constant for thread 0 */
#define THREAD_1 1    /* Define a constant for thread 1 */

pthread_t thread_id[2];  /* Array to store thread IDs for two threads */

/* Function for thread 0 */
void *thread0_func(void *num) {
    /* Print a message indicating which thread is running */
    printf("Hello from thread[%d]!!\n", (unsigned char)num);
    
    /* Print a message indicating that the thread is exiting */
    printf("Thread[%d] exit!!\n\n", (unsigned char)num);
    
    /*
     * pthread_exit function:
     * ----------------------
     *  void pthread_exit(void *retval);
     *
     * Parameters:
     * 1. void *retval:
     *    - A pointer to the return value that can be collected by pthread_join.
     *    - It can be used to return status or result of the thread.
     */
    pthread_exit((void *)THREAD_0);/* Exit the thread and return a value (THREAD_0 in this case) */
}

/* Function for thread 1 */
void *thread1_func(void *num) {
    void *ret;  /* Variable to store the return value from thread 0 */
    
    /* Wait for thread 0 to finish and retrieve its return value */
    /*
     * pthread_join function:
     * ----------------------
     *  int pthread_join(pthread_t thread, void **retval);
     *
     * Parameters:
     * 1. pthread_t thread:
     *    - The identifier of the thread to be joined.
     *
     * 2. void **retval:
     *    - A pointer to a pointer where the exit status of the joined thread is stored.
     *    - If NULL is passed, the return value of the thread is ignored.
     */
    pthread_join(thread_id[THREAD_0], &ret);
    
    /* Print a message indicating which thread is running */
    printf("Hello from thread[%d]!!\n", (unsigned char)num);
    
    /* Print the value received from thread 0 */
    printf("Thread [1] received [%d] from thread[0]\n\n", (unsigned char)ret);
    
    /* Print a message indicating that the thread is exiting */
    printf("Thread[%d] exit!!\n\n", (unsigned char)num);
    
    /* Exit the thread and return a value (THREAD_1 in this case) */
    pthread_exit((void *)THREAD_1);
}

int main() {
    int status[2];  /* Array to store the status of thread creation */
    
    /* Create thread 0 */
    status[THREAD_0] = pthread_create(&thread_id[THREAD_0], NULL, thread0_func, (void *)THREAD_0);
    
    /* Create thread 1 */
    status[THREAD_1] = pthread_create(&thread_id[THREAD_1], NULL, thread1_func, (void *)THREAD_1);
    
    /* Check if thread 0 was created successfully */
    if (status[THREAD_0] != 0) {
        printf("Error in creating the thread[0]!!\n");
    }
    
    /* Check if thread 1 was created successfully */
    if (status[THREAD_1] != 0) {
        printf("Error in creating the thread[1]!!\n");
    }
    
    /* Print a message from the main thread */
    printf("Hello From main thread!!\n\n");
    
    void *ret;  /* Variable to store the return value from thread 1 */
    
    /* Wait for thread 1 to finish and retrieve its return value */
    pthread_join(thread_id[THREAD_1], &ret);
    
    /* Print the value received from thread 1 */
    printf("Main Thread received [%d] from Thread[1]\n", (unsigned char)ret);
    
    return 0;
}