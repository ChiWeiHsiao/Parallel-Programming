#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int isprime(int n) {
	int i,squareroot;
	if (n>10) {
		squareroot = (int) sqrt(n);
		for (i=3; i<=squareroot; i=i+2)
			if ((n%i)==0)
				return 0;
		return 1;
	}
	else
		return 0;
}

int main(int argc, char *argv[])
{
	int pc,       /* prime counter */
	    foundone; /* most recent prime found */
	long long int n, limit;
	long long start, end, partition;
	int par_pc = 0, par_foundone; 
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0){
		sscanf(argv[1],"%llu",&limit);	
		printf("Starting. Numbers to be scanned= %lld\n",limit);

		pc=0;     /* Assume (2,3,5,7) are counted here */
	}

	MPI_Bcast(&limit, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	partition = (limit - 11 - 1) / size;//-1?
	start = 11 + partition * rank;
	if( rank != size - 1)	end = start + partition;
	else end = limit;//+1?

	if( start%2 == 0) start = start + 1;//should check from a odd num, becouse of n = n+2
	for ( n = start; n <= end; n = n+2) {
		if (isprime(n)) {
			par_pc++;
			par_foundone = n;
		}			
	}

	MPI_Reduce(&par_pc, &pc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&par_foundone, &foundone, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if(rank==0){
		pc = pc+4;
		printf("Done. Largest prime is %d Total primes %d\n",foundone,pc);
	}

	MPI_Finalize();

	return 0;
} 
