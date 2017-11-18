/*
 * Parallized recursive program to generate all NxN magic squares
 * Authors: Andres Quan and Charles Herrmann
 * Date: 11/17/17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#define MNUM 4
#define MSIZE 16
#define MSUM 34

/*Calculate the factorial of a number recursively
 * int n, the number to find the factorial of
 * return, the factorial
 */
long factorial(long n)
{
	if(n == 1)
		return n;
	else if (n == 0)
		return 1;
	else
		return n * factorial(n - 1);
}

/*Calculate the number of n choose k values
 * int n, the number to choose from
 * int k, the quantity to choose from
 * return, the n choose k values
 */
long nchoosek(long n, long k)
{
	return factorial(n) / (factorial(k) * factorial(n - k));
}

/*
 * Sums the elements of an array
 * int *array, array to be summed
 * int startIdx, index to start summing
 * int tosum, number of elements 
 * return, the sum of array
 */
int sumArray(int *array, int startIdx, int tosum)
{
	int i, total = 0;
	for(i = startIdx; i < (startIdx+tosum); i++) {total += array[i];}
	return total;
}

/*
 * Check if the given square is a valid Magic Square
 * int *square, the square to be checked
 * return, a 1 if vaild and 0 else
 */
int isMagicSquare(int *square)
{
	int i;
	//4x4 tests
	// vertical checks 
	for(i = 0; i < MNUM; i++) {
		if(square[i] + square[i + MNUM] + square[i + (MNUM * 2)] + square[i + (MNUM * 3)] != MSUM){
			return 0;
		}
	}

	// diagonal checks
	if (square[0] + square[1 + (MNUM)] + square[2 + (MNUM * 2)] + square[3 + (MNUM * 3)] != MSUM)
		return 0;
	if (square[0 + (MNUM * 3)] + square[1 + (MNUM * 2)] + square[2 + (MNUM)] + square[3] != MSUM)
		return 0;

	return 1;
}

/*
 * Remove element from array
 * int *array, the array with values to be removed
 * int size, the size of array
 * int *elements, the elements to be removed from array
 * int *new_array, store the kept numbers here
 */
void removeElements(int *array, int size, int *elements, int *new_array)
{
	int i,j,k = 0,f;
	for(i = 0; i < size; i++) {
		f = 1;
		for(j = 0; j < MSIZE; j++) {
			if(array[i] == elements[j])
				f = 0;
		}
		if(f)
			new_array[k++] = array[i];
	}
}

/*
 * A function to print out all the n choose k combinations of a list
 * int *list, the list to grab elements from
 * int size, the list size
 * int choose, the number of elements to grab
 * int **storage, a place to store all the combinations
 *
 * This function will offset the store of its values based on storage[0]. And append using this offset
 */
void combinations(int *list, int size, int choose, int **storage)
{
	int i = choose - 1; // the index of the last element in the initial selection
	int selections[choose];
	int b; // variable to iterate over the selection updates
	int store_pos = 0;
	//Set the initial conditions for the selection array
	int a;
	for(a = 0; a <= choose; a++){
		selections[a] = a;
	}
	
	//Calculate the offset, for where to store the elements
	//This will use the first element in storage for this purpose 
	int offset = 0;
	for(a = 0; a < MSIZE; a++){
		if(storage[0][a] != 0)
			offset = a + 1;	
	}
	if(offset == MSIZE){ //This means the array we were given is already full
		printf("The storage arrays are already full!");
		return; 
	}
	for(a = 0; a < choose; a++){
		storage[store_pos][a + offset] = list[selections[a]];
	}
	store_pos++;

	do {
		while(selections[i] < (size - choose + i)){
			//Update selections
			selections[i] += 1;
			for(b = i + 1; b <= choose - 1; b++){
				selections[b] = selections[b - 1] + 1;
			}
			for(a = 0; a < choose; a++){
				storage[store_pos][a + offset] = list[selections[a]];
			}
			store_pos++;

			i = choose - 1;
		}
		//Decrement i
		i--;
	} while(i >= 0);
}


/*
 * Simple swap function
 */
void swap(int *x, int *y)
{
	if(x == y)
		return;
	//Bitwise XOR swap
	*x = *x ^ *y;
	*y = *y ^ *x;
	*x = *x ^ *y;
}

/*
 * Print array
 * int *array, the array to print
 * int size, size of array to print
 */
void printArray(int *array, int size)
{
	int i;
	for(i = 0; i < size; i++) {
		printf("%d, ", array[i]);
	}
	printf("\n");
}

/*
 * Heap's Algorithm implementation
 * int *comb, the combination to permutate
 * int size, size of list
 * int offend, number of varibles off the end of list to permutate
 * int **storage, a place to store all the permutations
 */
