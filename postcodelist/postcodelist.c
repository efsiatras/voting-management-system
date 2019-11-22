#include <stdlib.h>

#include "postcodelist.h"

struct postcodeNode {
	int postcode; // Postcode number
	recordNodePtr recs; // Pointer to list of records of specific postcode
	postcodeNodePtr nextp; // Pointer to next postcode node

	int numOfRecords; // Number of records of specific postcode
	int numOfVoted; // Number of records that have voted of specific postcode
};

int postcodeListPushR(postcodeNodePtr *head, recordPtr re) { // Add record to record list of its postcode
	if (re == NULL) { // If given pointers are empty, error

		return 1;
	}

	postcodeNodePtr foundNode = postcodeNodeFindPC(*head, recordGetPostcode(re)); // Check if postcode exists already in postcode list
	
	if (foundNode != NULL) { // If it exists
		if (recordListPush(&(foundNode->recs), re) == 1) { // Add record to start of record list

			return 1;
		}

		(foundNode->numOfRecords)++; // Increase number of records of specific postcode
	}

	else { // If it does not exist
		postcodeNodePtr new = malloc(sizeof(struct postcodeNode)); // Malloc new code
		if (new == NULL) { // Check malloc failure

			return 1;
		}
		new->recs = NULL;
	
		if (recordListPush(&(new->recs), re) == 1) { // Add record to start of record list
			postcodeListFree(new); // Free what we have already malloc'ed

			return 1;
		}

		new->nextp = *head;
		new->postcode = recordGetPostcode(re);
		new->numOfRecords = 1;
		new->numOfVoted = 0;

		*head = new;
	}

	return 0;
}

int postcodeListDeleteR(postcodeNodePtr *head, recordPtr re) { // Delete record from record list of its postcode
	if (re == NULL) { // If given pointers are empty, error

		return 1;
	}

	postcodeNodePtr foundNode = postcodeNodeFindPC(*head, recordGetPostcode(re)); // Check if postcode exists already in postcode list
	
	if (foundNode == NULL) { // If it does not exist
	
		return 1;
	}

	// If it exists
	if (recordListDelete(&(foundNode->recs), recordGetId(re))) { // Delete record from record list and failure check

		return 1;
	}

	(foundNode->numOfRecords)--; // Reduce number of records of specific postcode
	(foundNode->numOfVoted) -= recordHasVoted(re); // Reduce number of records of specific postcode that have voted, if specific record had voted

	if ((*head)->recs == NULL) { // If record list is empty, we will delete the postcode node
		postcodeNodePtr curr = *head;

		if ((curr != NULL) && (curr->postcode == recordGetPostcode(re))) { // Check if head node of postcode list has the postcode we are looking for to delete
			*head = (*head)->nextp;
			free(curr);

			return 0;
		}

		postcodeNodePtr prev;
		while ((curr != NULL) && (curr->postcode != recordGetPostcode(re))) { // We have to iterate again to find the PREVIOUS node of the one we are looking for to delete
			prev = curr;
			curr = curr->nextp;
		}

		if (curr == NULL) { // If node of the postcode we are looking for does not exist (It exists, double check just in case)

			return 1;
		}

		// If node of the postcode we are looking for exists
		prev->nextp = curr->nextp; // Skip the node we are about to delete
		free(curr); // Delete node
	}

	return 0;
}

void postcodeListFree(postcodeNodePtr head) { /// Free postcode list and record lists ; Not freeing records, that is RBT's responsibility
	while (head != NULL) { // Iterate through list to free all nodes
		postcodeNodePtr curr = head;

		head = head->nextp;
		if (curr->recs != NULL) {

			recordListFree(curr->recs); // Free every record list of every postcode
		}
		
		free(curr);
		curr = NULL;
	}
}

postcodeNodePtr postcodeNodeFindPC(postcodeNodePtr head, int postc) { // Return pointer to postcodeNode if found in postcode list ; Else return NULL
	postcodeNodePtr curr = head;
	while (curr != NULL) { // Iterate through list
		if (curr->postcode == postc) { // If we found the postcode already in list
			
			return curr; // Return pointer to postcodeNode
		}

		curr = curr->nextp;
	}

	return NULL; // Else return NULL
}

int postcodeNodeVote(postcodeNodePtr head, int postc) { // Find postcodeNode in postcode list and increase number of records of specific postcode that have voted
	if (head == NULL) { // If given pointer is empty, error

		return 1;
	}

	postcodeNodePtr foundNode = postcodeNodeFindPC(head, postc); // Check if postcode exists in postcode list
	
	if (foundNode != NULL) { // If it exists
		(foundNode->numOfVoted)++; // Increase number of records of specific postcode that have voted

		return 0;
	}

	return 1; // If it does not exist, error
}

int postcodeGetNumOfVoted(postcodeNodePtr p) { // Return numOfVoted of given postcode node
	if (p != NULL) {

		return p->numOfVoted;
	}

	return -1; // p == NULL
}

void postcodeVotedperpc(postcodeNodePtr head) { // Print percentage of voted for each postcode
	postcodeNodePtr curr = head;

	while (curr != NULL) { // Iterate through list to print all percentages
		float perc = ((float) curr->numOfVoted / (float) curr->numOfRecords) * 100.0;

		printf("	# IN %d VOTERS-ARE %.4f %%\n", curr->postcode, perc);

		curr = curr->nextp;
	}
}