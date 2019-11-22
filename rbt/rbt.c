#include <stdlib.h>

#include "rbt.h"

enum col {RED, BLACK};

struct rbtNode {
	recordPtr record; // Pointer to record

	enum col colour; // Colour of node: RED or BLACK
	rbtNodePtr parent; // Parent node
	rbtNodePtr left; // Left child node
	rbtNodePtr right; // Right child node
};

rbtNodePtr rbtSearch(rbtNodePtr t, char *k) { // Search RBT Tree using BST Search's logic
	if (t == NULL) {

		return NULL;
	}

	int cmp = recordCmpKey(t->record, k); // Compare node record's key with given key

	if (cmp == 0) {

		return t;
	}

	else if (cmp > 0) {

		return rbtSearch(t->left, k);
	}

	else { // cmp < 0

		return rbtSearch(t->right, k);
	}
}

void rbtInsert(rbtNodePtr *T, recordPtr r) {  // Insert record in RBT
	rbtNodePtr z = malloc(sizeof(struct rbtNode)); // Malloc new rbt node
	
	z->record = r;

	rbtNodePtr y = NULL;
	rbtNodePtr x = *T;

	while (x != NULL) {
		y = x;

		if (recordCmp(z->record, x->record) < 0) { // z->key < x->key
			
			x = x->left;
		}

		else { // z->key > x->key

			x = x->right;
		}
	}
	z->parent = y;

	if (y == NULL) {

		*T = z;
	}

	else if (recordCmp(z->record, y->record) < 0) { // z->key < y->key

		y->left = z;
	}

	else { // z->key > x->key

		y->right = z;
	}

	z->left = NULL;
	z->right = NULL;
	z->colour = RED;

	rbtInsertFixUp(T, &z);
}

void rbtInsertFixUp(rbtNodePtr *T, rbtNodePtr *z) { // Fix up tree to restore the red-black properties after insert
	while ((*z != *T) && ((*z)->colour == RED) && ((*z)->parent->colour == RED)) {
		if ((*z)->parent == (*z)->parent->parent->left) {
			rbtNodePtr y = (*z)->parent->parent->right;

			if ((y != NULL) && (y->colour == RED)) { // Case 1: z, z's parent and z's right uncle are red
				(*z)->parent->colour = BLACK;
				y->colour = BLACK;
				(*z)->parent->parent->colour = RED;

				*z = (*z)->parent->parent;
			}

			else {
				if ((*z) == (*z)->parent->right) { // Case 2: z's right uncle is black and z is a right child
					*z = (*z)->parent;

					rbtLeftRotate(T, *z);
				}

				// Case 3: z's right uncle is black and z is a left child
				(*z)->parent->colour = BLACK;
				(*z)->parent->parent->colour = RED;

				rbtRightRotate(T, ((*z)->parent->parent));
			}
		}

		else {
			rbtNodePtr y = (*z)->parent->parent->left;

			if ((y != NULL) && (y->colour == RED)) { // Case 1: z, z's parent and z's left uncle are red
				(*z)->parent->colour = BLACK;
				y->colour = BLACK;
				(*z)->parent->parent->colour = RED;

				*z = (*z)->parent->parent;
			}

			else {
				if ((*z) == (*z)->parent->left) { // Case 2: z's left uncle is black and z is a left child
					*z = (*z)->parent;

					rbtRightRotate(T, *z);
				}

				// Case 3: z's left uncle is black and z is a right child
				(*z)->parent->colour = BLACK;
				(*z)->parent->parent->colour = RED;

				rbtLeftRotate(T, ((*z)->parent->parent));
			}
		}
	}

	(*T)->colour = BLACK;
}

void rbtDelete(rbtNodePtr *T, rbtNodePtr z) { // Delete record in RBT
	rbtNodePtr toFree = z; // Keep pointer to deleted node to free in the end

	rbtNodePtr y = z;
	enum col yOriginalColour = y->colour;

	rbtNodePtr x;
	rbtNodePtr xParent; // Parent of x ; Needed in case x = NULL (leaf)

	if (z->left == NULL) {
		x = z->right;
		xParent = z;

		rbtTransplant(T, z, z->right);
	}

	else if (z->right == NULL) {
		x = z->left;
		xParent = z;

		rbtTransplant(T, z, z->left);
	}

	else {
		y = rbtMinimum(z->right);

		yOriginalColour = y->colour;
		x = y->right;
		xParent = y;

		if (y->parent == z) {

			xParent = y;
		}

		else {
			rbtTransplant(T, y, y->right);

			y->right = z->right;
			y->right->parent = y;
		}

		rbtTransplant(T, z, y);

		y->left = z->left;
		y->left->parent = y;
		y->colour = z->colour;
	}

	if (yOriginalColour == BLACK) {

		rbtDeleteFixUp(T, &x, &xParent);
	}

	rbtNodeFree(toFree);
}

