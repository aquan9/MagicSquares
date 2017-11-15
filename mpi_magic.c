/* A 4x4 magic square permutation checker using MPI and C */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAGIC_SUM 34
#define MAGIC_ORDER 4

void swap(int *x, int *y)
{
	if(x == y)
		return;
	//Bitwise XOR swap
	*x = *x ^ *y;
	*y = *y ^ *x;
	*x = *x ^ *y;
}


int sumVector(int *square, int idx, int stride)
{
	int i, sum = 0;
	for(i = 0; i < MAGIC_ORDER; i++) {
		sum += square[idx+(stride*i)];
	}
	return sum;
}

int isMagicSquare(int *square, int n)
{
	int i;

	// horizontal checks
	for(i = 0; i < n; i++) {
		if (sumVector(square, i*n, 1) != MAGIC_SUM)
			return 0;
	}

	// vertical checks
	for(i = 0; i < n; i++) {
		if (sumVector(square, i, n) != MAGIC_SUM)
			return 0;
	}

	// diagonal checks
	if (sumVector(square, 0, (n+1)) != MAGIC_SUM)
		return 0;
	if (sumVector(square, (n-1), (n-1)) != MAGIC_SUM)
		return 0;
	
	return 1;
}

int main(void)
{
	
	//Initialize MPI and get MPI world information
	int comm_sz; /* number of processes */
	int my_rank; /* my process rank */
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


	//Distributed implementation of non-recursive heap's algorithm in C
	//This will generate all the permutations of the sets of numbers
	//https://en.wikipedia.org/wiki/Heap%27s_algorithm
	
	int i = 0;
	int n = 16;
	int final_sum = 0;
	int *local_sums = malloc(sizeof(int) * n);
	int local_sum = 0;
	//int a[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	//int c[16];
	int c[16];
	int a[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	for(i = 0; i < n; i++){
		c[i] = 0;
	}

	//Set the last value of the array for a permutation category
	swap(&a[15], &a[my_rank]);
	//swap(&a[8], &a[my_rank]);
	
	//Check the first swapped state
	if(isMagicSquare(a, MAGIC_ORDER))
		local_sum += 1;
	
	//Keep going as long as the rank is the same as the last value of the array
	i = 0;
	while(i < n-1 && a[n-1] == (my_rank + 1)){
		if(c[i] < i ){
			if(i % 2 == 0){
				swap(&a[0],&a[i]);
			} else {
				swap(&a[c[i]], &a[i]);
			}
			//Output permutation here
			//Only print array if Magic square
			if(isMagicSquare(a, MAGIC_ORDER))
				local_sum += 1;
			
			c[i] += 1;
			i = 0;
		} else {
			c[i] = 0;
			i += 1;
		}
	}
	printf("Local sum = %d, rank = %d\n", local_sum, my_rank);
	MPI_Gather(&local_sum, 1, MPI_INT, local_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);	
	
	if(my_rank == 0){
	int b = 0;
	for(b = 0; b < n; b++){
		final_sum += local_sums[b];
	}

	printf("Final sum = %d\n", final_sum);
	}
	MPI_Finalize();
	return 0;
}	
