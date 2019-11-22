#ifndef ELECTION_H
#define ELECTION_H

#include <stdio.h>

typedef struct election *electionPtr;

electionPtr electionInit(int, int); // Initialize election with predictedNumOfRecords and numOfUpdates parameters

electionPtr electionInit2(int); // Initialize election only with predictedNumOfRecords parameter

electionPtr electionInit3(int); // Initialize election only with numOfUpdates parameter

electionPtr electionInit4(); // Initialize election with no parameters

void electionFree(electionPtr); // Free election

void electionLbf(electionPtr, char *key); // Lookup Bloom Filter

void electionLrb(electionPtr, char *key); // Lookup Red-Black Tree

int electionIns(electionPtr, char *, char *, char *, int, char, int, int); // Insert record to all data structures of election
																		   // Last parameter flag = 0 if initial insert, flag = 1 if not

void electionFind(electionPtr, char *key); // Find and print record

int electionDelete(electionPtr, char *key); // Delete record

int electionVote(electionPtr, char *key); // Indicate that record voted

void electionVoted(electionPtr); // Print number of records that have voted

void electionVotedpc(electionPtr, int); // Print number of records that have voted of specific postcode

void electionVotedperpc(electionPtr); // For every postcode, print percentage of people that have voted

void electionReconstructBF(electionPtr);  // Reconstruct BF if currUpdates >= numOfUpdates

void electionFPrint(electionPtr, FILE *); // Print records in output file

#endif