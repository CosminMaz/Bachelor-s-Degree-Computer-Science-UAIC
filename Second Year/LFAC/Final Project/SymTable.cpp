#include "SymTable.h"
using namespace std;

void SymTable::addVar(string type, string name, string value){
    IdInfo var(type, name, "var", value, 1);
    ids[name] = var; 
}

/*
void SymTable::addClass(const char* name){
    clase[name] = ClassInfo();
}

void SymTable::addMethod(const char* className, const char* returnType, const char* methodName){
    clase[className].addMethod(returnType, methodName);
}
void SymTable::addField(const char* className, const char* fieldType, const char* fieldName){
    clase[className].addCamp(fieldType, fieldName);
}
*/

/*
bool SymTable::existsCamp(const char* className, const char* fieldName){
    return clase[className].hasCamp(fieldName);
}

bool SymTable::existsMethod(const char* objectName, const char* methodName){
    string className = ids[objectName].type; 
    return clase[className].hasMethod(methodName);
}
*/
/**/
/*
void SymTable::addFunction(string returnType, string functionName){
    IdInfo fncvar("function", functionName, "function");
    ids[functionName] = fncvar;
}

void SymTable::addArray(string type, string arrayName, int size){
    IdInfo arr("array", arrayName, "array", size);
    ids[arrayName] = arr;
}
*/

/*
bool SymTable::existsIdInClass(const char* className, const char* methodName){
    return clase[className].hasMethod(methodName);
}
*/

bool SymTable::existsId(string var) {
    return ids.find(var)!= ids.end();  
}

/*
bool SymTable::existsClass(const char* cls){
    return clase.find(cls)!= clase.end();
}
*/

void SymTable::printVars() {
    for(const pair<string, IdInfo>& v : ids){
        cout << v.second.type << " " << v.first << endl; 
     }
}

/*
void SymTable::printClasses(){
    for(const auto& cls : clase){
        cout<< "Class: " <<cls.first << endl;
        const ClassInfo classInfo = cls.second;

        cout<<" Fields:"<<endl;
        for(const auto& field : classInfo.fields){
            cout << "   " << field.second.type << " " << field.first << endl;
        }

        cout<<" Methods:"<<endl;
        for(const auto& method : classInfo.methods){
            cout << "   " << method.second.type << " " << method.first << endl;
        }
    }
}
*/


SymTable::~SymTable() {
    ids.clear();
}