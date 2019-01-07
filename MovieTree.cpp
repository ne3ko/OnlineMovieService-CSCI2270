// Nikolai Alexander
// Assignment 4
// Christopher Godley

/* This file builds the functions in the MovieTree Class (found in the MovieTree.hpp file) */

#include <iostream>
#include <fstream>
#include <string>
#include "MovieTree.hpp"

using namespace std;

/* ----PUBLIC FUNCTIONS---- */

// Constructor
MovieTree::MovieTree(){
	root = new MovieNodeBST;

	root->letter = '\0';
	root->parent = NULL;
	root->leftChild = NULL;
	root->rightChild = NULL;

	root->head = new MovieNodeLL();
	root->head->next = NULL;
}
// Destructor
MovieTree::~MovieTree(){
	DeleteAll(root);
	delete root;

	cout << "Goodbye!" << endl;
}

// printMovieInventory helper function for privacy
void MovieTree::printMovieInventory(){
	printMovieInventory(root);
}

//countMovieNodes helper function for privacy
int MovieTree::countMovieNodes(){
	int count = 0;

	countMovieNodes(root, &count);

	return count;
}

// Deletes the chosen movie from the LL. If there aren't anymore movies in the LL, it proceeds to
// delete the BST node that the deleted movie was originally attached to.
//		- string title - the title of the movie you would like to delete
void MovieTree::deleteMovieNode(string title){
	MovieNodeBST* tmpBST = searchBST(root, title);

	// Using a conditional to determine what to set tmpLL to
	// 		If tmpBST is not equal to NULL, search for the title in the linked list
	//		If tmpBST is equal to NULL, set tmpLL to NULL
	MovieNodeLL* tmpLL = (tmpBST != NULL) ? searchLL(tmpBST->head, title) : NULL;

	// If we can't find the title, give a message
	if(tmpLL == NULL){
		cout << "Movie not found." << endl;
		return;
	}

	tmpLL = tmpBST->head;
	// Delete node from Linked List
	if(tmpBST->head->title != title){ // Middle/tail of the linked list

		while(tmpLL->next->title != title){
			tmpLL = tmpLL->next;
		}

		// Change pointers to bypass the node we want to delete
		MovieNodeLL* deleteLL = tmpLL->next;
		tmpLL->next = deleteLL->next;

		// Delete the LL node
		deleteLL->next = NULL;
		delete deleteLL;
	}
	else{ // Head of the linked list
		// Change the pointers to make new head
		tmpBST->head = tmpLL->next;
		tmpLL->next = NULL;

		// Delete the original head
		delete tmpLL;
	}

	// If there is still data in the LL, we do not need to delete the node
	if(tmpBST->head != NULL){
		return;
	}

	// If the linked list is empty, delete the node from the BST
	MovieNodeBST* tmpParent = tmpBST->parent;
	// Delete a node with no children
	if(tmpBST->rightChild == tmpBST->leftChild){
		// Delete root
		if(root == tmpBST){
			delete root;
			root = NULL;
		}
		// Delete middle or tail
		else{
			// The node is the left child of its parent
			if(tmpBST->letter < tmpParent->letter){
				tmpParent->leftChild = NULL;
			}
			// The node is the right child of its parent
			else{
				tmpParent->rightChild = NULL;
			}

			// Delete the node to free memory
			delete tmpBST;
		}
	}
	// Delete a node with 1 child
	else if((tmpBST->rightChild != NULL) != (tmpBST->leftChild != NULL)){
		// Delete the root
		if(root == tmpBST){
			MovieNodeBST* tmpBST;

			// The root has a left child
			if(root->leftChild != NULL){
				root = root->leftChild;
			}
			// The root has a right child
			else{
				root = root->rightChild;
			}

			delete tmpBST;
		}
		// Delete the middle or the tail
		else{
			// The node has a left child
			if(tmpBST->leftChild != NULL){
				// The node is a left child of it's parent
				if(title[0] < tmpParent->letter){
					tmpParent->leftChild = tmpBST->leftChild;
				}
				// The node is the right child of it's parent
				else{
					tmpParent->rightChild = tmpBST->leftChild;
				}

				// Set the left child's parent to your node's parent
				tmpBST->leftChild->parent = tmpParent;
			}
			// The node has a right child
			else{
				// Same as above
				if(title[0] < tmpParent->letter){
					tmpParent->leftChild = tmpBST->rightChild;
				}
				else{
					tmpParent->rightChild = tmpBST->rightChild;
				}

				tmpBST->rightChild->parent = tmpParent;
			}

			delete tmpBST;
		}
	}
	// Delete a node with 2 children
	else{
		// Find the inner most leaf for the right child of your node
		MovieNodeBST* innerBST = treeMinimum(tmpBST->rightChild);

		// Delete the root
		if (root == tmpBST){
			// Set parent pointer to NULL
			innerBST->parent = NULL;

			// Set the left child of innermost node to the root's left child
			innerBST->leftChild = root->leftChild;
			root->leftChild->parent = innerBST;

			// Set the right child of the innermost node to the root's right child, if the
			// innermost node is not the right child of the root
			if(innerBST->letter != root->rightChild->letter){
				innerBST->rightChild = root->rightChild;
				root->rightChild->parent = innerBST;
			}

			// Set the root to be the innermost node
			root = innerBST;

			// Free the original root from memory
			delete tmpBST;
		}
		else{
			// Set the child pointer of your node's parent to the innermost node
			//If your node is the left child of its parent
			if(title[0] < tmpParent->letter){
				tmpParent->leftChild = innerBST;
			}
			//If your node is the right child of its parent
			else{
				tmpParent->rightChild = innerBST;
			}

			// Set the innermost node's parent to your node's parent
			innerBST->parent = tmpParent;

			// Set the left child of the innermost node to your node's left child
			innerBST->leftChild = tmpBST->leftChild;
			tmpBST->leftChild->parent = innerBST;

			// Set the right child of the innermost node to your node's right child, if the
			// innermost node is not the right child of your node
			if(innerBST->letter != tmpBST->rightChild->letter){
				innerBST->rightChild = tmpBST->rightChild;
				tmpBST->rightChild->parent = innerBST;
			}

			delete tmpBST;
		}
	}
}

