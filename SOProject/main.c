#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

sem_t MXReader;
sem_t MXWriter;

int WritersNumber=0;
int WritersCount=0;
int ReaderNumber =0;
int ReaderCount;

void* Reader_FX(void* arg)
{
    while(1)
    {
        sem_wait(&MXReader);
        ReaderCount+=1;

        if(ReaderCount==1)
        {
            sem_wait(&MXWriter);
        }

        sem_post(&MXReader);
        printf("ReaderSleep");
        sleep(2);
        sem_wait(&MXReader);
        ReaderCount-=1;

        if(ReaderCount==0)
        {
            sem_post(&MXWriter);
        }

        sem_post(&MXReader);
    }
}

void* Writer_FX(void* arg){
    while(1)
    {
        int* WriterNo = (int*)arg;
        sem_wait(&MXWriter);
        printf("WriterSleep");
        sleep(2);
        sem_post(&MXWriter);
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

    for(int i=0;i<WritersNumber;i++)
    {
        WriterArray[i]=i;
        pthread_create(&WriterTH[i],NULL,Writer_FX,&WriterArray[i]);
    }
    for(int i=0;i<ReaderNumber;i++)
    {
        pthread_create(&ReaderTH[i],NULL,Reader_FX,NULL);
    }
    for(int i=0;i<WritersNumber;i++)
    {
        pthread_join(WriterTH[i],NULL);
    }
    for(int i=0;i<ReaderNumber;i++)
    {
        pthread_join(ReaderTH[i],NULL);
    }


}

int main() {

    printf("ReaderSleep");
    WritersNumber = 2;
    ReaderNumber = 2;
    InitThreads();

    //return 0;

}
