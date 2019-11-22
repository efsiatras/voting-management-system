#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>

typedef struct record *recordPtr;

recordPtr recordInit(char *, char *, char *, int, char, int); // Initialize record 
															  // Return pointer to record 
															  // Data as arguments

void recordFree(recordPtr); // Free record

void recordFPrint(recordPtr, FILE *); // Print data of record in file

void recordVote(recordPtr); // Indicate that record voted

int recordHasVoted(recordPtr); // Return 1 if record has already voted 
							   // Return 0 if not

int recordGetPostcode(recordPtr); // Return postcode

char *recordGetId(recordPtr); // Return ID

int recordCmp(recordPtr, recordPtr); // Compare keys of records 
									// Return value < 0 if key1 is less than key2
									// Return 0 if equal
									// Return value > 0 if key1 is more than key2

int recordCmpKey(recordPtr, char *); // Compare key of record and key
								  // Return value < 0 if key1 is less than key2
								  // Return 0 if equal
								  // Return value > 0 if key1 is more than key2

#endif