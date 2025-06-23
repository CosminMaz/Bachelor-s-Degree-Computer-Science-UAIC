#include "Function.h"
#pragma once

class Michalewicz: public Function {
public:
	Michalewicz(double a, double b, long long int dimensions, long long int precision);
	virtual double calculateFunction(const vector<double> &parameters);
};
