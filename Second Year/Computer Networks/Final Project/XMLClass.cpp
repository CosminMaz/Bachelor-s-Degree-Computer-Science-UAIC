#include "XMLClass.h"


XMLClass::XMLClass(char xml_file[]){
    try {
        std::ifstream xmlFile("trains.xml");
        std::vector<char> buffer((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());
        buffer.push_back('\0');
        doc.parse<0>(&buffer[0]);

        root = doc.first_node("Trains");
        if(root == nullptr){
            throw std::runtime_error("[Server]Root node <Trains> not found.");   
        }

    } catch(const rapidxml::parse_error &e){
        std::cerr << "[Server]Parsing error: " << e.what() << "\n";
        std::cerr << "[Server]Error location: " << e.where<char>() << "\n";
    } catch(const std::exception &e){
        std::cerr << "[Server]Error: " << e.what() << "\n";
    } catch(...){
        std::cerr << "[Server]An unknown error occured.\n";
    } 
}

char* XMLClass::GetTodayTrains(){
    static char result[10000];
    result[0] = '\0';  
    strcat(result, "\n");
    for(rapidxml::xml_node<>* train = this->root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* train_id = train->first_attribute("id")->value();
        const char* owner = train->first_attribute("owner")->value();

        
        rapidxml::xml_node<>* first_station = train->first_node("route")->first_node("routeElement");
        if (!first_station) continue;

        const char* first_station_name = first_station->first_attribute("departure")->value();
        const char* first_station_arrival_hour = first_station->first_attribute("arrivalHour")->value();
        const char* first_station_arrival_min = first_station->first_attribute("arrivalMin")->value();

        rapidxml::xml_node<>* last_station = train->first_node("route")->last_node("routeElement");
        if (!last_station) continue;

        const char* last_station_name = last_station->first_attribute("destination")->value();
        const char* last_station_arrival_hour = last_station->first_attribute("arrivalHour")->value();
        const char* last_station_arrival_min = last_station->first_attribute("arrivalMin")->value();
        
        strcat(result, "Train ID: ");
        strcat(result, train_id);
        strcat(result, ", Owner: ");
        strcat(result, owner);
        strcat(result, ", First Station: ");
        strcat(result, first_station_name);
        strcat(result, ", First Arrival Time: ");
        strcat(result, first_station_arrival_hour);
        strcat(result, ":");
        strcat(result, first_station_arrival_min);
        strcat(result, ", Last Station: ");
        strcat(result, last_station_name);
        strcat(result, ", Last Arrival Time: ");
        strcat(result, last_station_arrival_hour);
        strcat(result, ":");
        strcat(result, last_station_arrival_min);
        strcat(result, "\n");
    }

    if (result[0] == '\0') {
        strcpy(result, "\nNo trains available.\n");
    }
    return result;
}

char* XMLClass::GetThisHourDepartures(int hour){
    static char result[10000]; 
    result[0] = '\0';         
    strcat(result, "\n");
    bool train_found = false;
    for(rapidxml::xml_node<>* train = root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* train_id = train->first_attribute("id")->value();
        const char* owner = train->first_attribute("owner")->value();

        for(rapidxml::xml_node<>* routeElement = train->first_node("route")->first_node("routeElement"); 
             routeElement != nullptr; 
             routeElement = routeElement->next_sibling("routeElement")) {

            int arrivalHour = atoi(routeElement->first_attribute("departureHour")->value());
            if (arrivalHour == hour) {
                const char* destination = routeElement->first_attribute("departure")->value();
                const char* arrivalMin = routeElement->first_attribute("arrivalMin")->value();
                const char* delay = routeElement->first_attribute("delayMin")->value();
                const char* isDelay = routeElement->first_attribute("delay")->value();
                strcat(result, "Train ID: ");
                strcat(result, train_id);
                strcat(result, ", Owner: ");
                strcat(result, owner);
                strcat(result, ", Departure Station: ");
                strcat(result, destination);
                strcat(result, ", Departure Time: ");

            
                char time[6]; 
                time[0] = '0' + (arrivalHour / 10); 
                time[1] = '0' + (arrivalHour % 10); 
                time[2] = ':';                      
                time[3] = arrivalMin[0];          
                time[4] = arrivalMin[1];           
                time[5] = '\0';       

                strcat(result, time); 
                if(strcmp(isDelay, "true") == 0){
                    strcat(result, ", Delay: ");
                } else {
                    strcat(result, ", Early: ");
                }

                strcat(result, delay);
                strcat(result, " minutes");
                strcat(result, "\n");
                train_found = true;
            }
        }
    }

    // Check if no trains were found
    if (train_found == false) {
        strcpy(result, "\nNo trains departure at this hour.\n");
    }

    return result;
}

char* XMLClass::GetThisHourArrivals(int hour){
    static char result[1000]; 
    result[0] = '\0';         
    strcat(result, "\n");
    bool train_found = false;

    for(rapidxml::xml_node<>* train = root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* train_id = train->first_attribute("id")->value();
        const char* owner = train->first_attribute("owner")->value();

        for (rapidxml::xml_node<>* routeElement = train->first_node("route")->first_node("routeElement"); 
             routeElement != nullptr; 
             routeElement = routeElement->next_sibling("routeElement")) {

            int arrivalHour = atoi(routeElement->first_attribute("arrivalHour")->value());
            if (arrivalHour == hour) {
                const char* destination = routeElement->first_attribute("destination")->value();
                const char* arrivalMin = routeElement->first_attribute("arrivalMin")->value();
                const char* delay = routeElement->first_attribute("delayMin")->value();
                const char* isDelay = routeElement->first_attribute("delay")->value();
                
                strcat(result, "Train ID: ");
                strcat(result, train_id);
                strcat(result, ", Owner: ");
                strcat(result, owner);
                strcat(result, ", Arrival Station: ");
                strcat(result, destination);
                strcat(result, ", Arrival Time: ");

                
                char time[6]; 
                time[0] = '0' + (arrivalHour / 10); 
                time[1] = '0' + (arrivalHour % 10); 
                time[2] = ':';                   
                time[3] = arrivalMin[0];           
                time[4] = arrivalMin[1];          
                time[5] = '\0';                    

                strcat(result, time); 
                if(strcmp(isDelay, "true") == 0){
                    strcat(result, ", Delay: ");
                } else {
                    strcat(result, ", Early: ");
                }

                strcat(result, delay);
                strcat(result, " minutes");
                strcat(result, "\n");
                train_found = true;
            }
        }
    }

    if (train_found == false) {
        strcpy(result, "\nNo trains arriving at this hour.\n");
    }

    return result;
}


char* XMLClass::showInfo(char train_id[100]){
    char* result = new char[4096]; 
    result[0] = '\0'; 
    if(strcmp(train_id, "notdefined") != 0){

    
    bool train_found = false;
    for(rapidxml::xml_node<>* train = root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* current_train_id = train->first_attribute("id")->value();
        
        
        if (strcmp(current_train_id, train_id) == 0) {
            const char* owner = train->first_attribute("owner")->value();
        
            strcat(result, "Train ID: ");
            strcat(result, current_train_id);
            strcat(result, "\nOwner: ");
            strcat(result, owner);
            strcat(result, "\n");

            
            rapidxml::xml_node<>* route = train->first_node("route");
            if (!route) continue;

            for (rapidxml::xml_node<>* routeElement = route->first_node("routeElement"); routeElement != nullptr; routeElement = routeElement->next_sibling("routeElement")) {
                const char* departure = routeElement->first_attribute("departure")->value();
                const char* destination = routeElement->first_attribute("destination")->value();
                const char* arrival_hour = routeElement->first_attribute("arrivalHour")->value();
                const char* arrival_min = routeElement->first_attribute("arrivalMin")->value();
                const char* delay = routeElement->first_attribute("delayMin") ? routeElement->first_attribute("delayMin")->value() : "0";
                const char* isDelay = routeElement->first_attribute("delay")->value();
                strcat(result, "Departure: ");
                strcat(result, departure);
                strcat(result, ", Destination: ");
                strcat(result, destination);
                strcat(result, ", Arrival Time: ");
                strcat(result, arrival_hour);
                strcat(result, ":");
                strcat(result, arrival_min);
                if(strcmp(isDelay, "true") == 0){
                    strcat(result, ", Delay: ");
                } else {
                    strcat(result, ", Early: ");
                }
                strcat(result, delay);
                strcat(result, " minutes\n");
            }

            train_found = true;
            break;
        }
    }

    if (!train_found) {
        strcpy(result, "Train not found.");
    }
    } else {
        strcat(result, "\nCommand use : showInfo train_id");
    }

    return result;
}

char* XMLClass::addDelay(char train_id[100], char delay_in_minutes[100], char station[100]){
    static char result[2048];                                  
    result[0] = '\0';         

    if((strcmp(train_id, "notdefined") == 0) || (strcmp(delay_in_minutes, "delaynotdefined") == 0) || (strcmp(station, "stationnotdefined") == 0)){
        strcat(result, "\nCommand use : addDelay train_id delay_in_minutes station\n");
    } else {
    int delay_to_add = atoi(delay_in_minutes);

    for(rapidxml::xml_node<>* train = root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* current_train_id = train->first_attribute("id")->value();

        if(strcmp(current_train_id, train_id) == 0) {
            
            for (rapidxml::xml_node<>* routeElement = train->first_node("route")->first_node("routeElement"); 
                 routeElement != nullptr; 
                 routeElement = routeElement->next_sibling("routeElement")) {

                const char* current_station = routeElement->first_attribute("destination")->value();

                if(strcmp(current_station, station) == 0) {
                    int current_delay_min = atoi(routeElement->first_attribute("delayMin")->value());
                    const char* delay_status = routeElement->first_attribute("delay")->value();
                    bool is_delayed = (strcmp(delay_status, "true") == 0);

                    if (is_delayed) {
                        current_delay_min += delay_to_add; 
                    } else {
                        current_delay_min -= delay_to_add; 
                        if (current_delay_min < 0) {
                            current_delay_min = -current_delay_min; 
                            routeElement->first_attribute("delay")->value("true");
                        }
                    }

                    
                    char updated_delay_min[10];
                    snprintf(updated_delay_min, sizeof(updated_delay_min), "%d", current_delay_min);

                    routeElement->first_attribute("delayMin")->value(routeElement->document()->allocate_string(updated_delay_min));
                    if (current_delay_min >= 0) {
                        const char* delay_value = (current_delay_min > 0) ? "true" : "false";
                        routeElement->first_attribute("delay")->value(routeElement->document()->allocate_string(delay_value));
                    }

                    strcpy(result, "Delay successfully updated");
                    return result;
                }
            }
            snprintf(result, sizeof(result), "Station '%s' not found for train ID '%s'", station, train_id);
            return result;
        }
    }
    }
    snprintf(result, sizeof(result), "Train ID '%s' not found", train_id);
    return result;
}

char* XMLClass::addEarly(char train_id[100], char delay_in_minutes[100], char station[100]){
    static char result[2048]; 
    result[0] = '\0';         

    if((strcmp(train_id, "notdefined\0") == 0) || (strcmp(delay_in_minutes, "delaynotdefined") == 0) || (strcmp(station, "stationnotdefined") == 0)){
        strcat(result, "\nCommand use : addDelay train_id delay_in_minutes station\n");
    } else {
    int delay_to_add = atoi(delay_in_minutes);

    for(rapidxml::xml_node<>* train = root->first_node("train"); train != nullptr; train = train->next_sibling("train")) {
        const char* current_train_id = train->first_attribute("id")->value();

        if(strcmp(current_train_id, train_id) == 0) {
            for(rapidxml::xml_node<>* routeElement = train->first_node("route")->first_node("routeElement"); 
                 routeElement != nullptr; 
                 routeElement = routeElement->next_sibling("routeElement")) {

                const char* current_station = routeElement->first_attribute("destination")->value();

                if(strcmp(current_station, station) == 0) {
                    int current_delay_min = atoi(routeElement->first_attribute("delayMin")->value());
                    const char* delay_status = routeElement->first_attribute("delay")->value();
                    bool is_delayed = (strcmp(delay_status, "true") == 0);

                    if (delay_to_add > 0) {
                        if (!is_delayed) {
                            current_delay_min += delay_to_add;
                        } else {
                            current_delay_min -= delay_to_add;
                            if(current_delay_min < 0) {
                                current_delay_min = -current_delay_min;
                                routeElement->first_attribute("delay")->value("false");
                            } else {
                                routeElement->first_attribute("delay")->value("false");
                            }
                        }
                    } else if(delay_to_add < 0) {
                        if (is_delayed) {
                            current_delay_min += delay_to_add;
                            if (current_delay_min < 0) {
                                current_delay_min = -current_delay_min;
                                routeElement->first_attribute("delay")->value("false");
                            }
                        } else {
                            current_delay_min -= delay_to_add;
                        }
                    }

                    char updated_delay_min[10];
                    snprintf(updated_delay_min, sizeof(updated_delay_min), "%d", current_delay_min);
                    routeElement->first_attribute("delayMin")->value(routeElement->document()->allocate_string(updated_delay_min));

                    if(current_delay_min == 0) {
                        routeElement->first_attribute("delay")->value("false");
                    }

                    strcpy(result, "Delay successfully updated");
                    return result;
                }
            }

            snprintf(result, sizeof(result), "Station '%s' not found for train ID '%s'", station, train_id);
            return result;
        }
    }
    }
    snprintf(result, sizeof(result), "Train ID '%s' not found", train_id);
    return result;
}