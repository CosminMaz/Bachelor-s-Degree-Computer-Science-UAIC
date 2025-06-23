%{
#include <stdio.h>
#include "limbaj.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string"|"bool"|"void" {yylval.string = strdup(yytext);return TYPE;}

"true" { return TRUE; }
"false" { return FALSE; }

"begin_progr" {return BGIN;}
"end_progr" {return END;}

"if" {return IF;}
"else" {return ELSE;}

"while" {return WHILE;}
"for" {return FOR;} 

"return" { return RETURN_FUNC;}

"class" {return CLASS;}
"function" {return FUNCTION;}

"TypeOf" { return TO; }
"Print" { return PRINT; }

"'"[a-zA-Z]"'" { yylval.string = strdup(yytext); return CHAR; } //For Char: ex 'A'
"'"[a-zA-Z ]*"'" { yylval.string = strdup(yytext); return STRING; } //For Strings: ex "Abcd"
[0-9]+\.[0-9]* { yylval.string = strdup(yytext); return FLOAT; } //Floats: 3.4

[a-zA-Z][a-zA-Z0-9_]* {yylval.string = strdup(yytext); return ID;} //Name of the variable
[0-9]+ {yylval.integer = atoi(yytext); return NR; }

"="     { return EQUAL; }
"=="    { return EQ; }
"!="    { return NEQ; }
"<"     { return LT; }
"<="    { return LEQ; }
">"     { return GT; } 
">="    { return GEQ; }
"+"     { return '+'; }
"-"     { return '-'; }
"*"     { return '*'; }
"/"     { return '/'; }
"("     { return '('; }
")"     { return ')'; }
"{"     { return '{'; }
"}"     { return '}'; }
";"     { return ';'; }
","     { return ','; }

"&&"    { return AND;}
"||"    { return OR;}

[ \t] ;
\n {yylineno++;}
. {return yytext[0];}

