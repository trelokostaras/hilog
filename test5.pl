parent(able,adam).
parent(able,eve).
parent(cain,adam).
parent(cain,eve).
parent(eve,bob).
parent(eve,john).
parent(john,kon).


closure(R)(X,Y) :- R(X,Y).
closure(R)(X,Y) :- R(X,Z), closure(R)(Z,Y).

map(F)(nil,nil).
map(F)(l(X,Xs),l(Y,Ys)) :- F(X,Y), map(F)(Xs,Ys).
