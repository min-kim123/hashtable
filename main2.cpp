#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <string.h>
#include "student.h"
#include "node.h"
using namespace std;
/*
Author: Min Kim
Program Description: This is a program that will store the first name, last name, gpa, and id of students
in a hash table that grows as more nodes are added. It can generate random student names and also take 
manual inputs.
Date: 3/9/22
*/

void add(int &newID, int amount, Node** &hashtable, int &arraySize);
int hashfunction(Node* n, int arraySize);
void getLine(fstream& file, int num, char* line);
void print(Node** hashtable, int arraySize);
void deleet (Node** &hashtable, int arraySize, int id);
void rehash(int &arraySize, Node** &hashtable);
void add_manual(int &newID, int &arraySize, Node** &hashtable);

int main() {
  int newID= 0;//ID number, incremented by 1 for each new student
  int amount = 0;
  bool cont = true;
  char input[20];
  int arraySize = 100;
  Node** hashtable = new Node*[arraySize];
  //Node* hashtable[arraySize];//hashtable, starts at 100, doubles in size when there are 4 collisions
  //set all slots to null
  for (int i = 0; i < arraySize; ++i) {
    hashtable[i] = NULL;
  }
  while (cont == true) {
    cout << "Add manually, generate, print, delete, or quit? (add/generate/print/delete/quit): "; 
    cin >> input;
    cin.ignore();
    if (strcmp(input, "generate") == 0) {
      cout << "How many students? " ;
      cin >> amount;
      add(newID, amount, hashtable, arraySize);
      //getLine, hashfunction called from add
    }
    else if (strcmp(input, "add")==0) {
      add_manual(newID, arraySize, hashtable);
    }
    else if (strcmp(input, "print")==0) {
      print(hashtable, arraySize);
    }
    else if (strcmp(input, "delete")==0) {
      int id = 0;
      int iterations = 0;
      cout << "ID: ";
      cin >> id;
      deleet(hashtable, arraySize, id);
    }
    else if (strcmp(input, "quit")==0) {
      cont = false;
    }
    else {
      cout << "Invalid input" << endl;
    }
  }
}

void print(Node** hashtable, int arraySize) {
  int total = 0;
  for (int i = 0; i < arraySize; ++i ){
    Node* n = hashtable[i];
    cout << i << ": ";
    while (n != NULL) {
      cout << "|" << n->getStudent()->getFirst() << " " << n->getStudent()->getLast()  << " " << n->getStudent()->getID()  << " ";
      cout << fixed << setprecision(2);
      cout << n->getStudent()->getGPA()<<"|";
      ++total;
      n = n->getNext();
    }
    cout << endl;
  }
}

void getLine(fstream& file, int num, char* line){//had to put this stuff outside the generate function because instead of starting from line 1 in started from the previous line
    file.seekg(ios::beg);
    char fileline[80];
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    file>>fileline;
    strcpy(line, fileline);
}