void permutations(int *comb, int size, int offend, int **storage, int storeSize)
{
	int a[offend];
	int *c = (int *) calloc(offend, sizeof(int));
	//int c[offend];
	int offset = (size-1);
	int storIdx = 0;
	int i;
	
	// generate array of offend numbers off the end of comb
	while(comb[offset] == 0)
		offset--;
	offset += (1-offend);
	if(offset < 0) {
		printf("offset too low");
		return;
	}
	for(i = 0; i < offend; i++)
		a[i] = comb[offset+i];

	// setup storage
	for(i = 0; i < storeSize; i++) {
		memcpy(storage[i], comb, (size*sizeof(int)));
        }
		
	// permutation loop
        storIdx = 1;
	i = 0;	
	while(i < offend){
		if(c[i] < i){
			if(i % 2 == 0){
				swap(&a[0],&a[i]);
			} else {
				swap(&a[c[i]], &a[i]);
			}
			// copy permutation into storage
			memcpy(&storage[storIdx++][offset], a, (offend*sizeof(int)));
			c[i] += 1;
			i = 0;
		} else {
			c[i] = 0;
			i += 1;
		}
	}
	
	free(c);
}

/* build rows, eliminate invalid rows, permutate, repeat
 * int *array, the array of elements not yet choosen to be in a row
 * int rank, the process rank
 * int *square, the square so far
 * return, sum of magic numbers found
 */
