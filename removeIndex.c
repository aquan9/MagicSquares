#include <stdio.h>
#include <stdlib.h>

/*
 * Remove index from array
 */
void removeIdx(int **array, int *size, int index)
{
	int i, nsize;
	for(i = index; i < *size; i++) {
		array[0][i] = array[0][i+1];
	}
	array[0] = realloc(array[0], *size-1);
	nsize = *size-1;
	*size = nsize;
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

int main()
{
	int i;
	int size = 5;
	int *a = (int*) malloc(sizeof(int)*size);

	for(i = 0; i < size; i++) {
		a[i] = i+1;
	}

	printArray(a, size);

	removeIdx(&a, &size, 5);
	
	printArray(a, size);

	return 0;
}
