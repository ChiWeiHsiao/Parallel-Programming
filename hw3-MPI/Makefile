all:
  gcc -lm ori_prime.c -o ans_prime.out
  gcc -lm ori_integrate.c -o ans_integrate.out
  mpicc -lm prime.c -o prime.out 
  mpicc -lm integrate.c -o integrate.out 
clean:
	rm prime.out integrate.out ans_prime.out ans_integrate.out
