#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cctype>
#include <cstring>
#include <string.h>
using namespace std;
// /https://www.verywellfamily.com/top-1000-baby-boy-names-2757618
//https://namecensus.com/last-names/

char* GotoLine(fstream& file, int num){
    file.seekg(ios::beg);
    char line[80];
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    file>>line;
    return line;
}

void generate(int num) {
  char line[80];
  int i = 0;
  int random = 0;
  int numLines = 0;

  fstream File("lastnames.txt");
  fstream File2("newlastnames.txt");//rename to firstnames.txt
  char first[80];
  char last[80];
  srand(time(NULL));

  while (i != num) {
    i++;
    random = (rand() % 500);
    //cout << "random number for last: " << random;
    strcpy(last, (GotoLine(File, random)));
    random = (rand() % 1000);
    //cout << "random number for first: " << random;
    strcpy(first, (GotoLine(File2, random)));
    cout << first << " " << last << endl;
  }
}

int main() {
  int num = 0;
  cout << "How many students? " ;
  cin >> num;
  generate(num);
}
