#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include <utility>
#include <random>
#include <stdio.h>


#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "Function.h"
#include "Rastrigin.h"
#include "DeJong.h"
#include "Schwefel.h"
#include "Michalewicz.h"
using namespace std;

#define PI 3.14159265358979323846

mt19937_64 generator;

/* De Jong Parameters
Population Size: 100
Max number of generations: 1000
Max number of no improvement: 200
Mutation rate: 0.01
Crossover rate: 0.3
*/

/*Rastrigin Parameters

*/

int main() {

	Michalewicz dejongFunction(-5.12, 5.12, 5, 5, 100);
	//Michalewicz michalewiczFunction(0, PI, 30, 5, 100);
	auto start = std::chrono::high_resolution_clock::now();
	dejongFunction.geneticAlgorithm();
	//michalewiczFunction.geneticAlgorithm();

    auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	//std::cout << endl << "Execution time: " << duration.count() * 0.001 << " seconds" << std::endl;
	printf("Execution time: %.5f seconds\n",duration.count() * 0.001);

    return 0;
}
