/*
 * File: main.c
 * Description: This program demonstrates basic thread creation using POSIX threads (pthread).
 *              It creates a single thread that prints a message and then terminates.
 *
 * Author: Mohamed Ezzat
 * Date: 30/01/2025
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

/*
 * Function: thread_func1
 * ----------------------
 *  The function executed by the created thread.
 *
 *  name: A void pointer cast to an unsigned char pointer containing the thread identifier.
 *
 *  returns: NULL (void pointer) since the thread does not return a value.
 */
void *thread_func1(void *name) {
    printf("Hello from thread[%s]!!\n", (unsigned char *)name);
    return NULL; /* No need for an explicit return value */
}

/* Global variable to store the thread ID */
pthread_t thread_func1_pt;

int main() {
    /*
     * pthread_create function:
     * -------------------------
     *  int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
     *
     * Parameters:
     * 1. pthread_t *thread:
     *    - Output parameter that stores the thread identifier.
     *
     * 2. const pthread_attr_t *attr:
     *    - Specifies thread attributes. NULL sets default attributes.
     *
     * 3. void *(*start_routine)(void *):
     *    - Function pointer to the thread function.
     *    - Accepts a void pointer as an argument and returns a void pointer.
     *
     * 4. void *arg:
     *    - Argument passed to the thread function.
     */
    int status = pthread_create(&thread_func1_pt, NULL, thread_func1, (void *)"1");
    
    /* Check if thread creation was successful */
    if (status != 0) {
        printf("Error in creating the thread!!\n");
    }
    
    /* Print message from the main thread */
    printf("Hello from main thread!!\n");
    
    /* Sleep to allow the created thread to complete execution before main terminates */
    sleep(1);
    
    return 0;
}
