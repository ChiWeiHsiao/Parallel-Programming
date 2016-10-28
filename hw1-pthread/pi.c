#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAXTHREADS 4


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
    //cout << i <<": "<<"x=" <<x <<"; y=" << y<< endl;
    double r = x*x + y*y;
    if(r <= 1)  partial_in_circle++;
  }

  pthread_mutex_lock(&gLock);
  in_circle += partial_in_circle;
  pthread_mutex_unlock(&gLock);
  
  return 0;
}


int main(int argc, char *argv[]){

  //in_circle = 0;
  total = atoi( argv[1] );
  printf("total num of tosses = %lli\n", total);

  //initialize
  pthread_t threads[MAXTHREADS];
  long t = total / MAXTHREADS;//each thread need to calculate t times
  int thread_count = MAXTHREADS;//actural number of threads created
  int i;
  pthread_mutex_init(&gLock, NULL);
  
  //create threads
  for(i = 0; i<MAXTHREADS; i++){
    int err = pthread_create(&threads[i], NULL, Partial, &t);
    if(err != 0){
      printf("fail! \n");
      thread_count--;
    }
  }
  //join threads
  for(i = 0; i < MAXTHREADS; i++)
    pthread_join(threads[i], NULL);
  pthread_mutex_destroy(&gLock);

  //final result
  double pi = 4 * in_circle / ((double)total);
  printf("actual number of thread = %d\n\n", thread_count);
  printf("actual number of toss = %ld\n\n", t*thread_count);
  //double pi = 4 * in_circle / ((double) t*thread_count);
  printf("Estimated pi = %f \n\n", pi);
  return 0;
}
