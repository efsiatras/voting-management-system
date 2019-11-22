#include <stdlib.h>
#include <string.h>

#include "bf.h"
#include "../utils/primenumber.h"

struct bf { // Bloom Filter
	unsigned char *bitString; // Bit array
	int bits; // Number of bits
};

bfPtr bfInit(int numberOfRecords) { // Initialize BF ; Return pointer to BF ; Number of records as argument
	bfPtr b;
	b = malloc(sizeof(struct bf)); // Malloc bloom filter struct
	if (b == NULL) { // Check malloc failure

		return NULL;
	}

	int n = numberOfRecords * 3; // Potential number of bits
	if (isPrime(n) == 1) { // If n is prime number

		b->bits = n; // Set n as number of bits
	}

	else {

		b->bits = nextPrime(n); // Set number of bits as next prime number closest to n
	}

	b->bitString = malloc(b->bits * sizeof(unsigned char)); // Malloc bit array
	if (b->bitString == NULL) { // Check malloc failure
		bfFree(b);  // Free what we have already malloc'ed

		return NULL;
	}

	for (int i = 0; i < b->bits; i++) { // Initialize bit array to 0

		b->bitString[i] = 0;
	}

	return b;
}

void bfFree(bfPtr b) { // Free BF
	if (b->bitString != NULL) {
		free(b->bitString);

		b->bitString = NULL;
	}

	free(b);
	b = NULL;
}


int h1(char *key, int max) { // Hash function h1 ; djb2 hash function ; http://www.cs.yorku.ca/~oz/hash.html
	unsigned long int hash = 5381;

	while (*key != '\0') {
		hash = ((hash << 5) + hash) + *key; /* hash * 33 + c */
		key++;
	}

	return hash % max;
}

int h2(char *key, int max) { // Hash function h2 ; sdbm hash function ; http://www.cs.yorku.ca/~oz/hash.html
	unsigned long int hash = 0;

	while (*key != '\0'){
		hash = *key + (hash << 6) + (hash << 16) - hash;
		key++;
	}


	return hash % max;
}

int h3(char *key, int max) { // Hash function h3 ; Combination of h1 and h2

	return (h1(key, max) + 2 * h2(key, max)) % max;
}

int bfLookup(bfPtr b, char *key) { // Lookup key in BF ; Return 1 if possibly found ; Return 0 if not found
	int pos1 = h1(key, b->bits); // Calculate digest of h1
	int pos2 = h2(key, b->bits); // Calculate digest of h2
	int pos3 = h3(key, b->bits); // Calculate digest of h3

	if ((b->bitString[pos1] == 1) && (b->bitString[pos2] == 1) && (b->bitString[pos3] == 1)) {
		// If all positions are set to 1, filter returns 1
		return 1;
	}

	else {
		// If not, filter returns 0
		return 0;
	}
}

int bfInsert(bfPtr b, char *key) { // Insert key in BF
	if ((b == NULL) || (key == NULL)) { // If given pointers are empty, error

		return 1;
	}
	
	int pos1 = h1(key, b->bits); // Calculate digest of h1
	int pos2 = h2(key, b->bits); // Calculate digest of h2
	int pos3 = h3(key, b->bits); // Calculate digest of h3

	// Set all positions to 1
	b->bitString[pos1] = 1;
	b->bitString[pos2] = 1;
	b->bitString[pos3] = 1;

	return 0;
}