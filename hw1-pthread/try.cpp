#include <cstdio>
#include <pthread.h>
#define MAXTHREADS 1000000
#define THREADSTACK  65536

pthread_mutex_t mutex_;
int thread_nr = 0;

void* inc_thread_nr(void* arg) {
    (void*)arg;
    pthread_mutex_lock(&mutex_);
    thread_nr ++;
    pthread_mutex_unlock(&mutex_);

    printf("thread_nr = %d\n", thread_nr);

   // sleep(300000);
}

int main()
{
    pthread_t       pid[MAXTHREADS];
    pthread_attr_t  attrs;
    int  err, i;
    int  cnt = 0;

    pthread_attr_init(&attrs);
    pthread_attr_setstacksize(&attrs, THREADSTACK);

    pthread_mutex_init(&mutex_, NULL);

    for (cnt = 0; cnt < MAXTHREADS; cnt++) {
    
            err = pthread_create(&pid[cnt], &attrs, inc_thread_nr, NULL);
            if (err != 0)
                break;
        }

    pthread_attr_destroy(&attrs);

    for (i = 0; i < cnt; i++)
        pthread_join(pid[i], NULL);

    pthread_mutex_destroy(&mutex_);

    printf("Maximum number of threads per process is %d (%d)\n", cnt, thread_nr);
}
