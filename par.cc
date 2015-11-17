#include"par.h"
#include<fstream>
#include<stdlib.h>

using namespace std;

extern istream *stream;
//extern ifstream file;
extern char current;
int count = 0;

int nextvar() { return ++count; }


string name() {
	istream &file = *stream;
	char tmp[100];
	int i = 0;
	while((current >= 'a' && current <= 'z') || (current >='A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_' || current == '!') {
		tmp[i] = current;
		i++;
		file >> current;
	}
	tmp[i] = '\0';
	return string(tmp);
}

void skipwhitespace() {
	istream &file = *stream;
	while(current == ' ' || current == '\t' || current == '\n')
		file >> current;
}

void makeVar(list<Term> &vars, Term &t){
	bool f = false;
	/*look if the variable already exist*/
	for (std::list<Term>::iterator it=vars.begin(); it != vars.end(); ++it) {
		if(it->name == t.name) {
			t.varid  = it->varid;
			f = true;
			break;
		}
	}
	/*Assign  a new variable id if the variable doent exist or is the anonymous variable '_'*/
	if(f == false || t.name == "_"){
		t.varid = nextvar();
		t.args = list<list<Term> >();
		vars.push_front(t);
	}
}

Term term(list<Term> &vars) {
	istream &file = *stream;
	Term t;
	t.name = '@';
	t.varid = -1;
	if((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || current == '!' || current == '_'){
		char ff = current;
		t.name = name();
		if((ff>= 'A' && ff<= 'Z') || ff == '_')
			makeVar(vars,t);
		t.args = list<list<Term> >();
		bool b = true;
		while(b){
			switch(current){
				case '(':
					t.args.push_back(list<Term>());
					do{
						file >> current;
						skipwhitespace();
						(--t.args.end())->push_back(term(vars));
						skipwhitespace();
					}while(current == ',');
					if(current == ')') {
						file >> current;
					}
					else { 
						cout << "parse error1" << endl;
						exit(1);
					}
					break;
				default :
					b = false;
					break;
			}
		}
	}
	else { 
		cout << "parse error2" << endl;
		exit(1);
	}
	return t;
}

Clause clause(list<Term> &vars) {	
	istream &file = *stream;
	Clause cc;	
	cc.head = list<Term>();
	cc.body = list<Term>();
	skipwhitespace();
	cc.head.push_back(term(vars));
	skipwhitespace();
	if( current == '.') {
		file >> current;
		return cc;
	}
	else if( current == ':') {
		file >> current;
		if( current == '-') {
			do {
				file >> current;
				skipwhitespace();
				cc.body.push_back(term(vars));
				skipwhitespace();
			}while(current == ',');
			if( current == '.') {
				file >> current;
				return cc;
			}
			else {
				cout << "1parse error" << endl;
				exit(1);
			}
		}
		else {
			cout << "2parse error" << endl;
			exit(1);
		}
	}
	else {
		cout << "3parse error" << endl;
		exit(1);
	}
	return cc;
}

list<Clause> allClauses() {
	istream &file = *stream;
	list<Clause> lis;
	while(!file.eof()) {
		list<Term> vars;
		skipwhitespace();
		Clause tmp = clause(vars);
/*
		cout << "reading " << tmp << endl;
		char d;
		cin >> d;
*/
		lis.push_back(tmp);
		skipwhitespace();
	}
	return lis;
}

list<Term> goal() {
	istream &file = *stream;
	list<Term> vars;
	list<Term> cc;
	do {
		file >> current;
		skipwhitespace();
		cc.push_back(term(vars));
		skipwhitespace();
	}while(current == ',');
	if( current == '.') {
		//file >> current;
		return cc;
	}
	else {
		cout << "1parse error" << endl;
	}
	return cc;
}

ostream& operator<< (ostream &out, Clause &t) {
	if(t.head.size() != 0) { /*Term Arguments*/
		list<Term>::iterator it;
		for(it =  t.head.begin(); it != --t.head.end();++it)
			out << *it << ",";
		out << *it;
	}
	if(t.body.size() != 0) { /*Term Arguments*/
		cout << ":-";
		list<Term>::iterator it;
		for(it =  t.body.begin(); it != --t.body.end(); ++it)
			out << *it << ",";
		out << *it;
	}
	cout << '.';
}

ostream& operator<< (ostream &out, Term &t) {
	if(t.varid != -1){	/*Variable*/
		out << "_" << t.varid;
	}
	else {
		out << t.name;
	}
	for(list<list<Term> >::iterator ss = t.args.begin();ss != t.args.end();++ss){
		if(ss->size() != 0) { /*Term Arguments*/
			out << "(";
			list<Term>::iterator it;
			for(it =  ss->begin(); it != --ss->end();++it)
				out << *it << ",";
			out << *it << ")";
		}
	}
	return out;
}

ostream& operator<< (ostream &out, VarVal &t) {
	cout << '_' << t.varid << '/' << t.term ;
}

