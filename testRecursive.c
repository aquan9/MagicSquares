/*
 * Testing file for the recursive function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MNUM 3
#define MSIZE 9
#define MSUM 15


/*
 * Calculate the factorial of a number recursively
 * int n, the number to find the factorial of
 * return, the factorial
 */
int factorial(int n)
{
	if(n == 1)
		return n;
	else
		return n * factorial(n - 1);
}

/*
 * Sums the elements of an array
 * int *array, array to be summed
 * int startIdx, index to start summing
 * int tosum, number of elements 
 */
int sumArray(int *array, int startIdx, int tosum)
{
	int i, total = 0;
	for(i = startIdx; i < (startIdx+tosum); i++) {total += array[i];}
	return total;
}

/*
 * Remove element from array
 */
void removeElement(int **array, int *size, int element)
{
	int i, found = 0;
	for(i = 0; i < (*size-1); i++){
		if(!found && (*array)[i] == element)
			found = 1;
		if(found)
			(*array)[i] = (*array)[i+1];
	}
	if(!found)
		if((*array)[*size-1] == element)
			found = 1;
	if(found) {
		*array = realloc(*array, *size-1);
		*size -= 1;
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
	
	//Store once before doing all the others for fence post error
	//printf("%d, %d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]], list[selections[3]]);


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
			//Do something with a combination
			//printf("%d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]]);
			//Update selections
			selections[i] += 1;
			for(b = i + 1; b <= choose - 1; b++){
				selections[b] = selections[b - 1] + 1;
			}
			//printf("%d, %d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]], list[selections[3]]);
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
 * A function to print out all the permutations of a combination
 * int *comb, the combination to permutate
 * int size, size of list
 * int offend, number of varibles off the end of list to permutate
 * int **storage, a place to store all the permutations
 */
void permutations(int *comb, int size, int offend, int **storage)
{
	int a[offend];
	int *c = (int *) calloc(offend, sizeof(int));
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
	while(storage[storIdx] != NULL)
		memcpy(storage[storIdx++], comb, (size*sizeof(int)));
	storIdx = 1;
		
	// permutation loop
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
 */
int recursiveMagicSquare(int *array, int rank, int *square)
{
	int i,j,k;
	//ROW 1 is unpopulated
	if(square == NULL) {
		int num_perm = factorial(MNUM); //The number of permutations for 1 row
		//The number of combinations for the current row. 
		int num_comb = factorial(MSIZE) / (num_perm * factorial(MSIZE - MNUM));
		int **possibleCombinations = (int **) calloc(num_comb, sizeof(int *));
		for(i = 0; i < num_comb; i++){
			possibleCombinations[i] = (int *) calloc(MSIZE, sizeof(int));
		}
		combinations(array, MSIZE, MNUM, possibleCombinations);
		for(j = 0; j < num_comb; j++){
			if(sumArray(possibleCombinations[j], 0, MNUM) == MSUM) {
				//printArray(possibleCombinations[j], MSIZE);
				int **possiblePermutations = (int **) calloc(num_perm, sizeof(int *));
				for(k = 0; k < num_perm; k++){
					possiblePermutations[k] = (int *) calloc(MSIZE, sizeof(int));
				}
				permutations(possibleCombinations[j], MSIZE, MNUM, possiblePermutations);
				//for(k = 0; k < num_perm; k++){
				//	printArray(possiblePermutations[k], MSIZE);
				//}
				for(k = 0; k < num_perm; k++){
					recursiveMagicSquare(array, 0, possiblePermutations[k]);
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
		return 0;
	}	
	//ROW 2 in unpopulated
	if(square != NULL && square[MNUM] == 0) {
	
	}
	return 0;
	//if(row3 == NULL) {
	//	//gen array of all 8choose4 plus the rank number
	//	//eliminate invalid rows
	//	if(sumArray(row3, MNUM) == MSUM) {
	//		//permuate here
	//		//call recursiveMagicSquare with each permuation of each 8choose4
	//	}
	//	return 0;
	//}
	//if(row4 == NULL) {
	//	//see if last 4 numbers add to magic sum
	//	if(sumArray(row4, MNUM) == MSUM) {
	//		//permuate here
	//		//call recursiveMagicSquare with each permuation of the last 4
	//	}
	//	return;
	//}

	//if(isMagicSquare(row1, row2, row3, row4))
	//	globalTotal += 1;
}

int main(void)
{	
	int i;
	int *list = malloc(sizeof(int) * MSIZE);

	for(i = 1; i <= MSIZE; i++) {list[i-1] = i;}

	i = recursiveMagicSquare(list, 0, NULL);
	//printf("i = %d\n", i);

	free(list);
	return 0;
}
