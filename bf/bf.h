#ifndef BF_H
#define BF_H

typedef struct bf *bfPtr;

bfPtr bfInit(int); // Initialize BF 
				   // Return pointer to BF 
				   // Number of records as argument

void bfFree(bfPtr); // Free BF

int bfLookup(bfPtr, char *); // Lookup key in BF 
							 // Return 1 if possibly found 
							 // Return 0 if not found

int bfInsert(bfPtr, char *); // Insert key in BF

#endif