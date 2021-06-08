#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

int readers = 5;
int writers = 5;
int rcount, wcount, currentwriters;

pthread_mutex_t writeblock, entry, block;
pthread_t readersarr[5], writersarr[5];
pthread_attr_t readatt[5], writeatt[5];

void *writer();
void *reader();
void statePrinter();

int main() {
    pthread_mutex_init(&writeblock, NULL);
    pthread_mutex_init(&entry, NULL);
    pthread_mutex_init(&block, NULL);

    // setting up readers
    for(int i = 0; i < readers; i++) pthread_attr_init(&readatt[i]);
    for(int i = 0; i < readers; i++) pthread_create(&readersarr[i], &readatt[i], reader, NULL);

    // setting up writers
    for(int i = 0; i < writers; i++) pthread_attr_init(&writeatt[i]);
    for(int i = 0; i < writers; i++) pthread_create(&writersarr[i], &writeatt[i], writer, NULL);

    // joining
    for(int i = 0; i < readers; i++) pthread_join(readersarr[i], NULL);
    for(int i = 0; i < writers; i++) pthread_join(writersarr[i], NULL);
    return 0;
}

void statePrinter() {
    printf("ReaderQ:%d WriterQ:%d [in: R:%d W:%d]\n", readers-rcount, writers-currentwriters, rcount, currentwriters);
}

void *reader() {
    while(1) {
        pthread_mutex_lock(&block);
        pthread_mutex_lock(&entry);

        rcount++;
        if(rcount == 1) pthread_mutex_lock(&writeblock);

        statePrinter();

        pthread_mutex_unlock(&entry);
        pthread_mutex_unlock(&block);

        sleep(1);

        pthread_mutex_lock(&entry);
        rcount--;
        if(rcount == 0) pthread_mutex_unlock(&writeblock);

        statePrinter();

        pthread_mutex_unlock(&entry);
    }
}

void *writer() {
    while(1){
        pthread_mutex_lock(&entry);

        wcount++;
        if(wcount == 1) pthread_mutex_lock(&block);

        pthread_mutex_unlock(&entry);

        pthread_mutex_lock(&writeblock);
        currentwriters++;
        statePrinter();
        sleep(1);
        currentwriters--;
        statePrinter();
        pthread_mutex_unlock(&writeblock);

        pthread_mutex_lock(&entry);
        wcount--;

        if(wcount == 0) pthread_mutex_unlock(&block);

        pthread_mutex_unlock(&entry);
    }
}


