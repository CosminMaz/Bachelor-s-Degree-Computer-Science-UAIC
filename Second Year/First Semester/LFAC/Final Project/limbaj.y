%{
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "SymTable.h"
#include "Globals.h"
#include "ASTNode.h"

using namespace std;

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();

void yyerror(std::string s);



int errorCount = 0;

%}

%union {
     char* string;
     int integer;
}

%token '=' '+' '-' '*' '/' '(' ')' '{' '}' ';' ',' LT GT LEQ GEQ EQ NEQ AND OR
%token IF
%token ELSE
%token FOR 
%token WHILE
%token BGIN END CLASS EQUAL FUNCTION TO PRINT RETURN_FUNC 
%token <string> ID TYPE CHAR STRING FLOAT TRUE FALSE
%token <integer> NR

%token '[' ']'


%left '+' '-'
%left '*' '/'
%nonassoc GT LT LEQ GEQ EQ NEQ AND OR
%nonassoc IF
%nonassoc ELSE

%start program_start
%%

program_start :  class_section var_section funct_section entry {
          if (errorCount == 0) 
               cout<< "The program is correct." << endl;
     }
     ;

class_section : CLASS_DECL class_section
               | /* empty */
               ;

CLASS_DECL :  CLASS class_name '{' list_decl '}' class_end { current = current->prev; }
           ;

class_name : ID {
               for(auto &item: tables){
                    if(item->name == string($1)){
                         errorCount++;
                         yyerror("Class already defined");
                    }
               }
               addTableNewClass(string($1));     
          }

list_decl : method_decl
           | variabile_decl
           | method_decl list_decl
           | variabile_decl list_decl 
           ;
          
method_decl : TYPE ID {
                         if(addTableNewMethod(string($2), "method") == -1){ 
                                   errorCount++; 
                                   yyerror("Method already declared.");
                         }
                             // std::cout << current->name << "\n";
                    }   '(' list_param ')' '{' statement_list '}' {
                          if(addParameterToMethod() != "ok"){
                              errorCount++;
                              yyerror(addParameterToMethod());
                         }
                         parameters_list.clear();
                         current = current->prev;
                    }
          /*| TYPE ID '(' list_param ')' ';' {
                    
               }*/
            ;

variabile_decl : VAR_DECL

class_end : ';' {
     
}


var_section : VAR_DECL var_section
            | /* empty */
            ;

VAR_DECL : TYPE ID ';' { 
                    //cout << "ADDING VARIABLE " << $2 << " in table " << current->name << endl;
                    if(check_and_add_variable(string($2), string($1), string($1), "null ") == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }
          }
          | TYPE ID EQUAL expression ';'{
                  if(check_and_add_variable(string($2), string($1), string($1), va) == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }  
          }
          | TYPE ID EQUAL expression_bool ';' {
                    if(check_and_add_variable(string($2), string($1), string($1), va) == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }
          }
          | TYPE ID '['  ']' ';' { 
                    if(check_and_add_variable(string($2), string($1), string($1), "null ") == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }
          }
          | TYPE ID '[' NR ']' ';' { 
                    if(check_and_add_variable(string($2), string($1), string($1), "null ") == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }
          }
          | ID ID ';' {
                    if(check_and_add_variable(string($2), string($1), string($1), "null ") == -1){
                         errorCount++;
                         yyerror("Variable " + string($2) + " already defined.");
                    }    
          }
          | ID EQUAL expression ';' {
                if(check_if_exists(string($1), "var") == false){
                         errorCount++;
                         yyerror("Variable " + string($1) + " not defined");
                    }
                    /*
                    for(auto& item : last_types){
                         if(current->ids[string($1)].type != item){
                              errorCount++;
                         yyerror("Different data types");
                         }
                    }
                    */
                    last_types.clear();
          }
          | ID '.' ID EQUAL expression ';'
          | ID '.' ID '(' call_list ')' ';'
          ;

funct_section : FUNC_DECL funct_section
              | /* empty */
              ;

FUNC_DECL : function_decl
          ;

function_decl : FUNCTION TYPE ID { 
                    for(auto& item : tables){
                         if(item->name == string($3)){
                              //std::cout << item->name << " " << item->type << "\n";
                              errorCount++;
                              yyerror("Function " + string($3) + " already declared");
                         }
               }
               addTableNewFunction(string($3), string($2)); }
               '(' list_param ')'
               {    //std::cout << parameters_list.size();
                    std::string returned = addParameterToMethod();
                if(returned != "ok"){
                    errorCount++;
                    yyerror(returned);
               } 
               }
               
                '{' statement_list '}' {
                
               parameters_list.clear();
               current = current->prev;
          }
          ;


list_param : param
            | list_param ','  param 
            ;
            
param : TYPE ID { 
               int i = addParameter(string($1), string($2));
               //std::cout << i << " " << string($1) << " " <<  string($2)<< " " << current->name << "\n";
               if(i == -1){
                    errorCount++;
                    yyerror("Incorrect data type");
               }
               
      }
      ; 
      

entry : BGIN { addNewTable("main", "main");} statement_list END {

     }
     ;
     
statement_list : statement
               | statement_list statement
          ;

