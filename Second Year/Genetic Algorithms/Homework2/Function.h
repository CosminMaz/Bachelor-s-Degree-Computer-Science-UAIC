#pragma once
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include <utility>
#include <random>
#include <stdio.h>
#include <algorithm>
#include <limits.h>
using namespace std;

#define PI 3.14159265358979323846
#define E 2.71828182845904523536

class Function {
protected:
    long long int dimensions;
    long long int precision;
    long long int steps = 1000;
    double a;
    double b;
    double max;
    long long int n; //bitsNeeded for one parameter
    long long int bitStringLength;
    mt19937_64 generator;
    double mutationRate = 0.5;
    double crossoverRate = 0.01;

    struct chromosome {
        vector<bool>chrom;
        double fitness;
        double selectionProbability;
    };

    long long int generationsCounter;
    long long int populationSize;
    vector<chromosome> Population;

public:
    virtual double calculateFunction(const vector<double>& parameters) = 0;
    void initializeRandom(long long int rep);
    void generateRandomBitString(vector<bool>& bitString);
    double randomSubunitary();
    double evaluate(const vector<bool>& sol);

    void hillClimbing_firstImprovement();
    void hillClimbing_bestImprovement(const vector<bool> &bestC);
    void hillClimbing_worstImprovement();
    void simulatedAnnealing(); //hybridized with Hill Climbing First Improvement

    double evaluateFitness(const vector<bool>& bitString);
    void evaluatePopulation();
    void initializePopulation(); //Initialize population and calculate fitness
    void crossover(vector<Function::chromosome>& selectedParents);
    void mutate(vector<chromosome>& selectedParents);
    void selectPopulation(vector<chromosome>& pop);
    vector<double> decodeChromosome(const std::vector<bool>& bitString);
    vector<chromosome> elitism();
    void geneticAlgorithm();
};