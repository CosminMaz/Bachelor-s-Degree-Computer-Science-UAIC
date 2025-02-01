#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H
#include "SymTable.h"
#include <vector>
#include <string>
#include <fstream>

                        /*  type                  name       value*/
extern std::vector<std::pair<std::string, std::pair<std::string, std::string>>> *variable_ide;

extern std::string variable_type;
extern std::string last;
                            /*TYPE        ID*/
extern std::vector<pair<std::string, std::string>> parameters_list; 
extern std::vector<std::string> last_types;
extern std::vector<std::string> function_parameters;
extern SymTable* current;
extern SymTable* global;
extern SymTable* temp;
extern std::string va;
extern std::string boolean;

extern int if_counter, while_counter, for_counter;

extern std::vector<SymTable*> tables;

void showAll();
int check_type(std::string id, std::string type);
bool check_if_exists(std::string name, std::string type);
void showAllTables();
void addTableNewClass(string class_name);
void addTableNewFunction(string function_name, std::string type);
void addNewTable(std::string table_name, std::string table_type);
int addParameter(std::string type , std::string name);
std::string addParameterToMethod();
int addTableNewMethod(std::string method_name, std::string type);
int check_and_add_parameter(std::string id, std::string type, std::string expected_type);
int check_and_add_variable(std::string id, std::string type, std::string expected_type, std::string value);
int check_and_add_method(std::string id, std::string type);

#endif // GLOBAL_FUNCTIONS_H