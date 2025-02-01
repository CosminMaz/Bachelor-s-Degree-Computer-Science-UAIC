#include "Globals.h"
#include "SymTable.h"

std::vector<std::pair<std::string, std::pair<std::string, std::string>>> *variable_ide = 
    new std::vector<std::pair<std::string, std::pair<std::string, std::string>>>;

std::string variable_type;
std::string last;
std::vector<pair<std::string, std::string>> parameters_list;
std::vector<std::string> last_types;
std::string va;
std::vector<std::string> function_parameters;

SymTable* current = nullptr;
SymTable* global = nullptr;
SymTable* temp = nullptr;

std::vector<SymTable*> tables;
std::string boolean;
int if_counter = 0, while_counter = 0, for_counter = 0;

void addNewTable(std::string table_name, std::string table_type){
    temp = new SymTable(table_name, table_type);
    tables.push_back(temp);
    temp->prev = current;
    
    current = temp;
}

/*
temp->prev = current;
cuurent = temp;
*/

int check_type(std::string id, std::string type){
    temp = current;
    while(temp != nullptr){
        if(temp->existsId(id)){
            if(temp->ids[id].idType != type)
                return -1;
        } else {
            temp = temp->prev;
        }
    }
    return 1;
}

bool check_if_exists(std::string name, std::string type){
    temp = current;
    
    if(type == "var"){
        while(temp != nullptr){
            if(temp->existsId(name))
                if(temp->ids[name].idType == type || temp->ids[name].idType == "param")
                    return true;
            temp = temp->prev;
        }
    }
    return false;
}

void addTableNewClass(string class_name){
    temp = new SymTable(class_name, "class");
    tables.push_back(temp);
    temp->prev = global;
    temp->type = "class";

    current = temp;
}

void addTableNewFunction(string function_name, std::string return_val){
    temp = new SymTable(function_name, "function");
    tables.push_back(temp);
    temp->prev = global;
    temp->type = "function";
    temp->return_value = return_val;
    current = temp;
}

int check_and_add_variable(/* SymTable* correct, */std::string id, std::string type, std::string expected_type, std::string value){
    //std::cout << type << " " << expected_type << "\n";
    temp = current;

    if(temp->existsId(id)) return -1;

    /* correct = temp; */
    
    IdInfo a(type, id, "var", value, 1);
    current->ids[id] = a;
    return 1;
}

/*Symtable *correct; check_and_add_variable(correct, .. ) correct*/

int check_and_add_parameter(std::string id, std::string type, std::string expected_type){
    if(current->existsId(id)){
        if(current->ids[id].idType == "param"){
            return -1; //paramters already declared;
        }
    } 

    IdInfo a(type, id, "param");
    current->ids[id] = a;
    return 1;
}

int addTableNewMethod(std::string method_name, std::string type){
    temp = current;
        if(temp->existsId(method_name)){
            if(temp->ids[method_name].idType == "method"){
                return -1;
            }
        }

    SymTable* new_table = new SymTable(method_name, type);
    new_table->prev = current;
    current->methods.push_back(new_table);
    current = new_table;
    return 1;
}

int addParameter(std::string param_type , std::string name){
    int found = 0;

    if(param_type == "int" || param_type == "char" || param_type == "float" || param_type == "string" || param_type == "bool") {
        found = 1;
    }

    for(auto&item : tables){
        if(param_type == item->name && item->type == "class")
            found = 1;
    }

    
    if(found){
        parameters_list.push_back({param_type, name});
        return 1;
    } else {
        return -1;
    }
}

std::string addParameterToMethod(){
    //std::cout << "AAAA" << parameters_list.size() << endl;
    for(auto& item : parameters_list){
       // std::cout << "ADDING PARAM: " << item.second << endl;
        if(current->existsId(item.second)){
            return "Paramter " + item.second + " its already declared";
        } else {
            IdInfo a(item.first, item.second, "param");
            current->ids[item.second] = a;
        }
    }
    return "ok";
}

