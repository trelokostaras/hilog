#include"par.h"

using namespace std;

extern list<Clause> program;

/*returns the concatenation of list1 and list2*/
template<class T>
list<T> merge(list<T> list1, list<T> list2) {
	list1.splice(list1.end(),list2);
	return list1;
}

void substituteOneVar(Term &t,VarVal s) {
	/* make the substitution to all the argumensts */
	for (std::list<list<Term> >::iterator it = t.args.begin(); it != t.args.end(); ++it) {
		for(std::list<Term>::iterator i = it->begin(); i != it->end(); ++i)
			substituteOneVar(*i,s);
	}
	if(t.varid == s.varid && s.varid != -1) { /* t is a Variable */
		t.varid = s.term.varid;
		t.name = s.term.name;
		t.args = merge(s.term.args,t.args);
	}
}

void substitute(Term &t,list<VarVal> subst) {
	/*iterate through all the substitutions*/
	for (std::list<VarVal>::iterator iter=subst.begin(); iter != subst.end(); ++iter) {
		substituteOneVar(t,*iter);
		/*make the substitution to rest substitution */
		for (std::list<VarVal>::iterator it = iter; it != subst.end(); ++it) {
			substituteOneVar(it->term,*iter);
		}
	}
}

bool unifyVar(int varid,Term t, list<VarVal> &th) {
	bool alreadyIn = false;
	for (std::list<VarVal>::iterator it = th.begin(); it != th.end(); ++it) {
		if(it->varid == varid) {
			alreadyIn = true;
			return unify(it->term,t,th);
		}		
	}
	if(alreadyIn == false) {
		th.push_back(VarVal(varid,t));
		return true;
	}
}
/**/
bool unify(Term t1,Term t2,list<VarVal> &th) {
	//cout << " Trying to unify term1 is " << t1 << " Term2is " << t2 << " subst " << th << endl;
	while(!t1.args.empty() && !t2.args.empty()) {
		list<Term> it1 = *t1.args.rbegin(), it2 = *t2.args.rbegin();
		t1.args.pop_back(); t2.args.pop_back();
		if(it1.size() == it2.size()){
			for(std::list<Term>::iterator i1 = it1.begin(), i2 = it2.begin(); i1 != it1.end() && i2 != it2.end(); i1++, i2++) {
				if(unify(*i1,*i2,th) == false) {
					return false;
				}
			}
		}
		else{
			return false;
		}
	}
	if(t1.args.empty() && !t2.args.empty()){
		if(t1.varid != -1) 
			return unifyVar(t1.varid,t2,th);
		
		else
			return false;
	}
	else if(t2.args.empty() && !t1.args.empty()){
		if(t2.varid != -1) 
			return unifyVar(t2.varid,t1,th);
		else 
			return false;
	}
	else {
		if(t1.varid != -1) {
			return unifyVar(t1.varid,t2,th);
		}
		else if(t2.varid != -1) {
			return unifyVar(t2.varid,t1,th);
		}
		else{
			return t1.name == t2.name;
		}
	}
	//cout << "SUCESS term1 is " << t1 << " Term2is " << t2 << " subst " << th << endl;
}

void standaralize(Term cc,list<VarVal> &vars) {
	for(list<list<Term> >::iterator it = cc.args.begin();it != cc.args.end(); ++it) {
		for(list<Term>::iterator i = it->begin();i != it->end(); ++i) {
			standaralize(*i,vars);	
		}
	}
	if(cc.varid != -1) {
/*
		bool flag = false;
		for(list<VarVal>::iterator it = vars.begin();it != vars.end(); ++it) {
			if(it->varid == cc.varid) {
				flag = true;
				break;
			}
		}
		if(flag == false) {
			vars.push_back(VarVal(cc.varid,Term(cc.name,nextvar())));
		}
*/
		vars.push_back(VarVal(cc.varid,Term(cc.name,nextvar())));
/**/
	}	
}

/*rename the variables of the clause*/
void standaralize(Clause &cc) {
//	cout << "Before " << cc << endl;
	list<VarVal> vars;
	for(list<Term>::iterator it = cc.head.begin();it != cc.head.end(); ++it) {
		standaralize(*it,vars);	
	}
	for(list<Term>::iterator it = cc.body.begin();it != cc.body.end(); ++it) {
		standaralize(*it,vars);	
	}

	for(list<Term>::iterator it = cc.head.begin();it != cc.head.end(); ++it) {
		substitute(*it,vars);	
	}
	for(list<Term>::iterator it = cc.body.begin();it != cc.body.end(); ++it) {
		substitute(*it,vars);	
	}
//	cout << "After " << cc << endl;
}


#include<stdio.h>
/*print the  minimun substitution of the goal*/
void pr(list<Term> goal,list<VarVal> subst) {
	for(list<Term>::iterator it = goal.begin(); it !=  goal.end(); ++it){
		for(list<list<Term> >::iterator i = it->args.begin(); i !=  it->args.end(); ++i){
			pr(*i,subst);	
		}
		/*print it if it is a named variable*/
		if(it->varid != -1 && it->name != "_") {
			cout << it->name << " = ";
			Term n;
			n.varid = it->varid;
			substitute(n,subst);
			cout << n << endl;
		}
	}
}

/*sld-resolution*/
bool sld(list<Term> goal,list<VarVal> subst,list<Term> initialgoal) {
	if(goal.empty()) {
		pr(initialgoal,subst);
		cout << "yes" << endl << endl;
		return true;
	}
	else {
		Term first = *goal.begin();
		goal.pop_front(); /*pop the first term from the goal*/
		if(first.name == "!"){
			sld(goal,subst,initialgoal); 
			return false;
		}
		for(list<Clause>::iterator it = program.begin();it != program.end(); ++it) {
			Clause currentClause = *it;	/*make a copy of the clause*/
			standaralize(currentClause);    /*make the variables unique*/
			/*unify the first Term of the Goal with the head of each clause */
			list<VarVal> ss;
			if(unify(currentClause.head.front(),first,ss)) {
				list<Term> newgoal = merge(currentClause.body,goal); /* */
				for(list<Term>::iterator it = newgoal.begin();it != newgoal.end(); ++it) {
					substitute(*it,ss);  
				}
				if(sld(newgoal,merge<VarVal>(subst,ss),initialgoal) == false)
					return false;
			}
		}
		return true;
		//cout << "return" << endl;
	}
}

/*ektypwsh antikatastashs */
ostream& operator<< (ostream &out, list<VarVal> &t) {
	out << "{";
	if(t.size() != 0) { /*Term Arguments*/
		list<VarVal>::iterator it;
		for(it =  t.begin(); it != --t.end();++it)
			out << *it << ",";
		out << *it;
	}
	cout << "}";
}

