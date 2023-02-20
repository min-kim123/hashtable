#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <string.h>
#include "student.h"
#include "node.h"
using namespace std;

void add(int &newID, int amount, Node** &hashtable, int &arraySize);
int hashfunction(Node* n, int arraySize);
void getLine(fstream& file, int num, char* line);
void print(Node** hashtable, int arraySize);
void deleet(int arraySize, Node** hashtable);
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
    cout << "add or print?: "; 
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
      int id;
      cout << "ID: ";
      cin >> id;
      for (int i = 0; i < arraySize; ++i) {
        if (head == NULL) {
          cout << "Empty list. Nothing to delete" << endl;
        }
        else if (id == head->getStudent()->getID()) {//when the first one is being deleted, special case since head is being changed.
            Node* temp = head;
            if (head->getNext() == NULL) {//there is only one node
                head = NULL;
            }
            else {
                cout << "else  changing head" << endl;
                head = head->getNext();
                delete temp;
            }
        }
        else {
            deleet(head, id);
        }
      }
      
      deleet(arraySize, hashtable);
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
  cout << "total nodes printed: " << total << endl;
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
  int count = 1;
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
      cout << "NOT NULL" << endl;
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
  cout << "1. in rehash" << endl;
  arraySize = arraySize*2;
  Node** newtable = new Node*[arraySize];
  for (int i = 0; i < arraySize; ++i) {//set all slots in newtable to null
    newtable[i] = NULL;
  }
  //node does not get added to the newtable if it is not the first node in the array slot of hashtable
  for (int i = 0; i < arraySize/2; ++i) {//for every slot in hashtable array
    //cout << "2. in for lop" << endl;
    Node* nHashtable = hashtable[i];//nHashtable is the ith sloth in hashtable
    while (nHashtable != NULL) {//while that node in the ith slot is not null
      int key = hashfunction(nHashtable, arraySize);
      Node* nNewtable = newtable[key];//node pointer nNewtable is the first node at the key slot in newtable
      //cout << "3. in while loop" << endl;
      if (nNewtable == NULL) {//it is the first one
        //cout << "5" << endl;
        newtable[key] = nHashtable; //put the node from hashtable into newtable
        newtable[key]->setNext(NULL);
      }
      else{//not the first 
        //cout << "6" << endl;
        while (nNewtable != NULL) {
          //cout << "7" << endl;
          if (nNewtable->getNext() == NULL) {
            //cout << "8" << endl;
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
}

void deleet (int arraySize, Node** hashtable) {
  int input;
  cout << "ID: ";
  cin >> input;
  for (int i = 0; i < arraySize; ++i) {
    cout << i << endl;
    Node* n = hashtable[i];
    if (n != NULL) {
      if (n->getStudent()->getID() == input) {//head is the id
        cout << "2" << endl;
        Node* temp = n;
        if (n->getNext() == NULL) {
          cout << "3" << endl;
          Node* temp = n;
          n = NULL;
          delete temp;
        }
      else {
        hashtable[i] = n->getNext();
        delete temp;
        break;
      }
      
      }
    }
    
    while (n != NULL) {
      if (n->getNext()->getStudent()->getID() == input) {
        Node* temp = n->getNext();
        if (n->getNext()->getNext() == NULL) {
          n->setNext(NULL);
          delete temp;
          break;
        }
        else {
          n->setNext(n->getNext()->getNext());
          delete temp;
          break;
        }

      }
      n = n->getNext();
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