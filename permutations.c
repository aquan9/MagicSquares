/*
 * Work on a special case permutator
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void)
{
	//pPSize is possiblePermutations (Ex. 4 would be 4! possible permutations. 4! = 24)
	int i, pPSize = 6, aSize = 9, rowSize = 3;

        //Allocate enough space for a 2D array of possible permutations
	//This will also clear that memory
	int **possiblePermutations = (int **) calloc(pPSize, sizeof(int *));
	for(i = 0; i < pPSize; i++){
		possiblePermutations[i] = (int *) calloc(aSize, sizeof(int));
	}
	
	// init input array
	int *comb = calloc(aSize, sizeof(int));
	for(i = 0; i <= aSize; i++){
		if(i > 2 && i < 6)
			comb[i-1] = i;
	}

	printf("Combination in: ");
	printArray(comb, aSize);

	permutations(comb, aSize, rowSize, possiblePermutations);

	printf("The possible combinations:\n");
	for(i = 0; i < pPSize; i++)
		printArray(possiblePermutations[i], aSize);

	return 0;
}
