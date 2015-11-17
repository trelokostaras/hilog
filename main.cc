#include<iostream>
#include"par.h"
#include<list>
#include<fstream>
	
using namespace std;

list<Clause> program;
istream *stream;
char current; /*current character from input stream*/

int main(int argc, char *argv[]) {
	char *filename = NULL;
	if(argc == 2) {
		filename = argv[1];
		cout << "reading " << filename << endl;
	}
	else {
		cout << "Wrong input" << endl;
		return 1;
	}

	ifstream file;
	file.open(filename);
	file >> current;
	stream = &file;

	program = allClauses();
	for(list<Clause>::iterator it = program.begin();
		it != program.end();
		++it) {
		cout << *it << endl;
	}
/**/
	cout << "------------------------" << endl;
	stream = &cin;
	while(true){
		cout << "?-";
		list<Term> gg = goal();
		for(list<Term>::iterator it = gg.begin(); it !=  gg.end(); ++it){
			cout << *it << ',';
		}
		cout << endl;
		list<VarVal> tth;
		sld(gg,tth,gg);
	}

/**/
	return 0;
}
