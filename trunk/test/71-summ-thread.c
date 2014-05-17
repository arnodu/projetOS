#include <stdio.h>
#include <thread.h>
#define array_size 1000
#define nb_threads 20

int array[array_size];
int global_index = 0;
int sum = 0;
thread_mutex_t lock;

void *thread_func(void *ignored) {
    int local_index, partial_sum = 0;
    do {
        thread_mutex_lock(&lock);
        local_index = global_index;
        global_index++;
        thread_mutex_unlock(&lock);

        if (local_index < array_size){
          partial_sum += array[local_index];
        }
    } while (local_index < array_size);

    thread_mutex_lock(&lock);
    sum += partial_sum;
    thread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char const *argv[]) {
    int i;
    thread_t thread[10];
    thread_mutex_init(&lock);

    for (i = 0; i < array_size; i++)
        array[i] = i+1;

    for (i = 0; i < nb_threads; i++)
    if (thread_create(&thread[i], thread_func, NULL) != 0)
            perror("Thread create fails");

    for (i = 0; i < nb_threads; i++)
    if (thread_join(thread[i], NULL) != 0)
            perror("Thread join fails");

    printf("The sum of 1 to %i is %d\n", array_size, sum);
    return 0;
}
