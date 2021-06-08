#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int NumberOfReaders;
int NumberOfWriters;
int NumberOfReadersInside;
int NumberOfWritersInside;
int NumberOfReadersWaiting;
int NumberOfWritersWaiting;

pthread_cond_t ReaderCanRead;
pthread_cond_t WriterCanWrite;

pthread_mutex_t SyncMutex;

void InitPThread()
{
    pthread_cond_init(&ReaderCanRead,NULL);
    pthread_cond_init(&WriterCanWrite,NULL);
    pthread_mutex_init(&SyncMutex,NULL);
}

void* ReaderFX(void* ID)
{
    int IDX = *(int*)ID;
    int x=0;
    while(x<100)
    {
        sleep(1);

        //try start reading
        pthread_mutex_lock(&SyncMutex);

        if(NumberOfWritersInside==1 || NumberOfWritersWaiting>0){
            NumberOfReadersWaiting+=1;
            pthread_cond_wait(&ReaderCanRead,&SyncMutex);
            NumberOfReadersWaiting-=1;
        }
        NumberOfReadersInside+=1;
        //printf("Reader %d is inside\n",IDX);

        printf("ReaderQ:%d WriterQ:%d [in: R:%d W:%d]\n",NumberOfReadersWaiting,NumberOfWritersWaiting,NumberOfReadersInside,NumberOfWritersInside);
        pthread_mutex_unlock(&SyncMutex);
        pthread_cond_broadcast(&ReaderCanRead);

        //try end
        pthread_mutex_lock(&SyncMutex);
        NumberOfReadersInside-=1;
        if(NumberOfReadersInside==0)
        {
            pthread_cond_signal(&WriterCanWrite);
        }
        pthread_mutex_unlock(&SyncMutex);
        x+=1;
    }
}

void* WriterFX(void* ID)
{
    int IDX = *(int*)ID;
    int x=0;
    while(x<100)
    {
        sleep(2);

        pthread_mutex_lock(&SyncMutex);

        if(NumberOfWritersInside ==1 || NumberOfReadersInside>0)
        {
            NumberOfWritersWaiting+=1;
            pthread_cond_wait(&WriterCanWrite,&SyncMutex);
            NumberOfWritersWaiting-=1;
        }
        NumberOfWritersInside =1;
        printf("ReaderQ:%d WriterQ:%d [in: R:%d W:%d]\n",NumberOfReaders-NumberOfReadersInside,NumberOfWriters-NumberOfWritersInside,NumberOfReadersInside,NumberOfWritersInside);
        pthread_mutex_unlock(&SyncMutex);

        pthread_mutex_lock(&SyncMutex);
        NumberOfWritersInside =0;
        if(NumberOfReadersWaiting>0)
        {
            pthread_cond_signal(&ReaderCanRead);
        }
        else
        {
            pthread_cond_signal(&WriterCanWrite);
        }
        pthread_mutex_unlock(&SyncMutex);
        x++;
    }
}



int main() {
    NumberOfReaders=4;
    NumberOfWriters=4;
    NumberOfReadersInside=0;
    NumberOfReadersInside=0;
    NumberOfReadersWaiting=0;
    NumberOfWritersWaiting=0;
    InitPThread();

    pthread_t Readers[NumberOfReaders],Writers[NumberOfWriters];

    for(int i=0;i<NumberOfReaders;i++)
    {
        int ID = i+1;
        pthread_create(&Readers[i],NULL,&ReaderFX,&ID);
    }
    for(int i=0;i<NumberOfWriters;i++)
    {
        int ID = i+1;
        pthread_create(&Writers[i],NULL,&WriterFX,&ID);
    }

    for(int i=0;i<NumberOfReaders;i++)
    {
        pthread_join(Readers[i],NULL);
    }
    for(int i=0;i<NumberOfWriters;i++)
    {
        pthread_join(Writers[i],NULL);
    }

    //printf("Hello, World!\n");
    //return 0;
}