void add_manual(int &newID, int &arraySize, Node** &hashtable) {
  int count = 0;
  ++newID;
  char newfirst[20];
  char newlast[20];
  float newGPA;
  cout << "First Name: ";
  cin >> newfirst;
  cin.ignore();
  cout << "Last Name: ";
  cin >> newlast;
  cin.ignore();
  cout << "ID: " << newID<<endl;
  cout << "GPA: " ;
  cin >> newGPA;   

  Student* newstudent = new Student(newfirst, newlast, newID, newGPA);
  Node* newnode = new Node(newstudent);//put newstudent into new node
  int key = hashfunction(newnode, arraySize);//make the key for the newnode

  Node* n = hashtable[key];
  if (n == NULL) {//newnode is first one with that key
    hashtable[key] = newnode;
  }
  else {
    while (n!= NULL) {
      ++count;
      if (n->getNext() == NULL) {
        if (count == 3) {
          cout << "-------------rehash" << endl;
          rehash(arraySize, hashtable);
          break;
        }
        n->setNext(newnode);
        break;
      }
      n = n->getNext();
    }
  }
}
void add(int &newID, int amount, Node** &hashtable, int &arraySize) {//make a random student, increment id by 1 each time
  int random = 0;
  int i = 0;
  fstream File("lastnames.txt");
  fstream File2("newlastnames.txt");//rename to firstnames.txt
  char newfirst[80];
  char newlast[80];
  srand(time(NULL));
  while (i != amount) {
    ++i;
    ++newID;//increment the ID
    char line[80];
    random = (rand() % 500);//there are 500 names in lastnames.txt
    getLine(File, random, line);
    strcpy(newlast, line);
    random = (rand() % 1000);//there are 1000 names in firstnames.txt
    getLine(File2, random, line);
    strcpy(newfirst, line);
    //get random gpa range 0-4.5
    float newGPA = (rand()%450);
    newGPA = newGPA/100;
    Student* newstudent = new Student(newfirst, newlast, newID, newGPA);
    Node* newnode = new Node(newstudent);//put newstudent into new node
    int key = hashfunction(newnode, arraySize);//make the key for the newnode

    //add it into the table
    int count = 1;//to count number of nodes in linked list
    Node* n = hashtable[key];
    if (n == NULL) {//newnode is first one with that key
      hashtable[key] = newnode;
    }
    else {
      while (n!= NULL) {
        ++count;
        if (n->getNext() == NULL) {
          if (count == 3) {
            cout << "-------------rehash" << endl;
            rehash(arraySize, hashtable);
            break;
          }
          n->setNext(newnode);
          break;
        }
        n = n->getNext();
      }
    }
    cout << newfirst << " " << newlast << " " << newGPA << " " << newID << " " << key << endl;
  }
}

void rehash(int &arraySize, Node** &hashtable) {
  arraySize = arraySize*2;
  Node** newtable = new Node*[arraySize];
  for (int i = 0; i < arraySize; ++i) {//set all slots in newtable to null
    newtable[i] = NULL;
  }
  //node does not get added to the newtable if it is not the first node in the array slot of hashtable
  for (int i = 0; i < arraySize/2; ++i) {//for every slot in hashtable array
    Node* nHashtable = hashtable[i];//nHashtable is the ith sloth in hashtable
    while (nHashtable != NULL) {//while that node in the ith slot is not null
      int key = hashfunction(nHashtable, arraySize);
      Node* nNewtable = newtable[key];//node pointer nNewtable is the first node at the key slot in newtable
      if (nNewtable == NULL) {//it is the first one
        newtable[key] = nHashtable; //put the node from hashtable into newtable
        newtable[key]->setNext(NULL);
      }
      else{//not the first 
        while (nNewtable != NULL) {
          if (nNewtable->getNext() == NULL) {
            nNewtable->setNext(nHashtable);
            nNewtable->getNext()->setNext(NULL);
            break;
          }
          nNewtable = nNewtable->getNext();
        }
      }
      nHashtable = nHashtable->getNext();
    }
  }
  delete[] hashtable;
  hashtable = newtable;
}

void deleet (Node** &hashtable, int arraySize, int id) {
  for (int i = 0; i < arraySize; ++i) {
    if (hashtable[i] != NULL) {
      if (id == hashtable[i]->getStudent()->getID()) {//if first node is equal
        if (hashtable[i]->getNext() == NULL) {//just delete 
          hashtable[i] = NULL;
        }
        else {
          hashtable[i] = hashtable[i]->getNext();
        }
      return;
      }
    }
  }
  for (int i = 0; i < arraySize; ++i) {
    Node* n = hashtable[i];
    if (hashtable[i] != NULL) {
      while (n->getNext() != NULL) {
        if (id == n->getNext()->getStudent()->getID()) {//if NEXT node is equal, separate because i have to reconnect
          Node* temp = n->getNext();
          n->setNext(n->getNext()->getNext());
          delete temp;
          return;
        }
        else {
          n = n->getNext();
        }
      }
    }
  }
}

int hashfunction(Node* n, int arraySize) {//how to get the key
  char last[80];
  strcpy(last, n->getStudent()->getLast());//copy the lastname char array into last
  //add all ascii values of last name and mod
  int i = 0;
  int total = 0;
  while (last[i] != '\0') {
    total = total+(int)last[i];
    ++i;
  }
  int key = total%arraySize;
  return key;
}