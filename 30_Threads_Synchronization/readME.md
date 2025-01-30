# Chapter 30 : Thread Synchronization
## Content of Chapter:
1. Protecting Accesses to Shared Variables: Mutexes

## Protecting Accesses to Shared Variables: Mutexes

In multithreaded programming, threads often need to access shared resources, such as global variables, files, or data structures. However, when multiple threads access or modify these shared resources simultaneously, it can lead to **race conditions**, which cause unpredictable and incorrect behavior. To solve this problem, we use the concept of a **critical section** and synchronization mechanisms like **mutexes**.

---

### **The Problem of Critical Sections**

#### **What is a Critical Section?**
- A **critical section** is a block of code that accesses a shared resource (e.g., a global variable, file, or data structure) and must be executed **atomically**.
- **Atomic execution** means that once a thread starts executing the critical section, it must complete it without being interrupted by other threads.

#### **Why Do We Need Critical Sections?**
- Without proper synchronization, multiple threads can interfere with each other when accessing shared resources, leading to **race conditions**.
- Race conditions occur when the final result depends on the timing of thread execution, which is unpredictable and controlled by the operating system's scheduler.

#### **Example of a Critical Section Problem**

Consider the following example where two threads increment a shared global variable (`glob`):

```c
#include <pthread.h>
#include <stdio.h>

static int glob = 0;  // Shared global variable

void *threadFunc(void *arg) {
    int loops = *((int *)arg);
    for (int j = 0; j < loops; j++) {
        int loc = glob;  // Read global variable
        loc++;           // Increment local copy
        glob = loc;      // Write back to global variable
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int loops = 1000000;  // Number of increments per thread

    // Create two threads
    pthread_create(&t1, NULL, threadFunc, &loops);
    pthread_create(&t2, NULL, threadFunc, &loops);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("glob = %d\n", glob);  // Print final value of glob
    return 0;
}
```

#### **What Happens Here?**
- Both threads read, increment, and write back the value of `glob`.
- If Thread 1 reads `glob = 1000` and is interrupted before writing back, Thread 2 might also read `glob = 1000`, increment it, and write back `1001`.
- When Thread 1 resumes, it writes back `1001`, effectively losing the increment performed by Thread 2.
- This results in an incorrect final value of `glob`.

#### **Expected vs. Actual Output**
- Expected: `glob = 2000000` (each thread increments `glob` by 1,000,000).
- Actual: `glob = 1654321` (or some other incorrect value due to race conditions).

---

### **What is a Mutex?**

#### **Definition**
- A **mutex** (short for **mutual exclusion**) is a synchronization primitive used to protect shared resources from concurrent access.
- It ensures that only one thread can execute a critical section at a time.

#### **How Does a Mutex Work?**
- A mutex has two states: **locked** and **unlocked**.
- When a thread locks a mutex, it gains exclusive access to the critical section.
- Other threads attempting to lock the same mutex will block (wait) until the mutex is unlocked.

#### **Who Can Lock and Unlock a Mutex?**
- Only the thread that locks a mutex can unlock it. This ensures **ownership** and prevents other threads from unlocking a mutex they don't own.

---

### **Solving the Critical Section Problem with Mutexes**

- **How To Enter Critical Section Using Mutex**:
  1. Threads lock the mutex before entering the critical section.
  2. Threads access the resource(global variable).
  3. Threads unlock the mutex after exiting the critical section.
---
### **Create a Static Mutex**

A **static mutex** is a mutex that is initialized at compile-time using `PTHREAD_MUTEX_INITIALIZER`. It provides a simple and efficient way to create a mutex without calling an explicit initialization function.

#### **Syntax**
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

#### **Attributes**
- A **static mutex** is initialized with **default attributes**, which means:
  - It is **fast** (normal) mutex by default.
  - No need to call `pthread_mutex_init()` explicitly.
  - It is automatically **destroyed** when the program terminates.

---

### **pthread_mutex_lock** Library Call
This function **locks** the mutex. If another thread has already locked it, the calling thread is **blocked** (waits) until the mutex becomes available.
If a thread locks a mutex, then the same thread tries to lock the mutex again while it was locked,then the call may fails and return **EDEADLK**, or in linux the thread deadlocks

#### **Syntax**
```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```

#### **Return Values**
- `0` → Success, mutex acquired.
- `EBUSY` → Mutex is already locked (**only applies to `PTHREAD_MUTEX_ERRORCHECK` type**).
- `EINVAL` → Invalid mutex.
- `EDEADLK` → The calling thread **already owns** the mutex (**only applies to `PTHREAD_MUTEX_ERRORCHECK` type**).

#### **Example**
```c
pthread_mutex_lock(&mutex);
```

---

#### **pthread_mutex_unlock** Library Call
This function **unlocks** a mutex that was previously locked by the calling thread.
If a mutex is unlocked and you tried to unlock it again, or a thread tries to unlock a mutex that has been locked by another thread, it is an error
If there are more than one thread waiting for the unlocking of a mutex, we can't determine which thread will acquire it when the mutex is unlocked.

#### **Syntax**
```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

#### **Return Values**
- `0` → Success, mutex unlocked.
- `EINVAL` → Invalid mutex.
- `EPERM` → The calling thread does not own the mutex (**only applies to `PTHREAD_MUTEX_ERRORCHECK` type**).

#### **Example**
```c
pthread_mutex_unlock(&mutex);
```
---

### **pthread_mutex_trylock** Library Call
This function **attempts** to lock a mutex but **does not block** if the mutex is already locked.

#### **Syntax**
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

#### **Return Values**
- `0` → Success, mutex acquired.
- `EBUSY` → Mutex is already locked by another thread.
- `EINVAL` → Invalid mutex.

#### **Example**
```c
if (pthread_mutex_trylock(&mutex) == 0) {
    printf("Mutex locked successfully\n");
    pthread_mutex_unlock(&mutex);
} else {
    printf("Mutex is already locked by another thread\n");
}
```

---

### **pthread_mutex_timedlock**
This function attempts to lock a mutex but **waits only for a specified time** before giving up.
That specific time is an absolute time given in the struct `abs_timeout`.

#### **Syntax**
```c
int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout);
```

#### **Return Values**
- `0` → Success, mutex acquired.
- `ETIMEDOUT` → Mutex was not acquired within the specified time.
- `EINVAL` → Invalid arguments (e.g., incorrect timeout value).

#### **Structure Definition**
```c
struct timespec {
    time_t tv_sec;  /* Seconds */
    long   tv_nsec; /* Nanoseconds (0 to 999,999,999) */
};
```

---