// Iterates through BST - If there is no match for the node, it creates a new BST node and
// initialized a linked list where the head is the MovieNodeLL data from the new BST node
//		- int ranking - the IMBD rank of the movie
//		- string title - the title of the movie
//		- int releaseYear - the year the movie was released
//		- int quantity - how many movies are in stock
void MovieTree::addMovieNode(int ranking, string title, int releaseYear, int quantity){
	char firstLetter = title[0];

	// Build new BST node
	MovieNodeBST* bstNode = new MovieNodeBST;
	bstNode->letter = firstLetter;
	bstNode->parent = NULL;
	bstNode->leftChild = NULL;
	bstNode->rightChild = NULL;

	// Build new LL node attached to bstNode
	bstNode->head = new MovieNodeLL;
	bstNode->head->ranking = ranking;
	bstNode->head->title = title;
	bstNode->head->year = releaseYear;
	bstNode->head->quantity = quantity;
	bstNode->head->next = NULL;

	// Insert new node into BST
	MovieNodeBST* tmpBST = root;

	// If the BST is empty (root = NULL), we can just set too the be the bstNode 
	if(root->letter == '\0'){
		root = bstNode;
	}
	// If the BST is not empty, we need to traverse through the tree and add the node to the
	// end of the tree
	else{
		while(1){
			// If the first letter of the title comes before the letter of the node in the tree
			// go left
			if(firstLetter < tmpBST->letter){
				// If the next available left child is empty, place our bstNode there
				if(tmpBST->leftChild == NULL){
					tmpBST->leftChild = bstNode;
					bstNode->parent = tmpBST;
					return; // Break infinite loop when bstNode is added
				}
				else{
					tmpBST = tmpBST->leftChild;
				}
			}
			// If the first letter of the title comes after the letter of the node in the tree
			// go right
			else if(firstLetter > tmpBST->letter){
				// If the next available right child is empty, place our bstNode there
				if(tmpBST->rightChild == NULL){
					tmpBST->rightChild = bstNode;
					bstNode->parent = tmpBST;
					return;

				}
				else{
					tmpBST = tmpBST->rightChild;
				}
			}
			// If the first letters are identical, stop on BST node and traverse through LL to 
			// tail
			else{
	
				// Add to the tail of the linked list attached to BST node
				MovieNodeLL* tmpLL = tmpBST->head;
				while(tmpLL->next != NULL){
					tmpLL = tmpLL->next;
				}
				tmpLL->next = bstNode->head;
	
				delete bstNode; // Delete bstNode to free memory
				return;
			}
		}
	}

}

// Finds all information on the movie chosen in the main menu
//		- string title - The title of the movie we are searching for
void MovieTree::findMovie(string title){
	MovieNodeBST* foundLetter = searchBST(root, title);

	// Using a conditional to determine what to set foundMovie to
	// 		If foundLetter is not equal to NULL, search for the title in the linked list
	//		If foundLetter is equal to NULL, set tmpLL to NULL
	MovieNodeLL* foundMovie = (foundLetter != NULL) ? searchLL(foundLetter->head, title) : NULL;

	// If we can't find the title, give a message
	if(foundMovie == NULL){
		cout << "Movie not found." << endl;
		return;
	}

	// Display movie information
	cout << "Movie Info: " << endl;
	cout << "===========" << endl;
	cout << "Ranking: " << foundMovie->ranking << endl;
	cout << "Title: " << foundMovie->title << endl;
	cout << "Year: " << foundMovie->year << endl;
	cout << "Quantity: " << foundMovie->quantity << endl;
}

