#include <stdio.h>
#include <pthread.h>
#define array_size 1000
#define nb_threads 10

int array[array_size];
int global_index = 0;
int sum = 0;
pthread_mutex_t lock;

void *thread_func(void *ignored) {
    int local_index, partial_sum = 0;
    do {
        pthread_mutex_lock(&lock);
        local_index = global_index;
        global_index++;
        pthread_mutex_unlock(&lock);

        if (local_index < array_size){
          partial_sum += array[local_index];
        }
    } while (local_index < array_size);

    pthread_mutex_lock(&lock);
    sum += partial_sum;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char const *argv[]) {
    int i;
    pthread_t thread[10];
    pthread_mutex_init(&lock, NULL);

    for (i = 0; i < array_size; i++)
        array[i] = i+1;

    for (i = 0; i < nb_threads; i++)
     //   if (pthread_create(&thread[i], NULL, thread_func, NULL) != 0)
            perror("Pthread create fails");

    for (i = 0; i < nb_threads; i++)
       // if (pthread_join(thread[i], NULL) != 0)
            perror("Pthread join fails");

    printf("The sum of 1 to %i is %d\n", array_size, sum);
    return 0;
}
