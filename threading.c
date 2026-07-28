#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREADS 5

pthread_mutex_t mutex;
sem_t semaphore;

int counter = 0;

void *threadFunction(void *arg)
{
    int id = *(int*)arg;

    sem_wait(&semaphore);

    pthread_mutex_lock(&mutex);

    counter++;

    printf("Thread %d running | Counter = %d\n", id, counter);

    pthread_mutex_unlock(&mutex);

    sem_post(&semaphore);

    sleep(1);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[THREADS];
    int id[THREADS];

    pthread_mutex_init(&mutex,NULL);

    sem_init(&semaphore,0,2);

    for(int i=0;i<THREADS;i++)
    {
        id[i]=i+1;
        pthread_create(&threads[i],NULL,threadFunction,&id[i]);
    }

    for(int i=0;i<THREADS;i++)
    {
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

    sem_destroy(&semaphore);

    return 0;
}


