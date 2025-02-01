#include "Function.h"
#pragma once

class Rastrigin: public Function {
public:
	Rastrigin(double a, double b, long long int dimensions, long long int precision, long long int popSize);
   	virtual double calculateFunction(const vector<double> &parameters);
};
