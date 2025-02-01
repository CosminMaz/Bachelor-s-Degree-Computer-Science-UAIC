#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>

class Command {
private:
    int clientDescriptor;
    int flag;
    char trainID[100];
    int currentHour;
    char delay_in_minutes[100];
    char station[100];
public:
    Command(char comm[100], int file_descriptor);
    void showInfo(char* (&word));
    void thisHourTrains();
    void addDelay(char* (&word));
    void addEarly(char* (&word));
    int GetFlag();
    int GetClient();
    int GetCurrentHour();
    char* GetTrainId();
    char* GetDelay();
    char* GetStation();
};