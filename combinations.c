/*
 * Work on a n choose k generator
 * algorithm from planetcalc
 *
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * A function to print out all the n choose k combinations of a list
 * int *list, the list to grab elements from
 * int choose, the number of elements to grab
 * int size, the list size
 * int **storage, a place to store all the combinations
 *
 * This function will offset the store of its values based on storage[0]. And append using this offset
 */
void combinations(int *list, int choose, int size, int **storage)
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
	printf("%d, %d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]], list[selections[3]]);


	//Calculate the offset, for where to store the elements
	//This will use the first element in storage for this purpose 
	int offset = 0;
	for(a = 0; a < 16; a++){
		if(storage[0][a] != 0)
			offset = a + 1;	
	}
	if(offset == 16){ //This means the array we were given is already full
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
			printf("%d, %d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]], list[selections[3]]);
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


int main(void)
{
	int i;
        //Allocate enough space for a 2D array of possible combinations
	//This will also clear that memory	
	int **possibleCombinations = (int **) calloc(1820, sizeof(int *));
	for(i = 0; i < 1820; i++){
		possibleCombinations[i] = (int *) calloc(16, sizeof(int));
	}
	
	int *list = malloc(sizeof(int) * 16);
	for(i = 1; i <= 16; i++){
		list[i-1] = i;
	}

	//Check to make sure the combinations function will offset its values
	//possibleCombinations[0][0] = 58;
	combinations(list, 4, 16, possibleCombinations);

	
	
	

	printf("the possible combinations\n");
	int b = 0;
	while(b < 1820){
		for(i = 0; i < 16; i++){
			printf("%d,", possibleCombinations[b][i]);
		}
		printf("\n");
		b++;
	}

	return 0;
}
