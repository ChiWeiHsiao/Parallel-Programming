#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
//#define MAXTHREADS 4

long long in_circle = 0;
long long total;
pthread_mutex_t gLock;


void *Partial( void *pArg){
  int my_times = *(int*)pArg;
  long long partial_in_circle = 0;

  while(my_times--){
    //create random (x.y) between 1 ~ -1
    double x = 2 * (double)(rand() % RAND_MAX) / RAND_MAX - 1;
    double y = 2 * (double)(rand() % RAND_MAX) / RAND_MAX - 1;
    double r = x*x + y*y;
    // thread1 要去讀寫 partial 的時候會因為 thread2 而要重新去 memory 抓嗎？
    // 用 time.h 算算看 speed up
    if(r <= 1)  partial_in_circle++;
  }

  pthread_mutex_lock(&gLock);
  in_circle += partial_in_circle;
  pthread_mutex_unlock(&gLock);
  
  return 0;
}


int main(int argc, char *argv[]){

  total = strtoll(argv[1], NULL, 10);
  int MAXTHREADS = get_nprocs();
  printf("num of threads = %d\n", MAXTHREADS);

  //initialize
  pthread_t threads[MAXTHREADS];
  long t = total / MAXTHREADS;//each thread need to calculate t times
  int thread_count = MAXTHREADS;//actural number of threads created
  int i;
  pthread_mutex_init(&gLock, NULL);
  
  //create threads
  for(i = 0; i<MAXTHREADS; i++){
    int err = pthread_create(&threads[i], NULL, Partial, &t);
    /*if(err != 0){
      printf("fail! \n");
      thread_count--;
    }*/
  }
  //join threads
  for(i = 0; i < MAXTHREADS; i++)
    pthread_join(threads[i], NULL);

  //final result
  double pi = 4 * (double) in_circle / total;
  //printf("actual number of toss = %ld\n\n", t*thread_count);
  //double pi = 4 * in_circle / ((double) t*thread_count);
  printf("%f\n", pi);

  pthread_mutex_destroy(&gLock);
  free(threads);


  return 0;
}