int recursiveMagicSquare(int *array, int size, int *square)
{
	int i,j,k;
	//The total magic squares found
	int sum = 0; 
	// ROW1 is unpopulated
	if(square != NULL && !square[1]) {
		int num_perm = factorial(MNUM - 1); //The number of permutations for 1 row
		//The number of combinations for the current row. 
		int num_comb = nchoosek(MSIZE - 1, MNUM - 1);
		int **possibleCombinations = (int **) calloc(num_comb, sizeof(int *));
		for(i = 0; i < num_comb; i++){
			possibleCombinations[i] = (int *) calloc(MSIZE, sizeof(int));
                        possibleCombinations[i][0] = square[0];
		}
                int *new_array = (int *) malloc(sizeof(int) * (MSIZE-1));
                removeElements(array, size, square, new_array);

		combinations(new_array, size - 1, MNUM - 1, possibleCombinations);

		for(j = 0; j < num_comb; j++){
			if(sumArray(possibleCombinations[j], 0, MNUM) == MSUM) {
				int **possiblePermutations = (int **) calloc(num_perm, sizeof(int *));
				for(k = 0; k < num_perm; k++){
					possiblePermutations[k] = (int *) calloc(MSIZE, sizeof(int));
				}
				permutations(possibleCombinations[j], MSIZE, MNUM - 1, possiblePermutations, num_perm);
				for(k = 0; k < num_perm; k++){
					sum += recursiveMagicSquare(new_array, size, possiblePermutations[k]);
				}
				for(k = 0; k < num_perm; k++) {
					free(possiblePermutations[k]);
				}
				free(possiblePermutations);
			}
		}
		for(i = 0; i < num_comb; i++) {
			free(possibleCombinations[i]);
		}
		free(possibleCombinations);
                free(new_array);
		return sum;
	}	
	// ROW2 is unpopulated
	if(square != NULL && square[MNUM] == 0) {
		int *new_array = (int *) malloc(sizeof(int) * (MSIZE-MNUM));
		removeElements(array, size - 1, square, new_array);
		int num_perm = factorial(MNUM); //The number of permutations for 1 row
		//The number of combinations for the current row. 
		int num_comb = nchoosek(MSIZE-MNUM, MNUM);
		int **possibleCombinations = (int **) calloc(num_comb, sizeof(int *));
		for(i = 0; i < num_comb; i++){
			possibleCombinations[i] = (int *) calloc(MSIZE, sizeof(int));
			memcpy(possibleCombinations[i], square, (MSIZE*sizeof(int)));
		}
		combinations(new_array, size-MNUM, MNUM, possibleCombinations);
		for(j = 0; j < num_comb; j++){
			if(sumArray(possibleCombinations[j], MNUM, MNUM) == MSUM) {
				int **possiblePermutations = (int **) calloc(num_perm, sizeof(int *));
				for(i = 0; i < num_perm; i++){
					possiblePermutations[i] = (int *) calloc(MSIZE, sizeof(int));
				}
				permutations(possibleCombinations[j], MSIZE, MNUM, possiblePermutations, num_perm);
				for(k = 0; k < num_perm; k++){
					sum += recursiveMagicSquare(new_array, size-MNUM, possiblePermutations[k]);
				}
				for(i = 0; i < num_perm; i++) {
					free(possiblePermutations[i]);
				}
				free(possiblePermutations);
			}
		}
		for(i = 0; i < num_comb; i++) {
			free(possibleCombinations[i]);
		}
		free(possibleCombinations);
                free(new_array);

		return sum;
	}
	//ROW 3 is unpopulated
	if(square != NULL && square[MNUM * 2] == 0) {
		int *new_array = (int *) malloc(sizeof(int) * (MSIZE-(MNUM * 2)));
		removeElements(array, size, square, new_array);
		int num_perm = factorial(MNUM); //The number of permutations for 1 row
		//The number of combinations for the current row. 
		int num_comb = nchoosek(MSIZE-(MNUM * 2), MNUM);
		int **possibleCombinations = (int **) calloc(num_comb, sizeof(int *));
		for(i = 0; i < num_comb; i++){
			possibleCombinations[i] = (int *) calloc(MSIZE, sizeof(int));
			memcpy(possibleCombinations[i], square, (MSIZE*sizeof(int)));
		}
		combinations(new_array, size-MNUM, MNUM, possibleCombinations);
		for(j = 0; j < num_comb; j++){
			if(sumArray(possibleCombinations[j], (MNUM * 2), MNUM) == MSUM) {
				int **possiblePermutations = (int **) calloc(num_perm, sizeof(int *));
				for(i = 0; i < num_perm; i++){
					possiblePermutations[i] = (int *) calloc(MSIZE, sizeof(int));
				}
				permutations(possibleCombinations[j], MSIZE, MNUM, possiblePermutations, num_perm);
				for(k = 0; k < num_perm; k++){
					sum += recursiveMagicSquare(new_array, size-MNUM, possiblePermutations[k]);
				}
				for(i = 0; i < num_perm; i++) {
					free(possiblePermutations[i]);
				}
				free(possiblePermutations);
			}
		}
		for(i = 0; i < num_comb; i++) {
			free(possibleCombinations[i]);
		}
		free(possibleCombinations);
                free(new_array);

		return sum;
	}
	//Row 4 unpopulated
	if(square != NULL && square[MNUM * 3] == 0) {
		int *new_array = (int *) malloc(sizeof(int) * (MSIZE-(MNUM * 3)));
		removeElements(array, size, square, new_array);
		int num_perm = factorial(MNUM); //The number of permutations for 1 row
		//The number of combinations for the current row. 
		int num_comb = nchoosek(MSIZE-(MNUM * 3), MNUM);
		int **possibleCombinations = (int **) calloc(num_comb, sizeof(int *));
		for(i = 0; i < num_comb; i++){
			possibleCombinations[i] = (int *) calloc(MSIZE, sizeof(int));
			memcpy(possibleCombinations[i], square, (MSIZE*sizeof(int)));
		}
		combinations(new_array, size-MNUM, MNUM, possibleCombinations);
		for(j = 0; j < num_comb; j++){
			if(sumArray(possibleCombinations[j], (MNUM * 3), MNUM) == MSUM) {
				int **possiblePermutations = (int **) calloc(num_perm, sizeof(int *));
				for(i = 0; i < num_perm; i++){
					possiblePermutations[i] = (int *) calloc(MSIZE, sizeof(int));
				}
				permutations(possibleCombinations[j], MSIZE, MNUM, possiblePermutations, num_perm);
				//Check to see if its a 4x4 magic square
				for(k = 0; k < num_perm; k++){
					if(isMagicSquare(possiblePermutations[k])){
						sum += 1;
					}
				}
				for(i = 0; i < num_perm; i++) {
					free(possiblePermutations[i]);
				}
				free(possiblePermutations);
			}
		}
		for(i = 0; i < num_comb; i++) {
			free(possibleCombinations[i]);
		}
		free(possibleCombinations);
                free(new_array);
		
		return sum;
	}
	return 0;
}

int main(void)
{	
        
	//Initialize MPI and get MPI world information
	int comm_sz; /* number of processes */
	int my_rank; /* my process rank */
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int i = 0;
	int n = 16;
	int final_sum = 0;
	int *local_sums = malloc(sizeof(int) * n);
	int local_sum = 0;

	int *list = malloc(sizeof(int) * MSIZE);

	for(i = 1; i <= MSIZE; i++) {list[i-1] = i;}
	//printf("Magic Squares\n");

        int *initial_square = (int *) calloc(MSIZE,sizeof(int));
        initial_square[0] = my_rank + 1;
	local_sum = recursiveMagicSquare(list, MSIZE, initial_square);

	free(list);

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
        free(local_sums);
        return 0;

}
