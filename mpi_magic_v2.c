/* A 4x4 magic square permutation checker using MPI and C */
/* Generates rows and permutates to form magic squares*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAGIC_SUM 34
#define MAGIC_ORDER 4

int globalTotal = 0;

/* generates permutations  */
int **permutateRow()

void swap(int *x, int *y)
{
	if(x == y)
		return;
	//Bitwise XOR swap
	*x = *x ^ *y;
	*y = *y ^ *x;
	*x = *x ^ *y;
}

bool isMagicSquare(int *row1, int *row2, int *row3, int *row4)
{
	int i;
	// Horizontal checks unsued because rows will always add to magic sum
	// horizontal checks
	//for(i = 0; i < n; i++) {
	//	if (sumVector(square, i*n, 1) != MAGIC_SUM)
	//		return 0;
	//}

	// vertical checks
	for(i = 0; i < n; i++) {
		if(row1[i]+row2[i]+row3[i]+row4[i] != MAGIC_SUM)
			return false;
	}

	// diagonal checks
	if (row1[0]+row2[1]+row3[2]+row4[3]  != MAGIC_SUM)
		return false;
	if (row1[3]+row2[2]+row3[1]+row4[0]  != MAGIC_SUM)
		return false;
	
	return true;
}

/* build rows, eliminate invalid rows, permutate, repeat */
int recursiveMagicSquare(int *array, int rank, int *row1, int *row2, int *row3, int *row4)
{
	if(row1 == NULL) {
		//gen array of all 15choose3 plus the rank number
		//eliminate invalid rows
		if(sumVector(row1, 0, 1) == MAGIC_SUM) {
			//permuate here
			//call recursiveMagicSquare with each permuation of each 15choose3
		}
		return;
	}	
	if(row2 == NULL) {
		//gen array of all 12choose4
		//eliminate invalid rows
		if(sumVector(row2, 0, 1) == MAGIC_SUM) {
			//permuate here
			//call recursiveMagicSquare with each permuation of each 12choose4
		}
		return;
	}
	if(row3 == NULL) {
		//gen array of all 8choose4 plus the rank number
		//eliminate invalid rows
		if(sumVector(row3, 0, 1) == MAGIC_SUM) {
			//permuate here
			//call recursiveMagicSquare with each permuation of each 8choose4
		}
		return;
	}
	if(row4 == NULL) {
		//see if last 4 numbers add to magic sum
		if(sumVector(row4, 0, 1) == MAGIC_SUM) {
			//permuate here
			//call recursiveMagicSquare with each permuation of the last 4
		}
		return;
	}

	if(isMagicSquare(row1, row2, row3, row4))
		globalTotal += 1;
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
	int a[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	recursiveMagicSquare(a, my_rank, NULL, NULL, NULL, NULL);

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