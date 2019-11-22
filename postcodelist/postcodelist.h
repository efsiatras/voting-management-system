#ifndef POSTCODENODE_H
#define POSTCODENODE_H

#include "recordlist.h"

typedef struct postcodeNode *postcodeNodePtr;

int postcodeListPushR(postcodeNodePtr *, recordPtr); // Add record to record list of its postcode
													 // Add postcode to postcode list if it does not exist already 

int postcodeListDeleteR(postcodeNodePtr *, recordPtr); // Delete record from record list of its postcode
													   // Delete postcode from postcode list if it has no records anymore 

void postcodeListFree(postcodeNodePtr); // Free postcode list and record lists ; Not freeing records, that is RBT's responsibility

postcodeNodePtr postcodeNodeFindPC(postcodeNodePtr, int); // Return pointer to postcodeNode if found in postcode list 
														  // Else return NULL

int postcodeNodeVote(postcodeNodePtr, int); // Find postcodeNode in postcode list and increase number of records of specific postcode that have voted, return 0
											// If postcodeNode not found, error return 1

int postcodeGetNumOfVoted(postcodeNodePtr); // Return numOfVoted of given postcode node

void postcodeVotedperpc(postcodeNodePtr); // Print percentage of voted for each postcode

#endif