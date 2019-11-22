#include <stdlib.h>
#include <string.h>

#include "array.h"

char *arrayMallocCopy(char *arr) { // Malloc a new array, copy the given array and return the new copy
	if (arr == NULL) { // If the given array is empty

		return NULL;
	}

	char *newArray = malloc((strlen(arr)+1)*sizeof(char)); // Malloc new array
	if (newArray == NULL) { // Check malloc failure
		return NULL;
	}

	strcpy(newArray, arr);

	return newArray;
}