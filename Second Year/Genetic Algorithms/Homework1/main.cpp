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
#include "func.h"
using namespace std;

#define PI 3.14159265358979323846

mt19937_64 generator;

int main() {

	// Rastrigin Function 
	
	//Rastring rastriginFunction(-5.12, 5.12, 30, 5);
	//auto start = std::chrono::high_resolution_clock::now();
	//rastriginFunction.hillClimbing_bestImprovement();
	
	/*	
	pid_t pid;
    for(int k=1;k<=10;k++){
    pid=fork();
    if(pid<0){ perror("fork failed");
            exit(1);}
    if(pid==0){

    auto start = std::chrono::high_resolution_clock::now();
	//Rastring rastriginFunction(-5.12, 5.12, 5, 5);
	//rastriginFunction.simulatedAnnealing();
	Michalewicz michalewiczFunction(0, PI, 5, 5);
	michalewiczFunction.simulatedAnnealing();
	//DeJong deJongFunction(-5.12, 5.12, 5, 5);
	//deJongFunction.simulatedAnnealing();
	//Schwefel schwefelFunction(-500, 500, 5, 5);
	//schwefelFunction.simulatedAnnealing();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
	printf("Execution time: %.5f seconds\n",duration.count() * 0.001);
    std::cout << std::endl<< "**************"<< endl << endl;
    exit(0);
    }

    }
    for (int p = 1; p <= 10; p++) wait(NULL);
	
	
	// DeJong Function
  	//DeJong deJongFunction(-5.12, 5.12, 5, 5);
	//auto start = std::chrono::high_resolution_clock::now();
	//deJongFunction.hillClimbing_bestImprovement();
	
	
	
	
	// Schwefel Function
	//Schwefel schwefelFunction(-500, 500, 5, 5);
	//auto start = std::chrono::high_resolution_clock::now();
	//schwefelFunction.hillClimbing_bestImprovement();
	
	
	
	//Michalewicz michalewiczFunction(0, PI, 5, 5);
	//auto start = std::chrono::high_resolution_clock::now();
	//michalewiczFunction.hillClimbing_firstImprovement();
	
	//%.5f
	*/
	func funct(0, 31, 1, 1);
	auto start = std::chrono::high_resolution_clock::now();
	funct.hillClimbing_bestImprovement();
    auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	//std::cout << endl << "Execution time: " << duration.count() * 0.001 << " seconds" << std::endl;
	printf("Execution time: %.5f seconds\n",duration.count() * 0.001);

    return 0;
}
