#!/bin/bash

echo "compiling $1"
rm -f lex.yy.c
rm -f $1.tab.c
rm -f $1
bison -d -Wcounterexamples $1.y
lex $1.l
g++ Globals.cpp ASTNode.cpp SymTable.cpp lex.yy.c  $1.tab.c -o $1
