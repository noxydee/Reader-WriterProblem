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
    int x=0;
    while(x<100)
    {
        sem_wait(&MXReader);
        ReaderCount+=1;

        if(ReaderCount==1)
        {
            sem_wait(&MXWriter);
        }

        sem_post(&MXReader);
        printf("ReaderQ:%d WriterQ:%d [in: R:%d W:%d]\n",ReaderNumber,WritersNumber,ReaderCount,WritersCount);
        sleep(1);
        sem_wait(&MXReader);
        ReaderCount-=1;

        if(ReaderCount==0)
        {
            sem_post(&MXWriter);
        }

        sem_post(&MXReader);
        x+=1;
    }
}

void* Writer_FX(void* arg){
    int x=0;
    while(x<100)
    {
        int* WriterNo = (int*)arg;
        sem_wait(&MXWriter);
        printf("ReaderQ:%d WriterQ:%d [in: R:%d W:%d]\n",ReaderNumber,WritersNumber-1,0,1);
        sleep(2);
        sem_post(&MXWriter);
        x+=1;
    }

}

void InitThreads()
{
    sem_init(&MXReader,0,1);
    sem_init(&MXWriter,0,1);
    int WriterArray[WritersNumber];

    //printf("TEST");
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

    //printf("TestMain");
    WritersNumber = 2;
    ReaderNumber = 2;
    InitThreads();

    //return 0;

}
