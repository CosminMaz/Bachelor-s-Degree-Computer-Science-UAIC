#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include <unordered_set>
#include <thread>
#include <climits>
#include <chrono>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define POP_SIZE 200
#define MAX_GENERATIONS 1000
//#define MUTATION_RATE 0.5
#define CROSSOVER_RATE 0.3

std::mt19937_64 generator;

double MUTATION_RATE = 0.15;

std::vector<std::vector<int>> flow;
std::vector<std::vector<int>> distances;
long long int instance_size;

void init_random(long long int step) {
	std::mt19937_64 helper;
	helper.seed(time(NULL) + step * 1000 + clock() * 1000 + std::hash<std::thread::id>{}(std::this_thread::get_id()));
	helper.discard(657821 + step);
	generator.seed(helper());
}

int random_number(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);

	return dist(gen);
}

void init_instance() {
	long long int x;
	std::ifstream file("./test_instances/chr12a.txt");
	file >> instance_size;

	flow.resize(instance_size);
	distances.resize(instance_size);

	for (long long int i = 0; i < instance_size; ++i) {
		for (long long int j = 0; j < instance_size; ++j) {
			file >> x;
			flow[i].push_back(x);
		}
	}

	for (long long int i = 0; i < instance_size; ++i) {
		for (long long int j = 0; j < instance_size; ++j) {
			file >> x;
			distances[i].push_back(x);
		}
	}
	file.close();
}

void init_population(std::vector<std::vector<long long int>> &pop) {
	std::vector<long long int> temp;
	for (long long int i = 0; i < instance_size; ++i) {
		temp.push_back(i);
	}
	for (long long int i = 0; i < POP_SIZE; ++i) {
		std::shuffle(temp.begin(), temp.end(), std::default_random_engine(random_number(0, 100000)));
		pop[i] = temp;
	}
}

long long int fitness_function(const std::vector<long long int> &chromosome) {
	long long int cost = 0;

	for (long long int i = 0; i < instance_size; ++i) {
		for (long long int j = 0; j < instance_size; ++j) {
			cost += flow[i][j] * distances[chromosome[i]][chromosome[j]];
		}
	}
	return cost;
}

std::vector<long long int> evaluate_population(const std::vector<std::vector<long long int>> &pop) {
	std::vector<long long int>fitness_return;
	
	for (long long int i = 0; i < POP_SIZE; ++i) {
		fitness_return.push_back(fitness_function(pop[i]));
	}
	return fitness_return;
}

void selection(std::vector<std::vector<long long int>>& new_population, const std::vector<std::vector<long long int>>& population, const std::vector<long long int>& fitness) {
	// Tournament Selection
	int tournament_size = 10;
	std::vector<std::pair<long long int, long long int>> participants(tournament_size);
	
	for (long long int i = 0; i < POP_SIZE; ++i) {
		std::unordered_set<long long int> selected_indices;
		for (int j = 0; j < tournament_size; ++j) {
			long long int pos;
			do {
				pos = random_number(0, POP_SIZE - 1); 
			} while (selected_indices.count(pos));
			selected_indices.insert(pos);
			participants[j] = { pos, fitness[pos] };
		}

		auto best = std::min_element(participants.begin(), participants.end(),
			[](const std::pair<long long int, long long int>& a, const std::pair<long long int, long long int>& b) {
				return a.second < b.second;
			});

		new_population[i] = population[best->first];
	}
}

std::vector<long long int> correct(std::vector<long long int> perm) {
	std::vector<long long int> corr;
	std::vector<long long int> unity_permutation;
	corr.resize(instance_size);
	for (long long int i = 0; i < instance_size; ++i) {
		unity_permutation.push_back(i);
	}

	int unity_position, mod;

	for (int i = 0; i < instance_size; i++) {
		mod = instance_size - i;
		perm[i] = perm[i] % mod;
		unity_position = perm[i];
		corr[i] = unity_permutation[unity_position];
		auto delete_index = unity_permutation.begin() + unity_position;
		unity_permutation.erase(delete_index);
	}

	return corr;
}

void crossover(std::vector<std::vector<long long int>> &new_pop) {
	std::vector<long long int> selected_parents;
	for (long long int i = 0; i < POP_SIZE; ++i) {
		if ((double)random_number(0, 100) / 100 <= CROSSOVER_RATE) {
			selected_parents.push_back(i);
		}
	}

	if (selected_parents.size() % 2 != 0) {
		long long int pos;
		do {
			pos = random_number(0, POP_SIZE - 1);
		} while (std::find(selected_parents.begin(), selected_parents.end(), pos) != selected_parents.end());
		selected_parents.push_back(pos);
	}

	std::shuffle(selected_parents.begin(), selected_parents.end(), std::default_random_engine(random_number(0, 100000)));

	for (long long int i = 0; i < selected_parents.size() - 1; i += 2) {
		std::vector<long long int>parent1 = new_pop[selected_parents[i]];
		std::vector<long long int>parent2 = new_pop[selected_parents[i + 1]];
		std::vector<long long int>offspring1;
		std::vector<long long int>offspring2;

		long long int cutting_point1 = random_number(0, instance_size - 1);
		long long int cutting_point2 = random_number(0, instance_size - 1);
		
		for (long long int j = 0; j < cutting_point1; ++j) {
			offspring1.push_back(parent1[j]);
			offspring2.push_back(parent2[j]);
		}

		for (long long int j = cutting_point1; j <= cutting_point2; ++j) {
			offspring1.push_back(parent2[j]);
			offspring2.push_back(parent1[j]);
		}

		for (long long int j = cutting_point2 + 1; j < instance_size; ++j) {
			offspring1.push_back(parent1[j]);
			offspring2.push_back(parent2[j]);
		}

		offspring1 = correct(offspring1);
		offspring2 = correct(offspring2);
		
		new_pop[selected_parents[i]] = offspring1;
		new_pop[selected_parents[i + 1]] = offspring2;
	}
}

