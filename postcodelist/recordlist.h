#ifndef RECORDNODE_H
#define RECORDNODE_H

#include "../records/record.h"

typedef struct recordNode *recordNodePtr;

int recordListPush(recordNodePtr *, recordPtr); // Add record to start of record list

int recordListDelete(recordNodePtr *, char *); // Delete record of given key

void recordListFree(recordNodePtr); // Free record list ; Not freeing records, that is RBT's responsibility

// int recordListCountVoted(recordNodePtr, recordNodePtr); // Count number of records that have voted

#endif