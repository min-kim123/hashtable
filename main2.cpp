#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <string.h>
#include "student.h"
#include "node.h"
using namespace std;

void add(int &newID, int amount, Node* hashtable[], int &arraySize, int &totalNodes);
int hashfunction(Node* n, int arraySize);
void getLine(fstream& file, int num, char* line);
void print(Node* hashtable[], int arraySize);
void deleet(int arraySize, Node* hashtable[], int &totalNodes);
//void rehash(int arraySize, Node* hashtable[]);

int main() {
  int totalNodes = 0;//total number of students, add to when student added, delete when student deleted
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
    cout << "add or print?: "; 
    cin >> input;
    cin.ignore();
    if (strcmp(input, "add") == 0) {
      cout << "How many students? " ;
      cin >> amount;
      add(newID, amount, hashtable, arraySize, totalNodes);
      //getLine, hashfunction called from add
    }
    else if (strcmp(input, "print")==0) {
      print(hashtable, arraySize);
    }
    else if (strcmp(input, "delete")==0) {
      deleet(arraySize, hashtable, totalNodes);
    }
    else if (strcmp(input, "quit")==0) {
      cont = false;
    }
    else {
      cout << "Invalid input" << endl;
    }
  }
  delete[] hashtable;
}

void print(Node* hashtable[], int arraySize) {
  int total = 0;
  for (int i = 0; i < arraySize; ++i ){
    Node* n = hashtable[i];
    cout << i << ": ";
    while (n != NULL) {
      cout << "|" << n->getStudent()->getFirst() << " " << n->getStudent()->getLast() << " " << setprecision(3) <<n->getStudent()->getGPA() << " " << n->getStudent()->getID() << "|";
      ++total;
      n = n->getNext();
    }
    cout << endl;
  }
  cout << "total nodes printed: " << total << endl;
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

void getLine(fstream& file, int num, char* line){//had to put this stuff outside the generate function because instead of starting from line 1 in started from the previous line
    file.seekg(ios::beg);
    char fileline[80];
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    file>>fileline;
    strcpy(line, fileline);
}

void add(int &newID, int amount, Node* hashtable[], int &arraySize, int &totalNodes) {//make a random student, increment id by 1 each time
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
    int slotsTaken = 0;
    Node* n = hashtable[key];
    if (n == NULL) {//newnode is first one with that key
      hashtable[key] = newnode;
      ++slotsTaken;//if this reaches half the array size, rehash
    }
    else {
      cout << "NOT NULL" << endl;
      while (n!= NULL) {
        ++count;
        if (n->getNext() == NULL) {
          n->setNext(newnode);
          break;
        }
        n = n->getNext();
        if (count == 2) {
          cout << "-------------rehash" << endl;
         //rehash(arraySize, hashtable);
        }
      }
    }
    ++totalNodes;//add 1 student
    cout << newfirst << " " << newlast << " " << newGPA << " " << newID << " " << key << endl;
  }
}
/*
void rehash(int arraySize, Node* hashtable[]) {
  cout << "-----------REHASH-----------" << endl;
  arraySize = arraySize*2;
  Node* newtable[arraySize];
  for (int i = 0; i < arraySize; ++i) {
    newtable[i] = NULL;
  }
  for (int i = 0; i < arraySize/2; ++i) {//copy hashtable into temp
    Node* nHashtable = hashtable[i];
    while (nHashtable != NULL) {
      for (int i = 0; i < arraySize; ++i) {
        if (i == hashfunction(nHashtable, arraySize)) {
          Node* nNewtable = newtable[i];
          if(nNewtable == NULL) {//it is the first one
            nNewtable = nHashtable;
          }
          else{//not the first 
            if (nNewtable->getNext() == NULL) {
              nNewtable->setNext(nHashtable);
              nNewtable = nHashtable;
              //break;
            }
            nNewtable = nNewtable->getNext();
          }
        }
      }
      nHashtable = nHashtable->getNext();
    }
  }
  hashtable = &newtable[0];

  //print(hashtable, arraySize);
  for (int i = 0; i < arraySize; ++i ){
    Node* n = hashtable[i];
    cout << i << ": ";
    while (n != NULL) {
      cout << "|" << n->getStudent()->getFirst() << " " << n->getStudent()->getLast() << " " << setprecision(3) <<n->getStudent()->getGPA() << " " << n->getStudent()->getID() << "|";
      n = n->getNext();
    }
    cout << endl;
  }
}
*/
void deleet (int arraySize, Node* hashtable[], int &totalNodes) {
  --totalNodes;//subtract 1
  int input;
  cout << "ID: ";
  cin >> input;
  for (int i = 0; i < arraySize; ++i) {
    Node* n = hashtable[i];
    while (hashtable[i]!= NULL) {
      if (n->getStudent()->getID() == input) {

        break;
      }
      n = n->getNext();
    }
  }
}