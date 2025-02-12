//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashChaining.cpp
//-----------------------------------------------------------------------------

#include "HashChaining.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

/**
 This function is creating a Course according to the inputted data of the year, department, courseNum, courseName and prof given by the lines in the file
 **/

Course *HashChaining::createNode(int year, string department, int courseNum, string courseName, Professor *prof)
{
    Course *nw = new Course;
    nw->year = year;
    nw->department = department;
    nw->courseNum = courseNum;
    nw->courseName = courseName;
    nw->prof = prof;
    nw->next = NULL;
    return nw;
}

HashChaining::HashChaining(int size) //parameterized constructor
{
    this->hashTableSize = size;              //initializes the size of the hash table accoridng to the user input
    hashTable = new Course *[hashTableSize]; //creates the hashTable full of Course pointer instances of the hashTableSize
    for (int i = 0; i < size; i++)
    {
        hashTable[i] = nullptr; //makes all of the Course instances nullptr before they have data inputted into them
    }
}

void HashChaining::deleteLL(Course *course) //this is a helper function to delete instance of a linked list in the dataset
{
    Course *pointer = course;
    Course *courseNext; //saves pointer's for the current and next elements of the linked list

    while (pointer != NULL)
    {
        courseNext = pointer->next; //first sets the next pointer to the correct element
        delete pointer->prof; //deletes the prof instance
        delete pointer; //deletes the element of the linkedList
        pointer = courseNext; //now moves the pointer forwward
    }
}

HashChaining::~HashChaining() //deconstructor
{
    for (int i = 0; i < hashTableSize; i++) //goes through the entire table
    {
        if (hashTable[i] != NULL) //goes into this statement if the current Course isn't already NULL
        {
            deleteLL(hashTable[i]); //calls the helper function
        }
    }
    delete[] hashTable; //then finally deletes the entire hashTable as to free up data
}

/**
 This is the hash function that is being used...it returns the remainder of the courseNumber modulo hashTableSize
 **/

int HashChaining::hash(int courseNumber)
{
    int hash = courseNumber % hashTableSize;
    return hash;
}

void HashChaining::bulkInsert(string filename)
{
    ifstream myFile;
    myFile.open(filename); //opens the file that is passed into function
    string line;

    //counter for the collisions and the searches
    int collisions = 0;
    int searches = 0;

    //variables to be used to take data from the line in the file
    string year2;
    int year;
    string department;
    string courseNum2;
    int courseNum;
    string courseName;
    string profId;
    string fname;
    string lname;

    if (myFile.is_open())
    {
        getline(myFile, line);
        while (getline(myFile, line)) //uses getline to go through every line in the file
        {

            istringstream s(line); //uses stringstream to seperate the content in the line seperated by commas
            getline(s, year2, ',');
            getline(s, department, ',');
            getline(s, courseNum2, ',');
            getline(s, courseName, ',');
            getline(s, profId, ',');
            getline(s, fname, ',');
            getline(s, lname, ',');

            year = stoi(year2);
            courseNum = stoi(courseNum2);
            Professor *prof = new Professor;
            Professor *search = new Professor;

            string name = fname + " " + lname; //saves name by combining the first and last
            prof->profId = profId;
            prof->profName = fname + " " + lname;
            search = profDb.searchProfessor(profId); //uses the searchProfessor function and saves in a Professor pointer to see whether the professor has already been added in the BST

            int index = hash(courseNum); //allocate hash function to find our index of where we are putting the element into the hashTable
            Course *pointer = new Course;
            Course *pointer2 = new Course;
            pointer = createNode(year, department, courseNum, courseName, prof); //creates the Course instance
            pointer2 = hashTable[index];                                         //saves the hashTable current index into a Course
            if (hashTable[index] == NULL) //if already NULL then no collision OpenAddressing is needed
            {
                hashTable[index] = pointer;
            }
            else
            {
                bool found = false; //create boolean that is used to track whether the course has been added or not
                collisions++; //increment the collisions
                while (pointer2 != NULL) //goes through the linkedList till the end element
                {
                    //if the course is already in the linkedList, then change the boolean and break from while loop
                    if (pointer2->courseNum == courseNum && pointer2->year == year && pointer2->department == department && pointer2->courseName == courseName && pointer2->prof == prof) 
                    {
                        found = true;
                        break;
                    }
                    searches++; //continue keeping track of how many searches it took
                    if (pointer2->next != NULL) //this is to make sure that the next is not NULL so we avoid the segmentation fault
                    {
                        pointer2 = pointer2->next; //keep iterating through linked list
                    }
                    else
                    {
                        break; //this shows that the entire linkedList has been searched through
                    }
                }
                if (found == false) //if it is not in the linkedList already then add it in
                {
                    pointer2->next = pointer; //make the final element next the current Course pointer
                    pointer->previous = pointer2;
                }
            }
            if (search == NULL) //uses search from earlier to see if going to add professor or not into BST
            {
                profDb.addProfessor(profId, name); //adds professor to the BST
                prof->coursesTaught.push_back(pointer); //push the course to the correct professors vector as to be used later
            }
            if (search != NULL)
            {
                search->coursesTaught.push_back(pointer); //push the course to the correct professors vector as to be used later
            }
        }
    }

    cout << endl;
    cout << "[CHAINING] Hash table populated" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Collisions using chaining: " << collisions << endl;
    cout << "Search operations using chaining: " << searches << endl; //output message for the user
}

void HashChaining::search(int courseYear, int courseNumber, string profId)
{
    int counter = 0; //creates counter to see how many times it is going to need to search
    int index = hash(courseNumber); //call the hash function to start our search
    Course *pointer = new Course;
    bool searchCheck = false;

    for (int i = index; i < hashTableSize; i++) //looks through the entire hashTable to look for this element
    {
        pointer = hashTable[i]; //set our pointer to the current location of the hashTable index
        if (hashTable[i] != NULL) //means there may or may not be a linked list at this location
        {
            while (pointer != nullptr) //parses through to find until it reaches final element
            {
                if ((pointer->year == courseYear) && (pointer->courseNum == courseNumber) && (pointer->prof->profId == profId)) //if all elements correct, then output
                {
                    cout << "Search operations using chaining: " << counter << endl;
                    cout << pointer->year << " " << pointer->courseName << " " << pointer->courseNum << " " << pointer->prof->profName << endl;
                    cout << endl;
                    searchCheck = true;
                    break;
                }
                pointer = pointer->next; //keep incrementing through the hashTable
                counter++;               //count how many times needed to search
            }
        }
    }
    if(searchCheck == false)
    {
        cout << "Course not found. Please try inputting correct information." << endl; //has gone through entire hashTable and linkedList accordingly
    }
}

void HashChaining::displayAllCourses()
{
    Course *pointer = new Course;

    for (int i = 0; i < hashTableSize; i++) //goes through entire hashTable to output all courses where index is NOT nullptr
    {
        pointer = hashTable[i]; //creates pointer to first element in hash table
        if (pointer != NULL)
        {
            cout << pointer->year << " " << pointer->courseName << " " << pointer->courseNum << " " << pointer->prof->profName << endl;
        } //outputs message detail of this certain element
    }
}

void HashChaining::displayCourseInfo(Course *c) //this function just displays the information for a certain course based on the input into the function
{
    cout << "Course Year: " << c->year << endl;
    cout << "Course Name: " << c->courseName << endl;
    cout << "Course Number: " << c->courseNum << endl;
    cout << "Course Professor: " << c->prof->profName << endl;
}
