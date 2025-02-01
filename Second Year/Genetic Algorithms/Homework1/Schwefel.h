#include "Function.h"
#pragma once

class Schwefel: public Function{
public:
	Schwefel(double a, double b, long long int dimensions, long long int precision);
	virtual double calculateFunction(const vector<double> &parameters);
};