void showAllTables() {
    std::ofstream output_file;
    output_file.open("output.txt");

    if(!output_file){
        std::cerr << "Error at opening file";
        return;
    }

    //std::cout << "Global variables: \n";
    for (auto &item : tables) {
        if (!item) continue; // Ensure item is not null
        //std::cout << "\n";
        if (item->type == "global") {
            //std::cout << "Global variables: \n";
            output_file << "Global variables: \n";
            for (auto &object : item->ids) {
                if (object.second.idType == "var") {
                    // Ensure value[1] exists
                    if (object.second.value.size() >= 1) {
                        //std::cout << object.second.type << " " 
                                  //<< object.second.name << " = " 
                                  //<< object.second.value[1] << "\n";

                        output_file << object.second.type << " " 
                                    << object.second.name << " = " 
                                    << object.second.value[1] << "\n";
                    } else {
                        //std::cout << object.second.type << " " << object.second.name << "\n";
                        output_file << object.second.type << " " << object.second.name << "\n";
                    }
                }
            }
        }
    }
    //std::cout << "Classes:";
    output_file << "Classes:";
    for (auto &item : tables) {
        //std::cout << "\n";
        if (item->type == "class") {
           // std::cout << "  Class " << item->name << ":\n";
            output_file << "  Class " << item->name << " - parent -> ";
            output_file << item->prev->name << "\n";
            for (auto &object : item->ids) {
                if (object.second.idType == "var") {
                    // Ensure value[1] exists
                    if (object.second.value.size() > 1) {
                       // std::cout << "      "
                                  //<< object.second.type << " " 
                                 // << object.second.name << " = " 
                                 // << object.second.value_var << "\n";   
                        
                        output_file << "      "
                                    << object.second.type << " " 
                                    << object.second.name << " = " 
                                    << object.second.value_var << "\n"; 
                    } else {
                        //std::cout << "      " << object.second.type << " " << object.second.name << "\n";
                        output_file << "      " << object.second.type << " " << object.second.name << "\n";
                    }
                }
            }

           // std::cout << "      Method members: \n";
            output_file  << "      Method members: \n";
            for (auto &object : item->methods) {
                if (object) { // Ensure object is not null
                   // std::cout << "          " << object->name << "(";
                    output_file << "          " << object->name << "(";
                    int first = 1;
                    for(auto& param : object->ids){
                        if(first == 0){
                            //std::cout << ", ";
                            output_file << ", ";
                        }
                        if(param.second.idType == "param"){
                           // std::cout << param.second.type << " " << param.second.name;
                            output_file << param.second.type << " " << param.second.name;
                            first = 0;
                        }
                    }
                   // std::cout << "):\n";
                    output_file << "):\n";
                }
                if(object){
                    for(auto& param : object->ids){
                        if(param.second.idType == "var"){
                           // std::cout << "              "<< param.second.type << " " << param.second.name << "\n";
                            output_file << "              "<< param.second.type << " " << param.second.name << "\n";
                        }
                    }

                }
            }
        }
    }

   // std::cout << "Functions: " << "\n";
    output_file << "Functions: " << "\n";
    for(auto& item: tables){
        if(item->type == "function"){
            //std::cout << "  "<< item->name << "(";
            output_file  << "  "<< item->name << "(";
            int first = 1;
            for(auto& param : item->ids){
                if(first == 0){
                   // std::cout << ", ";
                    output_file << ", ";
                }
                    if(param.second.idType == "param"){
                       // std::cout << param.second.type << " " << param.second.name;
                        output_file << param.second.type << " " << param.second.name;
                        first = 0;
                    }
            }
           // std::cout << "\b\b)\n";
            output_file << "\b\b)\n";
        }    
    }

  //  std::cout << "Main: \n";
    output_file << "Main: \n";
    for(auto& item: tables) {
        if(item->type == "main")
        std::cout << item->prev->name << "\n";
        for(auto& var : item->ids){
           // std::cout << "  " << var.second.type << " " << var.second.name << "\n";
            output_file << "  " << var.second.type << " " << var.second.name << var.second.value_var << "\n";
        }
    }

    std::cout << "\n";
    for(auto& item: tables){
        if(item->prev != nullptr)
        std::cout << item->name << " --parent--> " << item->prev->name << "\n";
    }

}


void showAll(){
    std::ofstream output_file;
    output_file.open("output.txt");

    if(!output_file){
        std::cerr << "Error at opening file";
        return;
    }


    for(auto&item: tables){
        if(item != nullptr){
            if(item->prev !=nullptr){
                output_file << item->type << " " << item->name << "- parent ->" << item->prev->name << "\n";
            } else {
                output_file << item->type << " " << item->name << "\n";
            }
            for(auto& obj : item->ids){
                output_file << obj.second.idType << " " << obj.second.type << " " << obj.second.name;
                if(obj.second.idType == "var"){
                    output_file << " = "<< obj.second.value_var << "\n";
                } else {
                    output_file << "\n";
                }
            }
            if(item->type == "class"){
                for(auto &m : item->methods){
                    if(m != nullptr)
                    output_file << m->type << " " << m->name << " - parent -> " << m->prev->name << "\n";
                    for(auto& v : m->ids){
                        output_file << v.second.idType << " " << v.second.type << " " << v.second.name;
                        if(v.second.idType == "var"){
                            output_file << " = "<< v.second.value_var << "\n";
                        } else {
                            output_file << "\n";
                }
                    }
                }
            }
            output_file << "\n\n\n";
        }
    }

}