#include <stdlib.h>
#include <string.h>

#include "record.h"
#include "../utils/array.h"

struct record {
	char *id;
	char *name;
	char *surname;
	int age;
	char gender;
	int postcode;
	int hasvoted;
};

recordPtr recordInit(char *i, char *na, char *sur, int ag, char gen, int postc) { // Initialize record ; Return pointer to record ; data as arguments
	recordPtr r;
	r = malloc(sizeof(struct record)); // Malloc record struct
	if (r == NULL) { // Check malloc failure

		return NULL;
	}

	r->id = arrayMallocCopy(i);
	if (r->id == NULL) { // If malloc and copy failed
		recordFree(r); // Free what we have already malloc'ed

		return NULL;
	}

	r->name = arrayMallocCopy(na);
	if (r->name == NULL) { // If malloc and copy failed
		recordFree(r); // Free what we have already malloc'ed

		return NULL;
	}

	r->surname = arrayMallocCopy(sur);
	if (r->surname == NULL) { // If malloc and copy failed
		recordFree(r); // Free what we have already malloc'ed

		return NULL;
	}

	r->age = ag;
	r->gender = gen;
	r->postcode = postc;
	r->hasvoted = 0;

	return r;
}

void recordFree(recordPtr r) { // Free record
	if (r->id != NULL) {
		free(r->id);

		r->id = NULL;
	}

	if (r->name != NULL) {
		free(r->name);

		r->name = NULL;
	}

	if (r->surname != NULL) {
		free(r->surname);

		r->surname = NULL;
	}

	free(r);
	r = NULL;
}

void recordFPrint(recordPtr r, FILE *foutput) { // Print data of record in file

	fprintf(foutput, "%s  %s %s %d  %c %d\n", r->id, r->surname, r->name, r->age, r->gender, r->postcode);
}

void recordVote(recordPtr r) { // Indicate that record voted

	r->hasvoted = 1;
}

int recordHasVoted(recordPtr r) { // Return 1 if record has already voted ; Return 0 if not
	
	return r->hasvoted;
}

int recordGetPostcode(recordPtr r) { // Return postcode

	return r->postcode;
}

char *recordGetId(recordPtr r) { // Return ID

	return r->id;
}

int recordCmp(recordPtr r1, recordPtr r2) { // Compare key of records and return 1 if they are the same

	return strcmp(r1->id, r2->id);
}

int recordCmpKey(recordPtr r, char *k) { // Compare key of record and key and return 1 if they are the same

	return strcmp(r->id, k);
}