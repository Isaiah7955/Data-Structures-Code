//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: ProfBST.cpp
//-----------------------------------------------------------------------------

#include "ProfBST.h"

#include <iostream>

using namespace std;

 /**
 This function will create a node using the professors ID and their name and put it in the BST and organize by their ID value
 **/

Professor* ProfBST:: createNode(string profId, string profName) 
{
    Professor* newNode = new Professor;
    newNode->profId = profId;
    newNode->profName = profName;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

 /**
 This function will destroy the subtree rooted at currNode.
 **/

void ProfBST:: destroyNode(Professor *currNode){
     if(currNode!=NULL)
     {
         destroyNode(currNode->left); //looks through to find the node using recursion
         destroyNode(currNode->right);

         delete currNode;
         currNode = NULL;
     }
 }

ProfBST::ProfBST() //parameterized constructor
{
    root = nullptr;
}

ProfBST::~ProfBST() //destructor calling the destroyNode functiomn
{
     destroyNode(root);
}

 /**
 This function is the helper for the addNode function as it recurisevly compares the current node to the one in the current position
  and decides if this is where it is to be placed or to keep searching deeper within the tree
 **/

Professor* ProfBST:: addNodeHelper(Professor* currNode, string profId, string profName)
{
    if(currNode == NULL){
        return createNode(profId, profName);
    }
    else if(currNode->profId < profId){
        currNode->right = addNodeHelper(currNode->right,profId, profName);
    }
    else if(currNode->profId > profId){
        currNode->left = addNodeHelper(currNode->left, profId, profName);
    }
    return currNode;

}

 /**
 This function is just calling the addNodeHelper and starting at the root and it is going to return the createNode function when it finds the correct spot
 **/

void ProfBST::addProfessor(string profId, string profName)
{
    root = addNodeHelper(root, profId, profName);
}

 /**
 This function is the helper function for the searchProfessor node
 It takes in a current position of node and the professor's ID and recurisvely compares it to all ID's in BST until it finds the right location
 **/

Professor* ProfBST::searchKeyHelper(Professor* currNode, string profId){
    if(currNode == NULL)
    {
        return NULL;
    }
    if(currNode->profId == profId)
    {
        return currNode;
    }
    if(currNode->profId > profId)
    {
        return searchKeyHelper(currNode->left, profId);
    }
    return searchKeyHelper (currNode->right, profId);
}

 /**
 This function just calls the helper function and returns the node that it produces
 **/

Professor* ProfBST::searchProfessor(string profId)
{
    Professor* tree = searchKeyHelper(root, profId);
    if(tree != NULL) {
        return tree;
    }
    return NULL;
}

 /**
 This function is for the main function and first calls the searchProfessor function and then the displayProfessorInfo function
 **/

void ProfBST::publicSearchProfessor(string profId)
{
    Professor* pointer = new Professor;
    pointer = searchProfessor(profId);
    displayProfessorInfo(pointer);
}

 /**
 This function goes through the vector of the courses taught by a certain professor and outputs them to be used in the publicSearchProfessor function
 **/

void ProfBST::displayProfessorInfo(Professor* p)
{
    cout << "Name: " << p->profName << endl;
    for(int i=0; i < p->coursesTaught.size(); i++)
    {
        cout << "-" << p->coursesTaught[i]->courseNum << ": " << p->coursesTaught[i]->courseName << ", " << p->coursesTaught[i]->year << endl;
    }
}
