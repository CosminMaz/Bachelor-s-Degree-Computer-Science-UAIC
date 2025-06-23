#include "Command.h"

Command::Command(char comm[100], int file_descriptor){
    this->clientDescriptor = file_descriptor;
    char *word = strtok(comm, " \n");

    if(word == nullptr){
        this->flag = 0;
    } else if(strcmp(word, "todayTrains") == 0){
        this->flag = 1;
    } else if(strcmp(word, "thisHourDepartures") == 0){
        this->flag = 2;
        thisHourTrains();
    } else if(strcmp(word, "thisHourArrivals") == 0){
        this->flag = 3;
        thisHourTrains();
    } else if(strcmp(word, "showInfo") == 0){
        this->flag = 4;
        showInfo(word);
    } else if(strcmp(word, "addDelay") == 0){
        this->flag = 5;
        addDelay(word);
    } else if(strcmp(word, "addEarly") == 0){
        this->flag = 6;
        addEarly(word);
    } else if(strcmp(word, "exit") == 0){
        this->flag = 7;
    } else
        this->flag = 0;
}

/*
todaysTrains -> all trains for today (flag 1)
thisHourTrains -> all train for current hour (flag 2)
showInfo id_train -> show all the informations about a train (flag 3)
addDelay id_train delay_in_minutes station -> add a delay for a train for a station and update delay
                                              for next stations (flag 5)
addEarly
*/

void Command::thisHourTrains(){
    time_t now;
    struct tm *now_tm;
    int hour;
    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;
    this->currentHour = hour;
}

void Command::showInfo(char* (&word)){
    word = strtok(NULL, " \n");
    if(word != nullptr)
        strcpy(this->trainID, word);
    else {
        strcpy(this->trainID, "notdefined");
    }
}

void Command::addDelay(char* (&word)){
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->trainID, word);
    } else {
        strcpy(this->trainID, "notdefined");
    }
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->delay_in_minutes, word);
    } else {
        strcpy(this->delay_in_minutes, "delaynotdefined");
    }
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->station, word);
    } else {
        strcpy(this->station, "stationnotdefined");
    }
}

void Command::addEarly(char* (&word)){
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->trainID, word);
    } else {
        strcpy(this->trainID, "notdefined");
    }
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->delay_in_minutes, word);
    } else {
        strcpy(this->delay_in_minutes, "delaynotdefined");
    }
    word = strtok(NULL, " \n");
    if(word != nullptr){
        strcpy(this->station, word);
    } else {
        strcpy(this->station, "stationnotdefined");
    }
}

int Command::GetFlag(){
    return this->flag;
}

int Command::GetClient(){
    return this->clientDescriptor;
}

int Command::GetCurrentHour(){
    return this->currentHour;
}

char* Command::GetTrainId(){
    return this->trainID;
}

char* Command::GetStation() {
    return this->station;
}

char* Command::GetDelay() {
    return this->delay_in_minutes;
}