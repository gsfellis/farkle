#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Menu.h"

using namespace std;

// Displays the Main Menu
void Menu::ShowMenu()
{
	system("cls");
	cout << "=================" << endl;
	cout << "Welcome to Farkle" << endl;
	cout << "=================" << endl;
	cout << "Maximize the console window for best results." << endl << endl;

	cout << "1) New Game" << endl;
	cout << "2) Show Rules" << endl;
	cout << "3) Quit" << endl << endl;

	cout << "Please enter a selection: ";
}

// Reads the Rules from a .txt file
void Menu::ShowRules()
{
	string line;	// holds each line in the file
	string filename = "Rules.txt";	// the file to be opened
	ifstream myfile(filename);	// opens a stream of the file
	if (myfile.is_open())
	{
		system("cls"); // clear screen

		// Get and output all lines in the file
		while (getline(myfile, line))
		{
			cout << line << endl;
		}

		myfile.close();	// close the file
	}
	else
	{
		cout << "Unable to open '" << filename << "'" << endl;
	}

	cout << endl;
	system("pause");
}