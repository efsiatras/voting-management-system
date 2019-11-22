#ifndef RBT_H
#define RBT_H 

#include <stdio.h>

#include "../records/record.h"
#include "../bf/bf.h"

typedef struct rbtNode *rbtNodePtr;

rbtNodePtr rbtSearch(rbtNodePtr, char *); // Search RBT Tree as BST Search

void rbtInsert(rbtNodePtr *, recordPtr); // Insert record in RBT ; Return root

void rbtInsertFixUp(rbtNodePtr *, rbtNodePtr *); // Fix up tree to restore the red-black properties after insert

void rbtDelete(rbtNodePtr *, rbtNodePtr); // Delete record in RBT ; Return root

void rbtDeleteFixUp(rbtNodePtr *, rbtNodePtr *, rbtNodePtr *); // Fix up tree to restore the red-black properties after delete
															   // Parent of z needed in case rbtDeleteFixUp is called with z = NULL (leaf)

void rbtLeftRotate(rbtNodePtr *, rbtNodePtr); // Rotate record to the left

void rbtRightRotate(rbtNodePtr *, rbtNodePtr); // Rotate record to the right

void rbtTransplant(rbtNodePtr *, rbtNodePtr, rbtNodePtr); // Replace rbt node with another rbt node

rbtNodePtr rbtMinimum(rbtNodePtr); // Find node with minimum key

recordPtr rbtNodeGetRecord(rbtNodePtr); // Get record from RBT Node

void rbtInorderBFInsert(rbtNodePtr, bfPtr); // Insert RBT's records in BF in Inorder traversal

void rbtInorderFPrint(rbtNodePtr, FILE *);  // Print RBT's records in file in Inorder traversal

void rbtNodeFree(rbtNodePtr); // Free RBT Node

void rbtFree(rbtNodePtr); // Free RBT

#endif
