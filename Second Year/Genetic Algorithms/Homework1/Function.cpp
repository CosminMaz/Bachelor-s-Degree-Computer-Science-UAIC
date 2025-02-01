#include "Function.h"

//Generator ideea taken from Lect. Dr. Eugen Croitoru
void Function::initializeRandom(long long int rep) {
	std::mt19937_64 helper;
	helper.seed(time(NULL) + rep * 1000 + clock() * 1000  + hash<thread::id>{}(this_thread::get_id()));
	helper.discard(23412 + rep);
	this->generator.seed(helper());
}

void Function::generateRandomBitString(vector<bool> &bitString) {
	int random = 0;
	initializeRandom(0);
	random = this->generator();
	bitString.clear();
	for (long long int i = 0; i < this->bitStringLength; ++i) {
		if (random <= 3) {
			initializeRandom(i);
			random = this->generator();
		}
		bitString.push_back(random % 2);
		random /= 3;
	}
}
    
double Function::randomSubunitary() {
	return (double)((double)(this->generator() % 100000)/100000);
}

double Function::evaluate(const vector<bool> &sol) {
	long long int decimal = 0;
	vector<double>parameters;
	long long int k = 0;
	//double max = (1 << this->n) - 1;
	for(long long int i = 0; i < this->bitStringLength; i++){
		decimal += (sol[i] * (1 << (this->n - 1 - k))); //Idea with shifting bits from Gabriel Antoniev
		k++;
		if(k == n){
			parameters.push_back((this->a + decimal * (this->b - this->a) / this->max));
			k = 0;
			decimal = 0;
		}
	}
	return calculateFunction(parameters);
}

void Function::hillClimbing_firstImprovement() {
	long long int i;
	int x = 0;
	double decimal;
	long long int k = 0;
	double candidateResult;
	double neighbourResult;
	double bestResult = -numeric_limits<double>::max();
	vector<bool> candidate = {1,1,1,0,1};
	//candidate.reserve(this->bitStringLength);
	for(long long int t = 0; t < this->steps; ++t){
		//generateRandomBitString(candidate);
		candidateResult = evaluate(candidate);
		for(long long int i = 0; i < this->bitStringLength; i++){
					decimal += (candidate[i] * (1 << (this->n - 1 - k))); //Idea with shifting bits from Gabriel Antoniev
					k++;
					if(k == n){
						std::cout << decimal << "\n";
						k = 0;
						decimal = 0;
					}
				}
		i = 0;
		do {
			candidate[i] = 1 - candidate[i];
			neighbourResult = evaluate(candidate);
			if (neighbourResult > candidateResult) {
				candidateResult = neighbourResult;
				for(long long int i = 0; i < this->bitStringLength; i++){
					decimal += (candidate[i] * (1 << (this->n - 1 - k))); //Idea with shifting bits from Gabriel Antoniev
					k++;
					if(k == n){
						std::cout << decimal << "\n";
						k = 0;
						decimal = 0;
					}
				}
				i = 0;
			} else {
				candidate[i] = 1 - candidate[i];
				++i;
			}
		}while(i < this->bitStringLength);
		if(candidateResult > bestResult){ 
			bestResult = candidateResult;
			x = 0;
		} else 
			++x;
		if(x == 200) //1 for DeJong1 //100 for Schewefel //200 for Rastrigin and Michalewicz
			break;
	} 
	printf("\nFirst Improvement -> Optim = %.5f \n", bestResult);
}

