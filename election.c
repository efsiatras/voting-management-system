#include <stdlib.h>

#include "election.h"
#include "./bf/bf.h"
#include "./postcodelist/postcodelist.h"
#include "./rbt/rbt.h"
#include "./records/record.h"

struct election {
	bfPtr bf; // Bloom Filter
	postcodeNodePtr postcodeList; // Struct for postcodes
	rbtNodePtr rbt; // Red-Black Tree

	int numOfRecords; // Total number of records
	int numOfVoted; // Total number of records that voted

	int numOfUpdates; // Number of updates needed to reconstruct BF 
	int currUpdates; // Counting updates up to numOfUpdates to reconstruct BF
};

electionPtr electionInit(int predictedNumOfRecords, int numOfUpdates) { // Initialize election with predictedNumOfRecords and numOfUpdates parameters
	electionPtr e = malloc(sizeof(struct election));
	if (e == NULL) { // Check malloc failure

		return NULL;
	}

	e->bf = bfInit(predictedNumOfRecords);

	e->postcodeList = NULL;

	e->rbt = NULL;

	e->numOfRecords = 0;
	e->numOfVoted = 0;

	e->numOfUpdates = numOfUpdates;
	e->currUpdates = 0;

	return e;
}

electionPtr electionInit2(int predictedNumOfRecords) { // Initialize election only with predictedNumOfRecords parameter

	return electionInit(predictedNumOfRecords, 5);
}

electionPtr electionInit3(int numOfUpdates) { // Initialize election only with numOfUpdates parameter

	return electionInit(10, numOfUpdates);
}

electionPtr electionInit4() { // Initialize election with no parameters

	return electionInit(10, 5);
}

void electionFree(electionPtr e) { // Free election
	if (e->bf != NULL) {

		bfFree(e->bf);
	}

	if (e->postcodeList != NULL) {

		postcodeListFree(e->postcodeList);
	}

	if (e->rbt != NULL) {
		rbtFree(e->rbt);
	}

	free(e);
}

void electionLbf(electionPtr e, char *key) { // Lookup Bloom Filter
	if (bfLookup(e->bf, key)) { // If possibly found in BF

		printf("	# KEY %s POSSIBLY-IN REGISTRY\n", key);
	}

	else {

		printf("	# KEY %s Not-in-LBF\n", key);
	}
}

void electionLrb(electionPtr e, char *key) { // Lookup Red-Black Tree
	if (rbtSearch(e->rbt, key) != NULL) { // If found in RBT

		printf("	# KEY %s FOUND-IN-RBT\n", key);
	}

	else {

		printf("	# KEY %s NOT-IN-RBT\n", key);
	}
}

int electionIns(electionPtr e, char *id, char *name, char *surname, int age, char gender, int postcode, int flag) { // Insert record to all data structures of election ; flag = 0 if initial insert, flag = 1 if not
	if (rbtSearch(e->rbt, id) != NULL) { // If record with same id found

		return 1;
	}

	// If record with same id not found
	recordPtr r = recordInit(id, name, surname, age, gender, postcode);

	(e->numOfRecords)++; // Increase number of records
	(e->currUpdates) += flag; // If it was an initial insert, then it does not count as an update (flag = 0)

	postcodeListPushR(&(e->postcodeList), r); // Push into Postcode Struct

	rbtInsert(&(e->rbt), r); // Insert into RBT

	bfInsert(e->bf, id); // Insert into BF
	electionReconstructBF(e); // Check if BF needs to be restructed

	printf ("	# REC-WITH %s INSERTED-IN-BF-RBT\n", id);

	return 0;
}

void electionFind(electionPtr e, char *key) { // Find and print record
	rbtNodePtr node = rbtSearch(e->rbt, key);

	if (node != NULL) { // If found in RBT
		printf("	# REC-IS: ");
		recordFPrint(rbtNodeGetRecord(node), stdout); // Get record from RBT Node and print record
	}

	else {

		printf("	# REC-WITH %s NOT-in-structs\n", key);
	}
}

int electionDelete(electionPtr e, char *key) { // Delete record
	rbtNodePtr node = rbtSearch(e->rbt, key);

	if (node == NULL) { // If record not found

		return 1;
	}

	// If record found
	(e->numOfRecords)--; // Reduce total number of records
	if (recordHasVoted(rbtNodeGetRecord(node))) { // If record had voted
		
		(e->numOfVoted)--; // Reduce total number of people who have voted
	}

	(e->currUpdates)++; // Increase current number of updates

	postcodeListDeleteR(&(e->postcodeList), rbtNodeGetRecord(node)); // Delete from Postcode Struct

	rbtDelete(&(e->rbt), node); // Delete from RBT

	electionReconstructBF(e); // Check if BF needs to be restructed

	printf ("	# DELETED %s FROM-structs\n", key);

	return 0;
}

int electionVote(electionPtr e, char *key) { // Indicate that record voted
	rbtNodePtr node = rbtSearch(e->rbt, key);

	if (node == NULL) { // If not found in RBT

		return 1;
	}

	// If found in RBT
	recordPtr r = rbtNodeGetRecord(node);
	if (!recordHasVoted(r)) { // If record had not voted already
		(e->numOfVoted)++; // Increase total number of people who have voted
		recordVote(r);
		postcodeNodeVote(e->postcodeList, recordGetPostcode(r)); // Add 1 to total votes of postal code (that record belongs to) in postcodeList

		printf("	# REC-WITH %s SET-VOTED\n", key);
	}

	else { // If record had voted already

		printf("	# REC-WITH %s ALREADY-VOTED\n", key);			
	}

	return 0;
}

void electionVoted(electionPtr e) { // Print number of records that have voted

	printf("	# NUMBER %d\n", e->numOfVoted);
}

void electionVotedpc(electionPtr e, int postcode) { // Print number of records that have voted of specific postcode
	postcodeNodePtr p = postcodeNodeFindPC(e->postcodeList, postcode);

	if (p != NULL) {

		printf("	# IN %d VOTERS-ARE %d\n", postcode, postcodeGetNumOfVoted(p));
	}
}

void electionVotedperpc(electionPtr e) { // For every postcode, print percentage of people that have voted

	postcodeVotedperpc(e->postcodeList);
}

void electionReconstructBF(electionPtr e) { // Reconstruct BF if currUpdates >= numOfUpdates
	if (e->currUpdates >= e->numOfUpdates) { 
		bfFree(e->bf);
		e->bf = NULL;

		e->bf = bfInit(e->numOfRecords);
		rbtInorderBFInsert(e->rbt, e->bf);  // Insert RBT's records in BF in Inorder traversal

		e->currUpdates = 0;

		printf("Reconstructed BF with %d records.\n", e->numOfRecords);
	}
}

void electionFPrint(electionPtr e, FILE *foutput) { // Print records in output file 

	rbtInorderFPrint(e->rbt, foutput);
}