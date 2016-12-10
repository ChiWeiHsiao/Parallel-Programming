/**********************************************************************
 * DESCRIPTION:
 *   Serial Concurrent Wave Equation - C Version
 *   This program implements the concurrent wave equation
 *********************************************************************/
#include <stdio.h>    
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265

void check_param(void);
__global__ void init_line(int, float*, float*);
__global__ void update (int, int, float*, float*);
void printfinal (void);

int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode;                  	/* generic return code */
//float  values[MAXPOINTS+2], 	/* values at time t */
//       oldval[MAXPOINTS+2], 	/* values at time (t-dt) */
//       newval[MAXPOINTS+2]; 	/* values at time (t+dt) */
float *values, *oldval, *newval;

/**********************************************************************
 *	Checks input values from parameters
 *********************************************************************/
void check_param(void)
{
   char tchar[20];

   /* check number of points, number of iterations */
   while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
      printf("Enter number of points along vibrating string [%d-%d]: "
           ,MINPOINTS, MAXPOINTS);
      scanf("%s", tchar);
      tpoints = atoi(tchar);
      if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
         printf("Invalid. Please enter value between %d and %d\n", 
                 MINPOINTS, MAXPOINTS);
   }
   while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
      printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
      scanf("%s", tchar);
      nsteps = atoi(tchar);
      if ((nsteps < 1) || (nsteps > MAXSTEPS))
         printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
   }

   printf("Using points = %d, steps = %d\n", tpoints, nsteps);

}

/**********************************************************************
 *     Initialize points on line
 *********************************************************************/
__global__ void init_line(int __tpoints, float* __oldval, float* __newval)//(tpoints, oldval, newval)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  //float x;
  if (i < __tpoints) {
    float x = (float) i / (__tpoints - 1);
    __newval[i] = __sinf(6.2831853 * x);
    __oldval[i] = __newval[i];
  }
//=============================================
  /*int i, j;
   float x, k, tmp; 
   // Calculate initial values based on sine curve 
   k = 0.0; 
   tmp = tpoints - 1;
   for (j = 1; j <= tpoints; j++) {
      x = k/tmp;
      values[j] = sin (6.2831853 * x);//fac = 2.0 * PI;
      k = k + 1.0;
   } 
   //Initialize old values array 
   for (i = 1; i <= tpoints; i++) 
      oldval[i] = values[i];*/
}

/**********************************************************************
 *      Calculate new values using wave equation
 *********************************************************************/
void do_math(int i) // The Effect is: newval[i] = (1.82)*values[i] - oldval[i] 
{
   float dtime, c, dx, tau, sqtau;
   dtime = 0.3;
   c = 1.0;
   dx = 1.0;
   tau = (c * dtime / dx);  //=0.3
   sqtau = tau * tau; //=0.09
   newval[i] = (2.0 * values[i]) - oldval[i] + (sqtau *  (-2.0)*values[i]);// = (2-0.18)*values[i] - oldval[i]
}

/**********************************************************************
 *     Update all values along line a specified number of times
 *********************************************************************/
__global__ void update(int __nsteps, int __tpoints, float* __oldval, float* __newval)  //(nsteps, tpoints, oldval, newval);
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < __tpoints) {
    float tmp_old = __oldval[i];
    float tmp_new = __newval[i];
    float next;
    for (int j = 0; j < __nsteps; j++) {
      // global endpoints 
      if ((i == 0) || (i == __tpoints - 1))
        next = 0;
      else
        //do_math, newval[i] = (1.82)*values[i] - oldval[i] 
        next = (1.82) * tmp_new - tmp_old;
      tmp_old = tmp_new;
      tmp_new = next;
    }
    __newval[i] = tmp_new;
  }
  //==============
  /* Update values for each time step 
   for (i = 1; i<= nsteps; i++) {
      // Update points along line for this time step 
      for (j = 1; j <= tpoints; j++) {
         /* global endpoints 
         if ((j == 1) || (j  == tpoints))
            newval[j] = 0.0;
         else
            do_math(j);
      }

      // Update old values with new values 
      for (j = 1; j <= tpoints; j++) {
         oldval[j] = values[j];
         values[j] = newval[j];
      }
   }*/
}
/**********************************************************************
 *     Print final results
 *********************************************************************/
void printfinal()
{
   int i;
   //for (i = 1; i <= tpoints; i++) { //stat from 1
   for (i = 0; i < tpoints; i++) {  //start from 0
      printf("%6.4f ", values[i]);
      if ((i+1)%10 == 0)  //start from 0, so +1
         printf("\n");
   }
}

/**********************************************************************
 *	Main program
 *********************************************************************/
int main(int argc, char *argv[])
{
//clock_t t = clock();//time
	sscanf(argv[1],"%d",&tpoints);
	sscanf(argv[2],"%d",&nsteps);
	check_param();
  /* Memory */
  values = (float*) malloc( (tpoints+256) * sizeof(float));
  cudaMalloc((void**) &newval, (tpoints+256) * sizeof(float));
  cudaMalloc((void**) &oldval, (tpoints+256) * sizeof(float));
  /* Invoke kernel*/
  dim3 threadsPerBlock(256);
  dim3 numOfBlocks((tpoints+256)/256);
	printf("Initializing points on the line...\n");
	init_line<<<numOfBlocks, threadsPerBlock>>>(tpoints, oldval, newval);
	printf("Updating all points for all time steps...\n");
	update<<<numOfBlocks, threadsPerBlock>>>(nsteps, tpoints, oldval, newval);
  //Move from gpu to cpu
  cudaMemcpy(values, newval, (tpoints+256)*sizeof(float), cudaMemcpyDeviceToHost);

	printf("Printing final results...\n");
	printfinal();
	printf("\nDone.\n\n");
  //Free all memory
  cudaFree(newval);
  cudaFree(oldval);
  free(values);
//!!!!!記得刪掉時間!!!!!
//t = clock() - t;//time
//printf("Time : %f sec\n", ((float)t)/CLOCKS_PER_SEC );//time
	
	return 0;
}


























































