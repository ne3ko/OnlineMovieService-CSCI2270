// Nikolai Alexander
// Assignment 4
// Christopher Godley

/* This program simulates an inventory database for an online movie service. The 
movies are stored in a binary search tree by the first letter of their title. Each
BST node then points to a Linked List where the information is stored for each 
movie sharing the same first letter. A main menu is presented when opening the
program - this main menu allows the user to find the information on a movie (IMBD
rank, title, release year, and quantity), rent a movie, print the entire inventory,
delete a movie, and count the number of movies in the database.*/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "MovieTree.hpp"

using namespace std;

void populateMovieData(MovieTree* data, char* txtFile);

int main(int argc, char* argv[]){
	MovieTree data;
	int option = 0;

	populateMovieData(&data, argv[1]);

	while(1){

		//Main Menu
		cout << "======Main Menu======" << endl;
		cout << "1. Find a movie" << endl;
		cout << "2. Rent a movie" << endl;
		cout << "3. Print the inventory" << endl;
		cout << "4. Delete a movie" << endl;
		cout << "5. Count the movies" << endl;
		cout << "6. Quit" << endl;

		cin >> option;

		// Find Movie
		if(option == 1){
			string foundMovie;

			cout << "Enter title:" << endl;
			cin >> ws;
			getline(cin, foundMovie);

			data.findMovie(foundMovie);
		}
		// Rent Movie
		else if(option == 2){
			string rental;

			cout << "Enter title:" << endl;
			cin >> ws;
			getline(cin, rental);

			data.rentMovie(rental);
		}
		// Print Inventory
		else if(option == 3){
			data.printMovieInventory();
		}
		// Delete movie
		else if(option == 4){
			string delMovie;

			cout << "Enter title:" << endl;
			cin >> ws;
			getline(cin, delMovie);

			data.deleteMovieNode(delMovie);
		}
		// Count movies
		else if(option == 5){
			cout << "Tree contains: " << data.countMovieNodes() << " movies." << endl;
		}
		// Quit
		else if(option == 6){
			break;
		}
		// Invalid Entry
		else{
			cout << "Invalid entry. Please try again." << endl;

			// Prevent from infinite looping if a non-int input is entered
			// Source: https://stackoverflow.com/questions/10828937/how-to-make-cin-take-only-numbers/10829026#10829026
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

	}

return 0;
}

// Reads the text file determined in the line argument, and populates the database
// with all the movie information from the text file.
//		- MovieTree* data - the object where all the data is stored
//		- char* txtFile - The text file all the movie information is being read 
//						  from.
void populateMovieData(MovieTree* data, char* txtFile){
	string movieTitle;
	string str_movieRanking;
	string str_movieYear;
	string str_movieQty;

	int movieRanking;
	int movieYear;
	int movieQty;

	ifstream movieData(txtFile);
	if(!movieData){
		cout << "Unable to locate " << txtFile << endl;
		return;
	}

	while(!movieData.eof()){
	
		// Pull all information from the txtFile determined in the arugment line
		// Movie Ranking
		getline(movieData, str_movieRanking, ',');
		movieRanking = stoi(str_movieRanking); // Convert to int
	
		// Movie Title	
		getline(movieData, movieTitle, ',');

		// Movie Year
		getline(movieData, str_movieYear, ',');
		movieYear = stoi(str_movieYear);

		// Movie Quantity
		getline(movieData, str_movieQty, '\n');
		movieQty = stoi(str_movieQty);

		data->addMovieNode(movieRanking, movieTitle, movieYear, movieQty);
	}

	movieData.close();
}