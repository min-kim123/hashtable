#include <iostream>
#include <string.h>
#include <fstream>//work with files
using namespace std;
// /https://www.verywellfamily.com/top-1000-baby-boy-names-2757618
//https://namecensus.com/last-names/

int main()
{
	string line;
	int random = 0;
	int numLines = 0;
	ifstream File("lastnames.txt");
	ifstream File2("newlastnames.txt");//rename to firstnames.txt
	char first[20];
	char last[20];

	srand(time(0));
	random = rand() % 500;
	while (getline(File, line)) {
		++numLines;
		if (numLines == random) {
			strcpy(last, line.c_str());
		}
	}
	numLines = 0;
	random = rand() % 1000;
	while(getline(File2, line)) {
		++numLines;
		if (numLines == random) {
			strcpy(first, line.c_str());
		}
	}
	cout << first << " " << last << endl;
}