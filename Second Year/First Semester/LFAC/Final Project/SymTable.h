#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class ParamList { };

class IdInfo {
    public:
    string idType; //var, class, function
    string type; //int chr, bool
    string name;
    std::vector<std::string>value;
    int size; // for var = 1; array.lenght
    string value_var;
    IdInfo() {}
    IdInfo(std::string type, std::string name, std::string idType, int size = 1)
      : type(type), name(name), idType(idType), size(size) {}
    IdInfo(std::string type, std::string name, std::string idType, std::string value, int size = 1)
      : type(type), name(name), idType(idType), size(size), value_var(value) {}
};

class SymTable {
    public:
    map<string, IdInfo> ids;
    std::string name;
    std::string type;
    std::string return_value;
    SymTable* prev;
    std::vector<SymTable*>methods; //For classes
    SymTable(std::string name, std::string type) :  name(name), type(type){}
    bool existsId(string s);
    void addVar(string type, string name, string value);
    void addClass(const char* name);
    void addMethod(const char* className, const char* returnType, const char* methodName);
    void addField(const char* className, const char* fieldType, const char* fieldName);
    void addFunction(string returnType, string functionName);
    void addArray(string type, string arrayName, int size);
    bool existsIdInClass(const char* className, const char* methodName);
    

    bool existsCamp(const char* objectName, const char* fieldName);
    bool existsMethod(const char* objectName, const char* methodName);
    bool existsClass(const char* cls);

    void printVars();
    void printClasses();
    ~SymTable();
};





