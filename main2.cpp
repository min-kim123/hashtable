#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cctype>
#include <cstring>
#include <string.h>
using namespace std;
// /https://www.verywellfamily.com/top-1000-baby-boy-names-2757618
//https://namecensus.com/last-names/

void generate(int num) {
  char line[80];
  int i = 0;
  int random = 0;
  int numLines = 0;
  ifstream File("lastnames.txt");
  ifstream File2("firstnames.txt");//rename to firstnames.txt
  char first[20];
  char last[20];

  while (i != num) {
    i++;
    srand(time(0));
    random = rand() % 500;

    while (File.getline(line, sizeof(line))) {
      ++numLines;
      if (numLines == random) {
	strcpy(last, line);

      }
    }
    numLines = 0;
    random = rand() % 1000;
    while(File2.getline(line, sizeof(line))) {
      ++numLines;
      if (numLines == random) {
	strcpy(first, line);

      }
    }
    cout << first << " " << last << endl;
  }
	
}
int main() {
  int num = 0;
  cout << "How many students? " ;
  cin >> num;
  generate(num);

}
