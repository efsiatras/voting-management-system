#include <stdlib.h>

#include "recordlist.h"

struct recordNode {
	recordPtr rec; // Pointer to record (in rbt)
	recordNodePtr nextr; // Pointer to next record node
};

int recordListPush(recordNodePtr *head, recordPtr re) { // Add record to start of record list
	recordNodePtr new = malloc(sizeof(struct recordNode)); //Malloc new code
	if (new == NULL) { // Check malloc failure

		return 1;
	}
	
	new->rec = re;
	new->nextr = *head;
	
	*head = new;

	return 0;
}

int recordListDelete(recordNodePtr *head, char *key) { // Delete record of given key
	recordNodePtr curr = *head;

	if ((curr != NULL) && (recordCmpKey(curr->rec, key) == 0)) { // Check if head node has the record we are looking for to delete
		*head = (*head)->nextr;
		free(curr);

		return 0;
	}

	recordNodePtr prev;
	while ((curr != NULL) && (recordCmpKey(curr->rec, key) != 0)) { // Iterate till we find the node with the record we are looking for to delete
		prev = curr;
		curr = curr->nextr;
	}

	if (curr == NULL) { // If node with the record we are looking for does not exist

		return 1;
	}

	// If node with the record we are looking for exists
	prev->nextr = curr->nextr; // Skip the node we are about to delete
	free(curr); // Delete node

	return 0;
}

void recordListFree(recordNodePtr head) { // Free record list ; Not freeing records, that is RBT's responsibility
	while (head != NULL) { // Iterate through list to free all nodes
		recordNodePtr curr = head;

		head = head->nextr;
		free(curr);
		curr=NULL;
	}
}

// int recordListCountVoted(recordNodePtr head, recordNodePtr rec) { // Count number of records that have voted
// 	int counter = 0; // Counter of number of records that have voted in record list
// 	recordNodePtr curr = head;

// 	while (curr != NULL) {
// 		if (recordHasVoted(curr->rec) == 1) { // Make sure that record has voted ; Not necessary because this list is only about records that have voted

// 			counter++;
// 		}

// 		curr = curr->nextr;
// 	}

// 	return counter;
// }