void rbtDeleteFixUp(rbtNodePtr *T, rbtNodePtr *z, rbtNodePtr *zParent) { // Fix up tree to restore the red-black properties after delete ; Parent of z needed in case z = NULL (leaf)
	rbtNodePtr w;

	while ((*z != *T) && ((*z == NULL) || (*z)->colour == BLACK)) { // NULL => leaf => colour = black
		if (*z == (*zParent)->left) {
			w = (*zParent)->right;
			if (w == NULL) {  // If z's sibling is leaf, nothing to fix up

				break;
			}

			if (w->colour == RED) { // Case 1: z's sibling is red
				w->colour = BLACK;
				(*zParent)->colour = RED;
				
				rbtLeftRotate(T, *zParent);

				w = (*zParent)->right;
			}

			if (((w->left == NULL) && (w->right == NULL)) || ((w->left->colour == BLACK) && (w->right->colour == BLACK))) { // Case 2: z's sibling is black and both of sibling's children are black ; NULL => leaf => colour = black
				w->colour = RED;
				*z = *zParent;
				*zParent = (*zParent)->parent; // Update z's parent
			}

			else {
				if ((w->right == NULL) || (w->right->colour == BLACK)) { // Case 3: z's sibling is black, sibling's left child is red and sibling's right child is black ; NULL => leaf => colour = black
					w->left->colour = BLACK;
					w->colour = RED;

					rbtRightRotate(T, w);

					w = (*zParent)->right;
				}

				// Case 4: z's sibling is black and sibling's right child is red
				w->colour = (*zParent)->colour;
				(*zParent)->colour = BLACK;
				w->right->colour = BLACK;

				rbtLeftRotate(T, *zParent);
				*z = *T;
			}
		}

		else {
			w = (*zParent)->left;
			if (w == NULL) { // If z's sibling is leaf, nothing to fix up

				break;
			}

			if (w->colour == RED) { // Case 1: z's sibling is red
				w->colour = BLACK;
				(*zParent)->colour = RED;
				
				rbtRightRotate(T, *zParent);

				w = (*zParent)->left;
			}

			if (((w->right == NULL) && (w->left == NULL)) || ((w->right->colour == BLACK) && (w->left->colour == BLACK))) { // Case 2: z's sibling is black and both of sibling's children are black ; NULL => leaf => colour = black
				w->colour = RED;
				*z = *zParent;
				*zParent = (*zParent)->parent;  // Update z's parent
			}

			else {
				if ((w->left == NULL) || (w->left->colour == BLACK)) { // Case 3: z's sibling is black, sibling's left child is black and sibling's right child is red ; NULL => leaf => colour = black
					w->right->colour = BLACK;
					w->colour = RED;

					rbtLeftRotate(T, w);

					w = (*zParent)->left;
				}

				// Case 4: z's sibling is black and sibling's left child is red
				w->colour = (*zParent)->colour;
				(*zParent)->colour = BLACK;
				w->left->colour = BLACK;

				rbtRightRotate(T, *zParent);
				*z = *T;
			}
		}
	}

	if (*z != NULL) {

		(*z)->colour = BLACK;		
	}
}

void rbtLeftRotate(rbtNodePtr *T, rbtNodePtr x) { // Rotate record to the left
	rbtNodePtr y = x->right; // Set y;

	x->right = y->left; // Turn y's left subtree into x's right subtree

	if (y->left != NULL) {

		y->left->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == NULL) {

		*T = y;
	}

	else if (x == x->parent->left) {

		x->parent->left = y;
	}

	else {

		x->parent->right = y;
	}

	y->left = x; // Put x on y's left
	x->parent = y;
}

void rbtRightRotate(rbtNodePtr *T, rbtNodePtr x) { // Rotate record to the right
	rbtNodePtr y = x->left; // Set y;

	x->left = y->right; // Turn y's right subtree into x's left subtree

	if (y->right != NULL) {

		y->right->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == NULL) {

		*T = y;
	}

	else if (x == x->parent->left) {

		x->parent->left = y;
	}

	else {

		x->parent->right = y;
	}

	y->right = x; // Put x on y's right
	x->parent = y;
}

void rbtTransplant(rbtNodePtr *T, rbtNodePtr u, rbtNodePtr v) { // Transplant RBT node with another rbt node
	if (u->parent == NULL) { // If u is the root
		
		*T = v; // v replace u as root of tree T
	}

	else if (u == u->parent->left) { // If u is left subtree of its parent

		u->parent->left = v; // v replace u as left subtree of u's parent
	}

	else { // If u is right subtree of its parent

		u->parent->right = v; // v replace u as right subtree of u's parent
	}

	if (v != NULL) { // If v is not NULL

		v->parent = u->parent; // v same parent as u		
	}
}

rbtNodePtr rbtMinimum(rbtNodePtr x) { // Find node with minimum key
	while (x->left != NULL) {

		x = x->left;
	}

	return x;
}

recordPtr rbtNodeGetRecord(rbtNodePtr x) { // Get record from RBT Node
	if (x != NULL) {

		return x->record;
	}

	else {

		return NULL;
	}
}

void rbtInorderBFInsert(rbtNodePtr x, bfPtr b) { // Insert RBT's records in BF in Inorder traversal
	if (x != NULL) {
		rbtInorderBFInsert(x->left, b); // Recursion on left child
		bfInsert(b, recordGetId(x->record)); // Insert key's record in BF
		rbtInorderBFInsert(x->right, b); // Recursion on right child
	}

}

void rbtInorderFPrint(rbtNodePtr x, FILE *foutput) { // Print RBT's records in file in Inorder traversal
	if (x != NULL) {
		rbtInorderFPrint(x->left, foutput); // Recursion on left child
		recordFPrint(x->record, foutput); // Insert key's record in BF
		rbtInorderFPrint(x->right, foutput); // Recursion on left child
	}

}

void rbtNodeFree(rbtNodePtr x) { // Free RBT node
	if (x->record != NULL) {

		recordFree(x->record);
	}

	free(x);
}

void rbtFree(rbtNodePtr T) { // Free RBT
	if (T != NULL) {
		rbtFree(T->left); // Free left subtree
		rbtFree(T->right); // Free right subtree

		rbtNodeFree(T); // When done, free node
		T = NULL;
	}
}