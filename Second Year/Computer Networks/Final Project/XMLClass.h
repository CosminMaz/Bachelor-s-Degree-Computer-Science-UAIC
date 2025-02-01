#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <iostream>
#include <errno.h>
#include <string.h>



class XMLClass{
private:
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root;
public:
    XMLClass(char xml_file[]);
    char* GetThisHourDepartures(int hour);
    char* GetThisHourArrivals(int hour);
    char* GetTodayTrains();
    char* addDelay(char train_id[100], char delay_in_minutes[100], char station[100]);
    char* addEarly(char train_id[100], char delay_in_minutes[100], char station[100]);
    char* showInfo(char train_id[100]);
};