void mutate(std::vector<std::vector<long long int>> &new_pop) {
	for (long long int i = 0; i < POP_SIZE; ++i) {
		if (random_number(0, 100) / 100 < MUTATION_RATE) {
			int pos1 = random_number(0, instance_size - 1);
			int pos2;
			do {
				pos2 = random_number(0, instance_size - 1);
			} while (pos1 == pos2);
			std::swap(new_pop[i][pos1], new_pop[i][pos2]);
			
		}
	}
}

std::vector<long long int> generate_neighbour(const std::vector<long long int> &candidate) {
	std::vector<long long int> to_return = candidate;
	long long int pos1 = random_number(0, instance_size - 1);
	long long int pos2;
	do {
		pos2= random_number(0, instance_size - 1);
	} while (pos1 == pos2);

	std::swap(to_return[pos1], to_return[pos2]);

	return to_return;
}

std::pair<long long int, std::vector<long long int>> simulated_annealing(std::vector<long long int>permutation) {
	double candidateResult;
	double neighbourResult;
	double bestResult = INT_MAX;
	std::vector<long long int> bestPermutation = permutation;
	double t, t_min = 10e-10;
	std::vector<long long int>candidate;
	std::vector<long long int>neighbour;
	for (int steps = 0; steps < 10; ++steps) {
		t = 1;
		candidate = permutation;
		candidateResult = fitness_function(candidate);
		 while (t > t_min) {
			neighbour = generate_neighbour(candidate);
			neighbourResult = fitness_function(neighbour);
			candidateResult = fitness_function(candidate);
			if (neighbourResult < candidateResult) {
				candidate = neighbour;
				
			} else if (random_number(0, 100) / 100 < exp((-abs(neighbourResult - candidateResult) / t))) {
				candidate = neighbour;
			}
			t *= 0.99;
			if (candidateResult < bestResult) {
				bestResult = candidateResult;
				bestPermutation = candidate;
			}
			if (neighbourResult < bestResult) {
				bestResult = neighbourResult;
				bestPermutation = neighbour;
			}
		}
	}
	std::pair<long long int, std::vector<long long int>> to_return = { bestResult, bestPermutation };

	return to_return;
}


void genetic_algorithm() {
	long long int best_result = INT_MAX;
	long long int final = INT_MAX;
	int best_index;   
	std::vector<std::vector<long long int>>population;
	std::vector<std::vector<long long int>>new_population;
	population.resize(POP_SIZE);
	new_population.resize(POP_SIZE);
	init_population(population);
	std::vector<long long int>population_fitness = evaluate_population(population);
	int x = 0;
	for (long long int i = 0; i < MAX_GENERATIONS; ++i) {
		selection(new_population, population, population_fitness);
		crossover(new_population);
		mutate(new_population);

		for (long long int y = 0; y < POP_SIZE; ++y) {
			population[y] = new_population[y];
		}

		population_fitness = evaluate_population(population);
		bool found = false;
		long long int worst_index = 0;
		long long int worst_fitness = 0;
		for (long long int j = 0; j < POP_SIZE; ++j) {
			if (population_fitness[j] < best_result) {
				best_result = population_fitness[j];
				best_index = j;
				found = true;
			}
			if (population_fitness[j] > worst_fitness) {
				worst_fitness = population_fitness[j];
				worst_index = j;
			}
		}
		
		std::pair<long long int, std::vector<long long int>>k = simulated_annealing(population[best_index]);

		if (k.first < final) {
			final = k.first;
		}

		population[worst_index] = k.second;

		if (found) {
			x = 0;
		} else {
			x++;
		}

		if (x >= 50) {
			MUTATION_RATE = 0.6;
			x = 0;
		} else {
			MUTATION_RATE = 0.15;
		}
	}
	printf("Best solution: %lld\n", final);
}


int main() {
	init_random(15042004);
	init_instance();

    const int num_processes = 10; // Number of parallel processes
    pid_t pids[num_processes];

    for (int i = 0; i < num_processes; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            auto start = std::chrono::high_resolution_clock::now();

            genetic_algorithm();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            printf("Process %d Execution time: %.5f seconds\n", i + 1, duration.count() * 0.001);
            exit(EXIT_SUCCESS); // Exit the child process
        } else {
            // Parent process
            pids[i] = pid; // Store child PID for reference
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_processes; ++i) {
        waitpid(pids[i], nullptr, 0);
    }

    printf("All processes have completed.\n");
	return 0;
}