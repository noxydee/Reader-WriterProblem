#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

sem_t MX_1, MX_2, MX_3;
sem_t MXReader, MXWriter;

int WritersNumber = 0;
int WritersCount = 0;
int ReaderNumber = 0;
int ReaderCount = 0;

void* Reader_FX(void* arg)
{
    while(1)
    {
        sem_wait(&MX_3);
        sem_wait(&MXReader);
        sem_wait(&MX_1);
        ReaderCount+=1;

        if(ReaderCount == 1)
        {
            sem_wait(&MXWriter);
        }

        sem_post(&MX_1);
        sem_post(&MXReader);
        sem_post(&MX_3);
        printf("Reading");
        sleep(2);
        sem_wait(&MX_1);
        ReaderCount-=1;

        if (ReaderCount = 0)
        {
            sem_post(&MXWriter);
        }
        sem_post(&MX_1);
    }
}

void* Writer_FX(void* arg)
{
    while(1)
    {
        sem_wait(&MX_2);
        WritersCount += 1;

        if(WritersCount = 1)
        {
            sem_wait(&MXReader);
        }

        sem_post(&MX_2);
        sem_wait(&MXWriter);
        printf("Writing");
        sleep(2);
        sem_post(&MXWriter);
        sem_wait(&MX_2);
        WritersCount -=1;

        if(WritersCount = 0)
        {
            sem_post(&MXReader);
        }
        sem_post(&MX_2);
    }

}
void InitThreads()
{
    sem_init(&MXReader,0,1);
    sem_init(&MXWriter,0,1);
    int WriterArray[WritersNumber];

    printf("ReaderSleep %d",WritersNumber);
    pthread_t ReaderTH[ReaderNumber];
    pthread_t WriterTH[WritersNumber];

    for(int i=0; i<WritersNumber; i++)
    {
        WriterArray[i]=i;
        pthread_create(&WriterTH[i],NULL,Writer_FX,&WriterArray[i]);
    }
    for(int i=0; i<ReaderNumber; i++)
    {
        pthread_create(&ReaderTH[i],NULL,Reader_FX,NULL);
    }
    for(int i=0; i<WritersNumber; i++)
    {
        pthread_join(WriterTH[i],NULL);
    }
    for(int i=0; i<ReaderNumber; i++)
    {
        pthread_join(ReaderTH[i],NULL);
    }


}
int main()
{

    printf("ReaderSleep");
    WritersNumber = 2;
    ReaderNumber = 2;
    InitThreads();

    //return 0;

}
