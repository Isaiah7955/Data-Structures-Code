//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: main.cpp
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"
#include "HashChaining.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// This is a helper function to call to display the menu in the main function

void displayMenu()
{
    cout << endl;
    cout << "=======Main Menu=======" << endl;
    cout << "1. Populate hash tables" << endl;
    cout << "2. Search for a course" << endl;
    cout << "3. Search for a professor" << endl;
    cout << "4. Display all courses" << endl;
    cout << "5. Exit" << endl;
    cout << endl;

    cout << "Enter an option: " << endl;
}

int main(int argc, char *argv[])
{
    string programName = argv[0]; //takes the 3 arguments and saves them into variables to be used
    string csvInputFile = argv[1];
    string hashTablesize = argv[2];
    int hashTableSizeInt = stoi(hashTablesize);

    string userInput;
    int intUserInput;
    HashOpenAddressing c = HashOpenAddressing(hashTableSizeInt); // this creates an instance of the HashOpenAddressing and the HashChaining functions with the hash table being the size given by the user
    HashChaining d = HashChaining(hashTableSizeInt);             //these also call the paramterized constructors to create empty spaces that are going to be filled with data

    int counter = 0; //counter to make sure that user selects the populating of hash tables first

    while (intUserInput != 5) // keeps menu coming out unless user selects quit
    {
        displayMenu();
        cin >> userInput;
        intUserInput = stoi(userInput);
        if (counter == 0 && intUserInput != 1) //tests if user tries to use any other functions before populating the hash tables and makes sure this is done first
        {
            while (intUserInput != 1)
            {
                cout << "Must populate the hash tables first" << endl;
                cout << endl;
                cout << "Please enter 1 to populate the hash tables" << endl;
                cin >> userInput;
                intUserInput = stoi(userInput);
            }
        }

        switch (intUserInput)
        {
        case 1:
        {
            c.bulkInsert(csvInputFile); //call the bulk insert functions with both the OpenAddressing and Chaining methods
            d.bulkInsert(csvInputFile);
            counter++; //increment counter so as to not enter the above if check to show that tables have been populated
            break;
        }
        case 2:
        {
            string courseYearString;
            string courseNumberString;

            int courseYear;
            int courseNumber;

            string profId;

            cout << "Ente the course year(e.g. 2021): " << endl;
            cin >> courseYearString;
            courseYear = stoi(courseYearString); //save the course year as an int

            cout << "Enter a course number(e.g. 2270): " << endl;
            cin >> courseNumberString;
            courseNumber = stoi(courseNumberString); //save the course name as an int

            cout << "Enter a Professsor's ID(e.g. llytellf): " << endl;
            cin >> profId; //saves professor ID as a string

            cout << endl;
            cout << "[OPEN ADDRESSING] Search for a course" << endl;
            cout << "-------------------------------------" << endl;
            c.search(courseYear, courseNumber, profId); //calls the search function with both the OpenAddressing and Chaining Methods
            cout << endl;
            cout << "[CHAINING] Search for a course" << endl;
            cout << "-------------------------------------" << endl;
            d.search(courseYear, courseNumber, profId);

            counter++;
            break;
        }
        case 3:
        {
            string professorID;
            Professor *check = new Professor;

            cout << "Enter a Professor's ID(e.g. nscollan0): " << endl;
            cin >> professorID; //takes in the professor's ID that is going to be used to search within the BST

            check = c.profDb.searchProfessor(professorID); //creates a new Professor pointer that is saved to variable check to see if the professor is in the BST
            if (check == NULL)
            {
                cout << endl;
                cout << "This professor ID was not found." << endl; //this means the professor was not found in the BST
                break;
            }

            cout << endl;
            cout << "[OPEN ADDRESSING] Search for a professor" << endl;
            cout << "----------------------------------------" << endl;
            c.profDb.publicSearchProfessor(professorID); //calls the function for the OpenAddressing method
            cout << "[CHAINING] Search for a professor" << endl;
            cout << "----------------------------------------" << endl;
            d.profDb.publicSearchProfessor(professorID); //calls the function for the Chaining method

            counter++;
            break;
        }
        case 4:
        {
            string oOrA;
            cout << "Which hash table would you like to display the courses for (O=Open Addressing, C=Chaining)?" << endl; //takes in which table wants to use
            cin >> oOrA;

            if (oOrA == "O" || oOrA == "o")
            {
                cout << "[OPEN ADDRESSING] displayAllCourses()" << endl;
                cout << "--------------------------------" << endl;
                c.displayAllCourses(); //calls function of open addressing if this is what user chooses
                cout << endl;
            }
            if (oOrA == "C" || oOrA == "c")
            {
                cout << "[CHAINING] displayAllCourses()" << endl;
                cout << "--------------------------------" << endl;
                d.displayAllCourses(); //calls function of chaining if this is what user chooses
            }

            counter++;
            break;
        }
        case 5:
        {
            cout << "Goodbye! Thank you so much for a great year!" << endl; //ends program because userInput was 5
            break;
        }
        default: // invalid input
        {
            cout << "Invalid Input" << endl;
            break;
        }
        }
    }
    return 0;
}
