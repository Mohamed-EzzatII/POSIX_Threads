/*
 * File: main.c
 * Author: Mohamed Ezzat
 * Description: A simple example demonstrating the way that pthread_mutex_trylock works
 * Compile: gcc -o main.out main.c -lpthread
 * Run: ./main.out
 */

#include <pthread.h>
#include <stdio.h>

#define THREAD_1 1    /* Define a constant for thread 1 */
#define THREAD_2 2    /* Define a constant for thread 2 */

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; /* Mutex to synchronize access to shared resource */
static int shared_resource = 0; /* Global variable shared between threads */
static int locking_error[3]={0};/* To store the number of unsuccessful attemps to lock the mutex*/

/**
 * Function executed by each thread.
 * 
 * @param num - Thread ID (cast from void* to int)
 * @return void* (NULL in this case)
 */
void *pthread_func(void *num) {
    printf("Thread [%d] started execution!!\n", (int)num);

    int local;   /* Local copy of shared_resource for the thread */
    int status;  /* Variable to store the return value of mutex operations */

    /*
     * Loop to increment the local variable multiple times until it
     * becomes 10000 by the help of the shared resource
     */
    while (local != 10000) {
        
        /* Lock the mutex before modifying the shared resource */
        /*
         * pthread_mutex_trylock function:
         * ----------------------------
         *  int pthread_mutex_trylock(pthread_mutex_t *mutex);
         *
         * Parameters:
         * 1. pthread_mutex_t *mutex:
         *    - A pointer to the mutex to be locked.
         * 
         * Returns:
         * - 0 if successful.
         * - EBUSY if mutex could not be locked.
         */
        status = pthread_mutex_trylock(&mtx);
        if (status != 0) {
            /* Increment the counter which stores the number of unsuccessful locks */
            locking_error[(int)num]++;
        }

        else{
            /* Critical section: modifying shared resource */
            shared_resource = local;  /* Assign shared value to local */
            shared_resource++;        /* Increment shared value */
            local = shared_resource;  /* Update local copy */

            /* Unlock the mutex after modifying shared resource */
            /*
            * pthread_mutex_unlock function:
            * ------------------------------
            *  int pthread_mutex_unlock(pthread_mutex_t *mutex);
            *
            * Parameters:
            * 1. pthread_mutex_t *mutex:
            *    - A pointer to the mutex to be unlocked.
            *
            * Returns:
            * - 0 if successful.
            * - Error code if mutex could not be unlocked.
            */
            status = pthread_mutex_unlock(&mtx);
            if (status != 0) {
                printf("Thread [%d] can't unlock the mutex\n", (int)num);
                pthread_exit(NULL);  /* Exit the thread if unlocking fails */
            }
        }
    }

    /* Print the final values of local and shared_resource */
    printf("Task[%d] : local = %d, shared_resource = %d\n", (int)num, local, shared_resource);

    pthread_exit(NULL); /* Exit the thread successfully */
}

int main() {
    pthread_t t1, t2;  /* Thread identifiers */
    int status[2];      /* Array to store thread creation status */

    /* Create first thread (t1) */
    /*
     * pthread_create function:
     * ------------------------
     *  int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
     *                     void *(*start_routine)(void *), void *arg);
     *
     * Parameters:
     * 1. pthread_t *thread:
     *    - A pointer to the pthread_t variable that will store the thread ID.
     * 
     * 2. const pthread_attr_t *attr:
     *    - NULL for default thread attributes.
     * 
     * 3. void *(*start_routine)(void *):
     *    - A pointer to the function that the thread will execute.
     * 
     * 4. void *arg:
     *    - The argument passed to the start routine (in this case, thread ID).
     *
     * Returns:
     * - 0 if successful.
     * - Error code if thread creation fails.
     */
    status[0] = pthread_create(&t1, NULL, pthread_func, (void *)THREAD_1);
    if (status[0] != 0) {
        printf("Error creating thread[1]!!\n");
        return -1;
    }

    /* Create second thread (t2) */
    status[1] = pthread_create(&t2, NULL, pthread_func, (void *)THREAD_2);
    if (status[1] != 0) {
        printf("Error creating thread[2]!!\n");
        return -1;
    }

    /* Print a message from the main thread */
    printf("Hello from main thread!!\n\n");

    /* Wait for thread 1 to complete execution */
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
     *
     * Returns:
     * - 0 if successful.
     * - Error code if join fails.
     */
    pthread_join(t1, NULL);
    
    /* Wait for thread 2 to complete execution */
    pthread_join(t2, NULL);

    /* Print the final value of shared_resource */
    printf("Main Thread: shared_resource = %d\n", shared_resource);

    /* Print the number of unsuccessful attemps to lock the mutex for each thread */
    printf("Tread[1] unsuccessful attemps = %d\n",locking_error[1]);
    printf("Tread[2] unsuccessful attemps = %d\n",locking_error[2]);

    return 0;  /* Exit successfully */
}