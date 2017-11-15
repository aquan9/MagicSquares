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
 */
void combinations(int *list, int choose, int size)
{
	int i = choose - 1; // the index of the last element in the initial selection
	int selections[choose];
	int b; // variable to iterate over the selection updates

	//Set the initial conditions for the selection array
	int a;
	for(a = 0; a <= choose; a++){
		selections[a] = a;
	}	
	

	printf("%d, %d, %d, %d\n", list[selections[0]], list[selections[1]], list[selections[2]], list[selections[3]]);
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

			i = choose - 1;
		}
		//Decrement i
		i--;
	} while(i >= 0);

	

}


int main(void)
{
	int *list = malloc(sizeof(int) * 8);
	int i; 
	for(i = 1; i <= 8; i++){
		list[i-1] = i;
	}
	combinations(list, 4, 8);
	return 0;
}
