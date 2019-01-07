----------------------
Assignment 4
----------------------
Author: Nikolai Alexander
Published: July 8, 2018
Course: CSCI2270-300


----------------------
DESCRIPTION
----------------------

This program simulates an inventory database for an online movie service. The 
movies are stored in a binary search tree by the first letter of their title. Each
BST node then points to a Linked List where the information is stored for each 
movie sharing the same first letter. A main menu is presented when opening the
program - this main menu allows the user to find the information on a movie (IMBD
rank, title, release year, and quantity), rent a movie, print the entire inventory,
delete a movie, and count the number of movies in the database.


----------------------
FILE DESCRIPTION
----------------------

MovieTree.hpp - The header file containing the MovieNodeBST and MovieNodeLL structures, and the
			  MovieTree class. It also initializes all of the function prototypes in 
			  the MovieTree class


MovieTree.cpp - This is the file building all of the functions in the MovieTree class.

	Functions:
	* MovieTree() - The constructor of the MovieTree class.

	* ~MovieTree() - The destructor of the MovieTree class.

    * void printMovieInventory() - Traverses through the BST in alphabetical order, starting at 
    							   the root. Once it gets to its desired node, it prints the 
    							   linked list in the order that each node has been inserted.
    
    * int countMovieNodes() - Iterates through the BST, stopping at each node and counting the 
    						  number of movies in each Linked List.
    
    * void deleteMovieNode(string) - Deletes the chosen movie from the LL. If there aren't 
    								 anymore movies in the LL, it proceeds to delete the BST 
    								 node that the deleted movie was originally attached to.
    
    * void addMovieNode(int, string, int, int) - Traverses through BST, and looks for a match in 
    											 the first letter from the title. If there is no
    											 match, it creates a new BST node and 
    											 initializes a linked list where the head is the
    											 MovieNodeLL data from the new BST node.
    
    * void findMovie(string) - Finds all information on the movie chosen in the main menu.

    * void rentMovie(string) - Finds all information on the chosen movie. It then reduces the 
    						   quantity by 1. If the movie quantity reaches 0, then the node is deleted from the list.


main.cpp - The main file of the program. Constructs the data structure, and displays a
		  main menu showing options to select any of the functions written in MovieTree.cpp.

	Functions:
	* int main(int, char*) - The main function of the file. Constructs the data structure, and uses a main menu to call functions written in MovieTree.cpp

	* void populateMovieData(MovieTree*, char*) - Reads the text file determined in the line 
												  argument and populates the database with all the movie information from the text file.




----------------------
BUILD INSTRUCTIONS
----------------------

To build and run the file, type the following commands in your terminal:

	g++ MovieTree.cpp Assignment4.cpp -o Assignment4

	./Assignment4 filename

	* Where filename is the name of the txt file containing the movie data you would like to 
	  populate the database with.