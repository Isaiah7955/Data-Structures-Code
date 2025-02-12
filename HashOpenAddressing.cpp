//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashOpenAddressing.cpp
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"

using namespace std;

/**
 This function is creating a Course according to the inputted data of the year, department, courseNum, courseName and prof given by the lines in the file
 **/

Course *HashOpenAddressing::createNode(int year, string department, int courseNum, string courseName, Professor *prof)
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

HashOpenAddressing::HashOpenAddressing(int size) //parameterized constructor
{
    this->hashTableSize = size;              //initializes the size of the hash table accoridng to the user input
    hashTable = new Course *[hashTableSize]; //creates the hashTable full of Course pointer instances of the hashTableSize
    for (int i = 0; i < size; i++)
    {
        hashTable[i] = nullptr; //makes all of the Course instances nullptr before they have data inputted into them
    }
}

HashOpenAddressing::~HashOpenAddressing() //deconstructor
{

    for (int i = 0; i < hashTableSize; i++)
    {
        if (hashTable[i] != NULL) //goes into this statement if the current Course isn't already NULL
        {
            delete hashTable[i]->prof; //makes sure to delete the instance of the professor seperately because it is a pointer
            delete hashTable[i];       //deletes element in the hashTable array
        }
    }
    delete[] hashTable; //then finally deletes the entire hashTable as to free up data
}

/**
 This is the hash function that is being used...it returns the remainder of the courseNumber modulo hashTableSize
 **/

int HashOpenAddressing::hash(int courseNumber)
{
    int hash = courseNumber % hashTableSize;
    return hash;
}

void HashOpenAddressing::bulkInsert(string filename)
{
    ifstream myFile;
    myFile.open(filename); //opens the file that is passed into function
    string line;

    int collisions = 0; //counters for the collisions and searches
    int searches = 0;

    string year2; //variables to be saved from line of file
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

            istringstream s(line); //uses stringstream to take in every line and seperate it by a comma
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
            prof->profId = profId;                   //saves ID to professor
            search = profDb.searchProfessor(profId); // this is going to check whether the professor is already in the BST

            string name = fname + " " + lname; //saves variable for full professor name
            prof->profName = fname + " " + lname;

            int index = hash(courseNum); //allocate hash function to find our index of where we are putting the element into the hashTable
            Course *pointer = new Course;
            Course *pointer2 = new Course;
            pointer = createNode(year, department, courseNum, courseName, prof); //creates the Course instance
            pointer2 = hashTable[index];                                         //saves the hashTable current index into a Course
            if (hashTable[index] == NULL)                                        //if already NULL then no collision OpenAddressing is needed
            {
                hashTable[index] = pointer;
            }
            else
            {
                collisions++;                    //increment collisions to show that their has been 1
                int i = 1;                       //this is 1 because we have already called the hash fucntion once
                while (hashTable[index] != NULL) //goes until there is an empty space at the current location of the hashTable according to index
                {
                    searches++;                              //increment searches everytime going through while loop
                    index = (index + i * i) % hashTableSize; //use the quadratic probing function to allocate a new location inside the hashTable array
                    i++;
                }
                hashTable[index] = pointer; //then, once exiting the while loop, we add the course into the hashTable
            }
            if (search == NULL) //uses search value from above in function
            {
                profDb.addProfessor(profId, name);      //adds professor if not already in the BST
                prof->coursesTaught.push_back(pointer); //push the course to the correct professors vector as to be used later
            }
            if (search != NULL)
            {
                search->coursesTaught.push_back(pointer); //push the course to the correct professors vector as to be used later
            }
        }
    }

    cout << "[OPEN ADDRESSING] Hash table populated" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Collisions using open addressing: " << collisions << endl;
    cout << "Search operations using open addressing: " << searches << endl; //output message for the user
}

void HashOpenAddressing::search(int courseYear, int courseNumber, string profId)
{
    int index = hash(courseNumber); //call the hash function to start our search
    int counter = 0;                //counter for how many searches are to be done
    Course *pointer = new Course;
    pointer = hashTable[index]; //saves current location of the hashTable

    int i = 1; // this is 1 because we are showing that the hash function has already been done
    bool searchCheck = false; //check if course is not found in the hashTable

    while (hashTable[index] != nullptr) //going until it finds a null space because this will tell us that
    {
        pointer = hashTable[index];
        if ((pointer->year != courseYear) || (pointer->courseNum != courseNumber) || (pointer->prof->profId != profId)) //all information has to be matching to find and produce this output
        {
            index = (index + i * i) % hashTableSize; //use quadratic probing to continue finding new locations to search and check whether it is in the hashTable
            i++; //increment i and the counter for how many searches
            counter++;
        }
        else
        {
            searchCheck = true; //this means it has been found
            break;
        }
    }

    if (searchCheck == false)
    {
        cout << "Course not found. Please try inputting correct information." << endl;
        cout << endl;
    }
    else if (searchCheck == true)
    {
        cout << "Search operations using open addressing: " << counter << endl;
        cout << pointer->year << " " << pointer->courseName << " " << pointer->courseNum << " " << pointer->prof->profName << endl;
    }
}

void HashOpenAddressing::displayAllCourses()
{
    Course *pointer = new Course;

    for (int i = 0; i < hashTableSize; i++) //goes through entire hashTable to output all courses where index is NOT nullptr
    {
        pointer = hashTable[i]; //creates pointer to first element in hash table
        if (pointer != NULL)
        {
            cout << hashTable[i]->year << " " << hashTable[i]->courseName << " " << hashTable[i]->courseNum << " " << hashTable[i]->prof->profName << endl;
        } //outputs message detail of this certain element
    }
}

void HashOpenAddressing::displayCourseInfo(Course *c) //this function just displays the information for a certain course based on the input into the function
{
    cout << "Course Year: " << c->year << endl;
    cout << "Course Name: " << c->courseName << endl;
    cout << "Course Number: " << c->courseNum << endl;
    cout << "Course Professor: " << c->prof->profName << endl;
}
