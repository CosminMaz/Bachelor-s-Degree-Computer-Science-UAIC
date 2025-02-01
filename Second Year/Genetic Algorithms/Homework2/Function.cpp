#include "Function.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iostream>

void Function::initializeRandom(long long int rep) {
    std::mt19937_64 helper;
    helper.seed(time(nullptr) + rep * 1000 + clock() * 1000 + std::hash<std::thread::id>{}(std::this_thread::get_id()));
    helper.discard(23412 + rep);
    this->generator.seed(helper());
}

void Function::generateRandomBitString(std::vector<bool>& bitString) {
    initializeRandom(0);
    bitString.clear();
    for (long long int i = 0; i < this->bitStringLength; ++i) {
        bitString.push_back(this->generator() % 2);
    }
}

double Function::randomSubunitary() {
    return (double)(this->generator() % 100000) / 100000.0;
}

void Function::hillClimbing_bestImprovement(const vector<bool> &bestC) {
	bool local;
	long long int bestIndex;
	double candidateResult;
	double bestNeighbourResult;
	double bestFinalResult = numeric_limits<double>::max();
	double temp;
	vector<bool> candidate = bestC;
	//candidate.reserve(this->bitStringLength);
	local = false;
		//generateRandomBitString(candidate);
	candidateResult = evaluate(candidate);
	bestIndex = -1;
	do {
		bestNeighbourResult = candidateResult;
		for(long long int i = 0; i < this->bitStringLength; i++){
			candidate[i] = 1 - candidate[i];
			temp = evaluate(candidate);
			if(temp < bestNeighbourResult){
				bestIndex = i;
				bestNeighbourResult = temp;				
			}
			candidate[i] = 1 - candidate[i];
		}
		if(bestNeighbourResult < candidateResult){
			candidate[bestIndex] = 1 - candidate[bestIndex];
			candidateResult = bestNeighbourResult;
		} else
			local = true;
		} while (local == false);

		if(candidateResult < bestFinalResult){
			bestFinalResult = candidateResult;
		} 
	
	printf("\n--> Optim = %.5f \nNumber of generations: %lld\n", bestFinalResult, this->generationsCounter);
}

double Function::evaluate(const std::vector<bool>& sol) {
    std::vector<double> parameters = decodeChromosome(sol);
    return calculateFunction(parameters);
}

double Function::evaluateFitness(const std::vector<bool>& bitString) {
    return evaluate(bitString);
}

std::vector<double> Function::decodeChromosome(const std::vector<bool>& bitString) {
    long long int decimal = 0;
    std::vector<double> parameters;
    long long int k = 0;

    for (long long int i = 0; i < this->bitStringLength; ++i) {
        decimal += (bitString[i] * (1 << (this->n - 1 - k)));
        k++;
        if (k == n) {
            parameters.push_back(this->a + decimal * (this->b - this->a) / this->max);
            k = 0;
            decimal = 0;
        }
    }
    return parameters;
}

void Function::evaluatePopulation() {
    for (auto& individual : this->Population) {
        individual.fitness = evaluateFitness(individual.chrom);
    }
}

void Function::initializePopulation() {
    this->Population.clear();
    for (long long int i = 0; i < this->populationSize; ++i) {
        chromosome candidate;
        generateRandomBitString(candidate.chrom);
        candidate.fitness = evaluateFitness(candidate.chrom);
        candidate.selectionProbability = 0.0;
        this->Population.push_back(candidate);
    }
}

void Function::crossover(std::vector<Function::chromosome>& selectedParents) {
    const double crossoverProbability = 0.7; // Probability of crossover
    for (size_t i = 0; i < selectedParents.size() - 1; i += 2) {
        if (randomSubunitary() < crossoverProbability) { // Only perform crossover with a certain probability
            long long int cutIndex = this->generator() % this->bitStringLength;
            for (long long int j = cutIndex; j < this->bitStringLength; ++j) {
                swap(selectedParents[i].chrom[j], selectedParents[i + 1].chrom[j]);
            }
        }
    }
}

void Function::mutate(std::vector<Function::chromosome>& selectedParents) {
    for (auto& individual : selectedParents) {
        for (long long int j = 0; j < this->bitStringLength; ++j) {
            if (randomSubunitary() <= 0.03) {
                individual.chrom[j] = !individual.chrom[j];
            }
        }
    }
}

void Function::selectPopulation(std::vector<Function::chromosome>& pop) {
    double popFitness = 0.0;
    for (const auto& individual : this->Population) {
        popFitness += individual.fitness;
    }

    std::vector<double> cumulativeProbabilities;
    cumulativeProbabilities.push_back(0.0);
    for (const auto& individual : this->Population) {
        double prob = individual.fitness / popFitness;
        cumulativeProbabilities.push_back(cumulativeProbabilities.back() + prob);
    }

    pop.clear();
    for (long long int i = 0; i < this->populationSize; ++i) {
        double pick = randomSubunitary();
        for (size_t j = 1; j < cumulativeProbabilities.size(); ++j) {
            if (cumulativeProbabilities[j - 1] <= pick && pick < cumulativeProbabilities[j]) {
                pop.push_back(this->Population[j - 1]);
                break;
            }
        }
    }
}

std::vector<Function::chromosome> Function::elitism() {
    size_t eliteSize = std::max(1LL, (long long int)(this->populationSize * 0.1));
    std::vector<chromosome> temp = this->Population;
    std::sort(temp.begin(), temp.end(), [](const chromosome& a, const chromosome& b) {
        return a.fitness < b.fitness;
        });
    return std::vector<chromosome>(temp.begin(), temp.begin() + eliteSize);
}

void Function::geneticAlgorithm() {
    double globalBest = std::numeric_limits<double>::max();
	std::vector<bool>bestChromosome;
    std::vector<chromosome> newPopulation;
    std::vector<chromosome> eliteGroup;
    double previousBest = std::numeric_limits<double>::max();
    int stagnantGenerations = 0;

    initializePopulation();

    while (this->generationsCounter < 10000) {
        this->generationsCounter++;

        // Apply elitism
        eliteGroup = elitism();

        // Select parents for crossover
        newPopulation.clear();
        selectPopulation(newPopulation);

        // Apply crossover
        crossover(newPopulation);

        // Apply mutation
        mutate(newPopulation);

        // Merge elite and new population
        this->Population.clear();
        std::copy(eliteGroup.begin(), eliteGroup.end(), std::back_inserter(this->Population));
        std::copy(newPopulation.begin(), newPopulation.end(), std::back_inserter(this->Population));

        // Evaluate population fitness
        evaluatePopulation();

        // Track the best solution
        for (const auto& individual : this->Population) {
            if (individual.fitness < globalBest) {
                globalBest = individual.fitness;
				bestChromosome = individual.chrom;
            }
        }

        // Convergence check
        if (fabs(globalBest - previousBest) < 1e-5) {
            stagnantGenerations++;
            if (stagnantGenerations >= 10000) {
                break;
            }
        }
        else {
            stagnantGenerations = 0;
        }
        previousBest = globalBest;
    }
	printf("Best Chrosome fitness: %f", globalBest);
    hillClimbing_bestImprovement(bestChromosome);
}