void Function::hillClimbing_bestImprovement() {
	bool local;
	int x = 0;
	double decimal;
	long long int k = 0;
	long long int bestIndex;
	double candidateResult;
	double bestNeighbourResult;
	double bestFinalResult = -numeric_limits<double>::max();
	double temp;
	vector<bool> candidate = {0,1,1,1,0};
	//candidate.reserve(this->bitStringLength);
	for(long long int t = 0; t < this->steps; t++){	
		local = false;
		//generateRandomBitString(candidate);
		candidateResult = evaluate(candidate);
		bestIndex = -1;
		for(long long int i = 0; i < this->bitStringLength; i++){
					decimal += (candidate[i] * (1 << (this->n - 1 - k))); //Idea with shifting bits from Gabriel Antoniev
					k++;
					if(k == n){
						std::cout << decimal << "\n";
						k = 0;
						decimal = 0;
					}
				}
		do {
			bestNeighbourResult = candidateResult;
			for(long long int i = 0; i < this->bitStringLength; i++){
				candidate[i] = 1 - candidate[i];
				temp = evaluate(candidate);
				if(temp > bestNeighbourResult){
					bestIndex = i;
					bestNeighbourResult = temp;				
				}
				candidate[i] = 1 - candidate[i];
			}
			if(bestNeighbourResult > candidateResult){
				candidate[bestIndex] = 1 - candidate[bestIndex];
				candidateResult = bestNeighbourResult;
				k = 0;
				for(long long int i = 0; i < this->bitStringLength; i++){
					decimal += (candidate[i] * (1 << (this->n - 1 - k))); //Idea with shifting bits from Gabriel Antoniev
					k++;
					if(k == n){
						std::cout << decimal << "\n";
						k = 0;
						decimal = 0;
					}
				}
			} else
				local = true;
		} while (local == false);

		if(candidateResult > bestFinalResult){
			bestFinalResult = candidateResult;
			//x = 0;
		} //else 
			//++x;
		//if(x == 200) //1 for DeJong1 //100 for Schewefel //200 for Rastrigin and Michalewicz
			//break;
	}
	
	printf("\nBest Improvement -> Optim = %.5f \n ", bestFinalResult);
}

void Function::hillClimbing_worstImprovement(){
	bool local;
	long long int bestIndex;
	int x = 0;
	double candidateResult;
	double bestNeighbourResult;
	double bestFinalResult = numeric_limits<double>::max();
	double temp;
	vector<bool> candidate;
	candidate.reserve(this->bitStringLength);
	for(long long int t = 0; t < this->steps; ++t){	
		local = false;
		generateRandomBitString(candidate);
		candidateResult = evaluate(candidate);
		bestIndex = 0;
		do {
			candidate[0] = 1 - candidate[0];	
			bestNeighbourResult = evaluate(candidate);
			candidate[0] = 1 - candidate[0];
			for(long long int i = 1; i < this->bitStringLength; ++i){
				candidate[i] = 1 - candidate[i];
				temp = evaluate(candidate);
				if(temp > bestNeighbourResult && temp < candidateResult){
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
			x = 0;
		} else 
			++x;
		if(x == 200) //1 for DeJong1 //100 for Schewefel //200 for Rastrigin and Michalewicz
			break;
	}
	printf("\nWorst Improvement -> Optim = %.5f \n ", bestFinalResult);
}


void Function::simulatedAnnealing(){
	long long int i;
	int x = 0;
	double T = 0.99 / log2(1);
	double initalTemperature = T;
	//double cooling = 0.9;
	double candidateResult;
	double neighbourResult;
	double bestResult = numeric_limits<double>::max();
	vector<bool> candidate;
	candidate.reserve(this->bitStringLength);
	T = 0.99;
	//int t = 0;
	for(int t = 0 ; t < this->steps; t++){
		generateRandomBitString(candidate);
		candidateResult = evaluate(candidate);
			for(i = 0; i < this->bitStringLength; ++i){
				candidate[i] = 1 - candidate[i];
				neighbourResult = evaluate(candidate);
				if(neighbourResult < candidateResult) {
					candidateResult = neighbourResult;	
					continue;
				} else if (randomSubunitary() < exp((-abs(neighbourResult - candidateResult) / T))){
					candidateResult = neighbourResult;
					//T = T * pow(1 + 0.9 * T, -1);
					//T = T * pow(0.9, i);	
					//T *= 0.9;
					//T = T * pow(0.9, i);
					//T = 0.9 * T / log2(1 + t);
					//T = 0.99 / log2(1 + t);
					//T = 0.99 / log2(1 + t);
					//T = T * pow(0.99, t);
					continue;
				}
				candidate[i] = 1 - candidate[i];
			}
		//T = T * pow(0.9, i);
		T = T * pow(0.99 , t); //Best Results
		//T = 0.99 / log2(1 + t);
		//T = T * pow(1 + 0.9 * T, -1);
		//T = 0.99 / log2(1 + t);
		if(candidateResult < bestResult){
			bestResult = candidateResult;
			x = 0;
		}else 
			x++;
		if(x == 10000) // 1 for DeJong / 100 For Schwefel / 200 For Rastrigin and 
			break;
		//t++;
	}
	printf("\nSimulated annealing -> Optim = %.5f \n", bestResult);
}
