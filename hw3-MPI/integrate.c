#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define PI 3.1415926535

int main(int argc, char **argv) 
{
	long long i, num_intervals;
	double rect_width, area, sum, x_middle; 
	long long start, end, partition;
	double par_sum;
	int size, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0)
		sscanf(argv[1],"%llu",&num_intervals);

	MPI_Bcast(&num_intervals, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	partition = (num_intervals - 1) / size;
	start = 1 + partition * rank;
	if( rank != size - 1)	end = start + partition;
	else end = num_intervals+1;//+1?
	rect_width = PI / num_intervals;

	par_sum = 0;
	for(i = start; i < end; i++) {//end+1?
		/* find the middle of the interval on the X-axis. */ 
		x_middle = (i - 0.5) * rect_width;
		area = sin(x_middle) * rect_width; 
		par_sum = par_sum + area;
	} 

	MPI_Reduce(&par_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if( rank == 0)
		printf("The total area is: %f\n", (float)sum);

	MPI_Finalize();
	return 0;
}   
