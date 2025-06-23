#include "Rastrigin.h"

Rastrigin::Rastrigin(double a, double b, long long int dimensions, long long int precision, long long int popSize) {
    this->a = a;
    this->b = b;
    this->dimensions = dimensions;
    this->precision = precision;
    this->n = ceil(log2(((b - a) * pow(10, this->precision))));
    this->bitStringLength = this->dimensions * this->n;
    this->max = (1 << this->n) - 1;
    this->populationSize = popSize;
    //this->Population.reserve(popSize);
    this->generationsCounter = 0;
    cout << "Rastrigin Function\n";
    cout << "Function Domain: [" << this->a << ", " << this->b << "]\n";
    cout << "Number of parameters: " << this->dimensions << "\n";
    cout << "Number of repetitions: " << this->steps << "\n";
    cout << "Precision: " << this->precision << "\n";
    cout << "Population size: " << this->populationSize << "\n";
}

double Rastrigin::calculateFunction(const vector<double> &parameters) {
    double result = 10 * this->dimensions;
    for (long long int i = 0; i < this->dimensions ; i++) {
        result += (pow(parameters[i], 2) - 10 * cos(2 * PI * parameters[i]));
    }
    return result;
}