if_start : IF {addNewTable("if" + to_string(if_counter++), "if_block");} '(' expression_bool ')' '{' statement_list '}' {
               current = current->prev;
          }

statement: VAR_DECL
          | ID '[' expression ']' EQUAL expression ';'
          | function_call
          | if_start
          | if_start ELSE '{' statement_list '}'{
               current = current->prev;
          }
          | FOR {addNewTable("for" + to_string(for_counter++), "for_block");}'(' assignment ';' expression_bool ';' assignment ')' '{' statement_list '}' {
               current = current->prev;
          }
          | WHILE {addNewTable("while" + to_string(while_counter++), "while_block");} '(' expression_bool ')' '{' statement_list '}' {
               current = current->prev;
          }
          | typeof_function
          | print_function
          ;

function_call : ID '(' call_list ')' ';' {
               function_parameters.clear();
          }
          | ID '(' call_list ')' {
               
          }

function_call_exp : ID {
                    int found = 0;
                    std::string return_val;
                    for(auto& item : tables){
                         if(item->name == string($1) && item->type == "function"){
                              found = 1;
                              return_val = item->return_value;
                         }
                    }
                    if(found = 0){
                         errorCount++;
                         yyerror("Function not defined");
                    }

                    for(auto& item : last_types){
                         if(return_val != item){
                              errorCount++;
                              yyerror("Different data types in function");
                         }
                    }

               } '(' call_list_exp ')' {
               
          }

expression_bool: expression LT expression
               | expression GT expression
               | expression LEQ expression
               | expression GEQ expression
               | expression EQ expression
               | expression NEQ expression
               | expression OR expression
               | expression AND expression
               | expression_bool AND expression_bool
               | expression_bool OR expression_bool
               | '(' expression_bool ')'
               | TRUE { va = "true";}
               | FALSE { va = "false";}
               ;

expression : expression '+' expression
           | expression '-' expression
           | expression '*' expression
           | expression '/' expression
           | '(' expression ')'
           | ID '.' ID /*{variabila aia e int } */  
           | ID '[' expression ']'//{variabila aia e int }
           | NR { 
               /*
               for(auto& item : last_types){
                    if(to_string($1) != item){
                         errorCount++;
                         yyerror("Different data types");
                    }
               }
               */
               va = to_string($1);
               last_types.push_back(to_string($1));

           }
           | ID {
               /*
               if(check_if_exists(string($1), "var") == -1){
                    errorCount++;
                    yyerror("Variable " + string($1) + " not defined");
               }
               */
           }
           | CHAR
           | STRING
           | FLOAT {
               /*
               for(auto& item : last_types){
                    if(string($1) != item){
                         errorCount++;
                         yyerror("Different data types");
                    }
               }
               */
               last_types.push_back(string($1));
           }
           | function_call_exp
           ;

assignment : ID EQUAL expression {
                    if(check_if_exists(string($1), "var") == false){
                         errorCount++;
                         yyerror("Variable " + string($1) + " not defined");
                    }
                    
                    for(auto& item : last_types){
                         if(current->ids[string($1)].type != item){
                              errorCount++;
                         yyerror("Different data types");
                         }
                    }
                   
                    last_types.clear();

          }
           | ID EQUAL expression_bool {
               if(check_if_exists(string($1), "var") == false){
                         errorCount++;
                         yyerror("Variable " + string($1) + " notexpression OR expression defined");
               }
               
               if(check_type(string($1), "bool") == -1){
                    errorCount++;
                    yyerror("Different data types");
               }
               
               last_types.clear();
           }
           | ID '[' expression ']' EQUAL expression
           | ID '.' ID EQUAL expression 
           | ID
           ;

function_expression : function_expression '+' function_expression
           | function_expression '-' function_expression
           | function_expression '*' function_expression
           | function_expression '/' function_expression
           | '(' function_expression ')'
           | ID '.' ID
           | ID '[' function_expression ']'
           | NR {
               function_parameters.push_back(to_string($1));
           }
           | ID {
               if(check_if_exists(string($1), "var") == false){
                         errorCount++;
                         yyerror("Variable " + string($1) + " not defined");
               }
               
               if(check_type(string($1), "int") == -1){
                    errorCount++;
                    yyerror("Different data types");
               }
               
               function_parameters.push_back("int");

           }
           | CHAR {
               function_parameters.push_back(string($1));
           }
           | STRING {
               function_parameters.push_back(string($1));
           }
           | FLOAT {
               function_parameters.push_back(string($1));
           }
           ;

call_list : function_expression
          | call_list ',' function_expression
          | call_list ',' function_call
          ;

call_list_exp : function_expression
          | call_list_exp ',' function_expression
          | call_list_exp ',' function_call_exp
          ;

typeof_function : TO '(' ID ')' ';'
               ;

print_function : PRINT '(' expression ')' ';'
               | PRINT '(' expression_bool ';'{
                    std::cout << "\n" << boolean << "\n";
               }
               ;
             
%%
void yyerror(std::string s){
     cout << "error:" << s << " at line: " << yylineno << "\n";
     
}

int main(int argc, char** argv)
{
     yyin=fopen(argv[1],"r");
     global = new SymTable("global", "global");
     tables.push_back(global);
     global->prev = nullptr;
     current = global;
     yyparse();
     //showAllTables();
     showAll();
}
