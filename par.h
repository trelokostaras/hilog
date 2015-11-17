#include<iostream>
#include<list>

using namespace std;

class Term {
public:
	int varid;	/* -1 if is not variable*/
	string name;
	list< list<Term> > args;
	
	Term(){};
	Term(string n,list< list<Term> > l):name(n) , varid(-1),args(l){};
	Term(string n,int varid):name(n) , varid(varid),args(list<list<Term> >()){};
};


class Clause {
public:
	list<Term> head;
	list<Term> body;

};

using namespace std;

Term term(list<Term> &vars);
list<Term> args(list<Term>& vars);
Term pred(list<Term> &vars);
list<Term> predlist(list<Term> &vars);
Clause clause(list<Term> &vars);
list<Clause> allClauses();

#define LINEBUFERSIZE 100

class VarVal {
public:
	int varid;
	Term term;
	VarVal(int id,Term t):varid(id),term(t) {}
};

int nextvar();

list<Term> goal();
void substitute(Term &t,list<VarVal> subst);
void substituteClause(Clause &c,list<VarVal> subst);
bool unify(Term t1,Term t2,list<VarVal> &th);

void standaralize(Clause &cc);
bool sld(list<Term> goal,list<VarVal> subst,list<Term> initialgoal);

ostream& operator<< (ostream &out, Term &t);
ostream& operator<< (ostream &out, VarVal &t);
ostream& operator<< (ostream &out, list<VarVal> &t);
ostream& operator<< (ostream &out, Clause &t);