// Finds all information on the chosen movie. It then reduces the quantity by 1. If the movie
// quantity reaches 0, then the node is deleted from the list
//		- string title - the title of the movie we would like to rent
void MovieTree::rentMovie(string title){
	MovieNodeBST* foundLetter = searchBST(root, title);

	// Using a conditional to determine what to set tmpLL to
	// 		If tmpBST is not equal to NULL, search for the title in the linked list
	//		If tmpBST is equal to NULL, set tmpLL to NULL
	MovieNodeLL* foundMovie = (foundLetter != NULL) ? searchLL(foundLetter->head, title) : NULL;

	// If we can't find the title, give a message
	if(foundMovie == NULL){
		cout << "Movie not found." << endl;
		return;
	}

	// Reduce movie quanitity by 1
	foundMovie->quantity = foundMovie->quantity - 1;

	// Display updated information for rental
	cout << "Movie Info: " << endl;
	cout << "===========" << endl;
	cout << "Ranking: " << foundMovie->ranking << endl;
	cout << "Title: " << foundMovie->title << endl;
	cout << "Year: " << foundMovie->year << endl;
	cout << "Quantity: " << foundMovie->quantity << endl;

	// If the quantity reaches 0, delete the movie node
	if(foundMovie->quantity == 0){
		deleteMovieNode(foundMovie->title);
	}
}



/* ----PRIVATE FUNCTIONS---- */

// Deletes the entire tree using a postorder tree traversal
//		- MovieNodeBST* node - The node we are starting at (in our case, the root)
void MovieTree::DeleteAll(MovieNodeBST * node){

	// If the root is equal to NULL, don't do anything
	if(node == NULL){
		return;
	}

	// Recursively delete all left children of BST
	if(node->leftChild != NULL){
		DeleteAll(node->leftChild);
	}

	// Recursively delete all right children of BST
	if(node->rightChild != NULL){
		DeleteAll(node->rightChild);
	}

	// Delete the singly linked list from the tail
	while(node->head->next != NULL){
		MovieNodeLL* tmpLL = node->head;

		// Traverse through the linked list to the tail
		while(tmpLL->next->next != NULL){
			tmpLL = tmpLL->next;
		}

		MovieNodeLL* deleteLL = tmpLL->next;

		cout << "Deleting: " << deleteLL->title << endl;

		// Delete the tail from memory
		tmpLL->next = NULL;
		delete deleteLL;
	}

	cout << "Deleting: " << node->head->title << endl;

	// Delete the head from memory
	node->head = NULL;
	delete node->head;

	if(root == node){
		root = NULL;
	}

	// Delete the node
	delete node;
}

// Iterates through the BST in alphabetical order. Once it gets to its desired node, it prints
// the linked list in the order that each node has been inserted
//		- MovieNodeBST* node - the BST node to which we are beginning the print process (in our
//		  					   case, this is the root)
void MovieTree::printMovieInventory(MovieNodeBST* node){
	if(node->letter == '\0'){
		return;
	}

	//Move to the left node in BST
	if(node->leftChild != NULL){
		printMovieInventory(node->leftChild);
	}

	// Print linked list attached to BST node
	MovieNodeLL* tmpNode = node->head;
	while(tmpNode != NULL){
		cout << "Movie: " << tmpNode->title <<  " " << tmpNode->quantity << endl;
		tmpNode = tmpNode->next;
	}

	// Move to the right node in BST
	if(node->rightChild != NULL){
		printMovieInventory(node->rightChild);
	}
}

// Iterates through the tree, stopping at each node and counting the number of movies in each
// Linked List.
//		- MovieNodeBST* node - the BST node the countMovieNodes function begins itterating from
//							   in our case, it is the root.
//		- int* c - The number of movies in the tree.
void MovieTree::countMovieNodes(MovieNodeBST *node, int *c){
	if(node->letter == '\0'){
		return;
	}

	// Move left through BST
	if(node->leftChild != NULL){
		countMovieNodes(node->leftChild, c);
	}

	// Move right through BST
	if(node->rightChild != NULL){
		countMovieNodes(node->rightChild, c);
	}

	// Traverse through LL and increase count by 1 for every movie in the LL
	MovieNodeLL*tmpNode = node->head;
	while(tmpNode != NULL){
		*c = *c + 1;
		tmpNode = tmpNode->next;
	}
}

// Searches through the BST and returns the node that shares the same first letter as the title
//		- MovieNodeBST* node - the node that the search begins on
//		- string title - the title of the movie we are searching for
MovieNodeBST* MovieTree::searchBST(MovieNodeBST *node, string title){
	char firstLetter = title[0];

	if(node == NULL){
		return NULL;
	}

	if(firstLetter == node->letter){
		return node;
	}
	else if(firstLetter < node->letter){
		return searchBST(node->leftChild, title);
	}
	else{
		return searchBST(node->rightChild, title);
	}
}

// Searches through the LL attached to the BST and returns the node that shares the same title
// as the head.
//		- MovieNodeLL* head - the LL node where the search begins
//		- string title - the title of the movie we are searching for
MovieNodeLL* MovieTree::searchLL(MovieNodeLL* head, string title){
	if(head == NULL){
		return NULL;
	}

	if(head->title == title){
		return head;
	}
	else{
		return searchLL(head->next, title);
	}
}

// Finds the left most leef of the determined BST node
//		- MovieNodeBST* node - the node that the traverse begins at
MovieNodeBST* MovieTree::treeMinimum(MovieNodeBST *node){
	while(node->leftChild != NULL){
		node = node->leftChild;
	}

	return node;
}