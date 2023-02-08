#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iomanip>//include for setprecision thing
#include <string.h>
#include "student.h"
#include "node.h"
using namespace std;

char* GotoLine(fstream& file, int num){//had to put this stuff outside the generate function because instead of starting from line 1 in started from the previous line
    file.seekg(ios::beg);
    char line[80];
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    file>>line;
    return line;
}

void generate(int& newID, int amount) {//make a random student, increment id by 1 each time
  int random = 0;
  int i = 0;
  int numLines = 0;

  fstream File("lastnames.txt");
  fstream File2("newlastnames.txt");//rename to firstnames.txt
  char newfirst[80];
  char newlast[80];
  srand(time(NULL));

  while (i != amount) {
    ++i;
    ++newID;
    random = (rand() % 500);//there are 500 names in lastnames.txt
    strcpy(newlast, (GotoLine(File, random)));
    random = (rand() % 1000);//there are 100 names in firstnames.txt
    strcpy(newfirst, (GotoLine(File2, random)));
    cout << newfirst << " " << newlast;

    //get random gpa
    float newGPA = (rand()%500);
    newGPA = newGPA/100;
    cout <<newGPA << " " << newID << endl;
    
    Student* newstudent = new Student(newfirst, newlast, newID, newGPA);
    Node* newnode = new Node(newstudent);//put newstudent into new node
  }
}



//void add() {
  //add to the array
//}

int main() {
  int amount = 0;
  int newID= 0;
  bool cont = true;
  while (cont == true) {
    cout << "How many students? " ;
    cin >> amount;
    if (amount > 0) {
      int i = 0;
      generate(newID, amount);
    }
    else {
      cont = false;
    }
    
    
  }
  
}