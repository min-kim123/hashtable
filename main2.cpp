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

void add(Node* newnode, Node* hashtable[], int &arraySize, int key);
int hashfunction(Node* n, int arraySize);
void GotoLine(fstream& file, int num, char* line);
void generate(int &newID, int amount, Node* hashtable[], int arraySize);
void printArray(Node* hashtable[], int arraySize);

void printArray(Node* hashtable[], int arraySize) {
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
//data will be searched for based on the ID
void add(Node* newnode, Node* hashtable[], int &arraySize, int key) {//now add newnode into hashtable
  int count = 0;//to count number of nodes in linked list
  int slotsTaken = 0;
  Node* n = hashtable[key];
  if (n == NULL) {//newnode is first one with that key
    hashtable[key] = newnode;
    ++slotsTaken;//if this reaches half the array size, rehash
  }
  else {
    cout << "NOT NULL" << endl;
    while (n!= NULL) {
      if (n->getNext() == NULL) {
        n->setNext(newnode);
        break;
      }
      n = n->getNext();
    }
    /*
    while (n != NULL) {
      ++count;
      if (count == 3) {
        cout << "Rehashing" << endl;
        break;
        
        Node* temp[arraySize] = hashtable;
        arraySize = arraySize*2;
        hashtable = new Node*[arraySize];//hashtable now has twice the slots
        //now spread out the data throughout the new array
        
      }
      if (n->getNext() == NULL) {
        n->setNext(newnode);
      }
      n = n->getNext();
    }*/
  }
}

int hashfunction(Node* n, int arraySize) {//how to get the key
  char first[80];
  char last[80];
  strcpy(first, n->getStudent()->getFirst());//copy the firstname char array into first
  strcpy(last, n->getStudent()->getLast());//copy the lastname char array into last
  int firstNum = (int)first[0];//first letter of first name
  int firstNum2 = (int)first[1];
  int lastNum = (int)last[0];//first letter of last name
  int lastNum2 = (int)last[1];//second letter of last name
  int key = (firstNum+firstNum2+lastNum+lastNum2)%(arraySize);
  return key;
}

void GotoLine(fstream& file, int num, char* line){//had to put this stuff outside the generate function because instead of starting from line 1 in started from the previous line
    file.seekg(ios::beg);
    char fileline[80];
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    file>>fileline;
    strcpy(line, fileline);
}

void generate(int &newID, int amount, Node* hashtable[], int arraySize) {//make a random student, increment id by 1 each time
  int random = 0;
  int i = 0;

  fstream File("lastnames.txt");
  fstream File2("newlastnames.txt");//rename to firstnames.txt
  char newfirst[80];
  char newlast[80];
  srand(time(NULL));

  while (i != amount) {//had to put the while loop in the generate since it generated same name over again
    ++i;
    ++newID;//increment the ID
    char line[80];
    random = (rand() % 500);//there are 500 names in lastnames.txt
    GotoLine(File, random, line);
    strcpy(newlast, line);
    random = (rand() % 1000);//there are 1000 names in firstnames.txt
    GotoLine(File2, random, line);
    strcpy(newfirst, line);

    //get random gpa range 0-4.5
    float newGPA = (rand()%450);
    newGPA = newGPA/100;
    
    Student* newstudent = new Student(newfirst, newlast, newID, newGPA);
    Node* newnode = new Node(newstudent);//put newstudent into new node
    int key = hashfunction(newnode, arraySize);//make the key for the newnode
    add(newnode, hashtable, arraySize, key);//now put the new node into the hashtable
    cout << newfirst << " " << newlast << " " << newGPA << " " << newID << " " << key << endl;
  }
}

void deleet (int arraySize, Node* hashtable[]) {
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

int main() {
  int newID= 0;//ID number, incremented by 1 for each new student
  int amount = 0;
  bool cont = true;
  char input[20];
  int arraySize = 100;
  Node* hashtable[arraySize];//hashtable, starts at 100, doubles in size when there are 4 collisions
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
      generate(newID, amount, hashtable, arraySize);
    }
    else if (strcmp(input, "print")==0) {
      printArray(hashtable, arraySize);
    }
    else {
      cout << "Invalid input" << endl;
    }
  }
}