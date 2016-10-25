#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;


long long in_circle = 0;
long long total = 0;

int main(int argc, char *argv[]){
  total = atoi( argv[1] );
  printf("total num of tosses = %lli\n", total);

  for(int i = 0; i < total; i++){
    //create random (x.y) between 1 ~ -1
    double x = (double)(rand() % RAND_MAX) / RAND_MAX - (double)(rand() % RAND_MAX) / RAND_MAX;
    double y = (double)(rand() % RAND_MAX) / RAND_MAX - (double)(rand() % RAND_MAX) / RAND_MAX;
    cout << i <<": "<<"x=" <<x <<"; y=" << y<< endl;
    double r = x*x + y*y;
    if(r <= 1)  in_circle++;
  }  
  double pi = 4 * in_circle / ((double)total);
  cout << "Estimated pi = " << pi <<endl;
//To be scalable, need to know the number of processors

}
