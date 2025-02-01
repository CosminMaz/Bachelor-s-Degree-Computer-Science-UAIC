#include "Michalewicz.h"

Michalewicz::Michalewicz(double a, double b, long long int dimensions, long long int precision, long long int popSize){
	this->a = a;
	this->b = b;
	this->dimensions = dimensions;
	this->precision = precision;
	this->n = ceil(log2(((b - a) * pow(10, this->precision))));
	this->bitStringLength = this->dimensions * this->n;
	this->max = (1 << this->n) - 1;
	this->populationSize = popSize;
	this->generationsCounter = 0;
	cout << "Michalewicz Function\n";
	cout << "Function Domain: [" << this->a << ", " << this->b << "]\n";
	cout << "Number of parameters: " << this->dimensions << "\n";
	cout << "Number of repetitions: " << this->steps << "\n";
	cout << "Precision: " << this->precision << "\n";
}

double Michalewicz::calculateFunction(const vector<double> &parameters){
	double result = 0;
	for(long long int i = 1; i <= this->dimensions; i++)
		result += (-sin(parameters[i]) * pow(sin(i * pow(parameters[i],2)/PI), 20));

	return result;
}
