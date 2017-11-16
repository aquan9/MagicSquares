#include <stdio.h>
#include <stdlib.h>

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
	int size = 10;
	int *a = (int*) malloc(sizeof(int)*size);

	for(i = 0; i < size; i++) {
		a[i] = i+1;
	}

	printArray(a, size);

	i = 0;
	while(i <= 10){
		removeElement(&a, &size, i++);
		printArray(a, size);
	}
	
	return 0;
}
