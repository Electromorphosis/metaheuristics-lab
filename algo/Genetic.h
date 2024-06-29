//
// Created by mechlon on 19.06.24.
//

#ifndef MHE_GENETIC_H
#define MHE_GENETIC_H

#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include "Algo.h"
#include "../include/namespace.h"

class Genetic : public Algo {
public:
    // TODO: (very optional) Tournament selection can be implemented and added to it, enum for parentSelection

    static std::vector<std::string> calculate(const std::vector<std::vector<int>> &graph, int k, bool quietMode, bool runValidation, int crossingPoints, const std::string& mutationType, const std::string& terminationStrategy, int termValue) {
        int n = graph.size();
        int popSize = n * 3;
        int numParents = popSize / 4;
        // Generate initial population
        std::vector<std::vector<int>> population = generatePopulation(graph, n, k, popSize); // TODO parametrized population size
        if(!quietMode) {
//            std::cout << "Inital population:\n";
            std::cout << "Inital population (fragment):\n";
            mhe::printAdjMatrixRow(population, 0);
            mhe::printAdjMatrixRow(population, 1);
            mhe::printAdjMatrixRow(population, 2);
            std::cout << "...\n";
            mhe::printAdjMatrixRow(population, popSize-1);
        }

        // Main loop for genetic algorithm
        if(terminationStrategy == "progress") {
            if(!quietMode) {
                std::cout << "Absolute generations termination mode choosen (" << termValue <<" iterations).\n";
            }
            for(int iterations = 0; iterations < termValue; iterations++) {
                // Fitness evaluation - evaluate assignment value for the whole population
                std::vector<int> fitnessValues(popSize);
                for (int i = 0; i < popSize; ++i) {
                    fitnessValues[i] = getAssignmentValue(graph, population[i]);
                }
                if(!quietMode) {
                    int sumValue = 0;
                    for (int i = 0; i < popSize; ++i) {
                        sumValue += fitnessValues[i];
                    }
                    double avgValue = static_cast<double>(sumValue) / static_cast<double>(popSize);
                    std::cout << "Generation (" << iterations <<"), it's value is: " << avgValue << "\n";
                }

                // Selection (choose parents for next generation)
                std::vector<std::vector<int>> parents = selectParents(population, fitnessValues, numParents);
                // TODO Parent pop size could be diversified / parametrized
                if(!quietMode) {
                    std::cout << "\tParent selected:\n";
                    for (std::vector<int> parent : parents) {
                        std::cout << "\t" << Algo::vectorToString(parent) << " Val = " << Algo::getAssignmentValue(graph, parent) << "\n";
                    }
                }

                // Crossover (generate offspring)
                std::vector<std::vector<int>> offspring = crossover(parents, 0.5, crossingPoints);
                // TODO Same as above for crossoverRate

                // Mutation (introduce random changes)
                mutate(offspring, 0.5, mutationType);
                // TODO aand same for mutationRate

               // Combine offspring with parents to form new population
               population.insert(population.end(), offspring.begin(), offspring.end());

               // TODO Here could elitism happened
            }
        } else if(terminationStrategy == "epsilon") {
            if(!quietMode) {
                std::cout << "Epsilon mode termination choosen (" << termValue <<" iterations).\n";
            }
            double epsilon = static_cast<double>(termValue)+1.0; // Default value to start the main loop
            while(epsilon > termValue) {
                // Fitness evaluation - evaluate assignment value for the whole population
                std::vector<int> fitnessValues(popSize);
                int oldSumValue = 0;
                for (int i = 0; i < popSize; ++i) {
                    fitnessValues[i] = getAssignmentValue(graph, population[i]);
                    oldSumValue += fitnessValues[i];
                }
                double previousGenValue = static_cast<double>(oldSumValue) / static_cast<double>(popSize);

                // Selection (choose parents for next generation)
                std::vector<std::vector<int>> parents = selectParents(population, fitnessValues, numParents);
                if(!quietMode) {
                    std::cout << "\tParents selected:\n";
                    for (std::vector<int> parent : parents) {
                        std::cout << "\t" << Algo::vectorToString(parent) << " Val = " << Algo::getAssignmentValue(graph, parent) << "\n";                    }
                }

                // Crossover (generate offspring)
                std::vector<std::vector<int>> offspring = crossover(parents, 0.5, crossingPoints);

                // Mutation (introduce random changes)
                mutate(offspring, 0.1, mutationType);

                // Combine offspring with parents to form new population
                population.insert(population.end(), offspring.begin(), offspring.end());

                int newSumValue = 0;
                for (int i = 0; i < popSize; ++i) {
                    fitnessValues[i] = getAssignmentValue(graph, population[i]);
                    newSumValue += fitnessValues[i];
                }
                double newGenValue = static_cast<double>(newSumValue) / static_cast<double>(popSize);
                epsilon = oldSumValue - newGenValue;
            }
        }
        // Identify best individual from final population
        int bestIndex = 0;
        int bestCost = std::numeric_limits<int>::max();
        for (int i = 0; i < popSize; ++i) {
            int cost = getAssignmentValue(graph, population[i]);
            if (cost < bestCost) {
                bestIndex = i;
                bestCost = cost;
            }
        }

        std::vector<std::string> outcome;
        outcome.push_back(Algo::vectorToString(population[bestIndex]));
        outcome.push_back(std::to_string(getAssignmentValue(graph, population[bestIndex])));
        return outcome;
    }

    static std::vector<std::vector<int>> generatePopulation(const std::vector<std::vector<int>>& graph, int n, int k, int populationSize) {
        // This implementation provides at least one node in each partition - please note that:
        // TODO: There might be more different approaches for creating random start pop. with good qualities, such as variability
        // (for small k and big n this method may favor first partition, which might skew variability!!
        std::vector<std::vector<int>> population(populationSize, std::vector<int>(n, 0)); // Initialize empty population
        for (std::vector<int>& specimen : population) {
            specimen = Algo::randomAssign(graph, k);
        }

        return population;
    }


    static std::vector<std::vector<int>> selectParents(const std::vector<std::vector<int>>& population, const std::vector<int>& fitnessValues, int numParents) {
        // Roulette Wheel Selection
        std::vector<std::vector<int>> selectedParents;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0); // Random float between 0 (inclusive) and 1 (exclusive)

        // Calculate total fitness
        int totalFitness = 0;
        for (int fitness : fitnessValues) {
            totalFitness += fitness;
        }

        // Create a "fitness roulette"
        std::vector<double> rouletteWheel(population.size());
        double currentFitness = 0.0;
        for (int i = 0; i < population.size(); ++i) {
            currentFitness += (double)fitnessValues[i] / totalFitness;
            rouletteWheel[i] = currentFitness;
        }

        // Select parents using roulette wheel selection
        for (int i = 0; i < numParents; ++i) {
            double selectionPoint = dis(gen);
            int selectedIndex = 0;
            while (selectionPoint > rouletteWheel[selectedIndex]) {
                selectedIndex++;
            }
            selectedParents.push_back(population[selectedIndex]);
        }

        return selectedParents;
    }


    static std::vector<std::vector<int>> crossover(const std::vector<std::vector<int>>& parents, double crossoverRate, int crossoverPoints) {
        std::vector<std::vector<int>> offspring;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0); // Random float between 0 (inclusive) and 1 (exclusive)
        std::uniform_int_distribution<> disPoint(1, parents.at(0).size() - 1); // Random int between 1 (inclusive) and parent1.size() - 1 (exclusive)

        for (int i = 0; i < parents.size(); i += 2) { // Process parents in pairs
            // Decide if crossover will happen based on crossover rate
            if (dis(gen) < crossoverRate) {
                int numParents = parents.size();
                int parent1Index = i % numParents;
                int parent2Index = (i + 1) % numParents;
                const std::vector<int>& parent1 = parents[parent1Index];
                const std::vector<int>& parent2 = parents[parent2Index];

                if (crossoverPoints == 1) {
                    // One-point crossover
                    std::uniform_int_distribution<> disPoint(1, parent1.size() - 1); // Random int between 1 (inclusive) and parent1.size() - 1 (exclusive) to avoid full copy
                    int crossoverPoint = disPoint(gen);
                    std::vector<int> offspring1(parent1.size());
                    std::vector<int> offspring2(parent1.size());
                    std::copy(parent1.begin(), parent1.begin() + crossoverPoint, offspring1.begin());
                    std::copy(parent2.begin() + crossoverPoint, parent2.end(), offspring1.begin() + crossoverPoint);
                    std::copy(parent2.begin(), parent2.begin() + crossoverPoint, offspring2.begin());
                    std::copy(parent1.begin() + crossoverPoint, parent1.end(), offspring2.begin() + crossoverPoint);
                    offspring.push_back(offspring1);
                    offspring.push_back(offspring2);
                } else if (crossoverPoints > 1) {
                    int numPoints = crossoverPoints; // Number of crossover points (excluding endpoints)
                    std::vector<int> crossoverPoints(numPoints + 2);
                    crossoverPoints[0] = 0; // Start point (inclusive)
                    crossoverPoints[numPoints + 1] = parent1.size(); // End point (exclusive)
                    for (int j = 1; j <= numPoints; ++j) {
                        crossoverPoints[j] = disPoint(gen); // Random crossover points between start and end (exclusive)
                    }
                    std::sort(crossoverPoints.begin(), crossoverPoints.end()); // Ensure crossover points are in order

                    std::vector<int> offspring1(parent1.size());
                    std::vector<int> offspring2(parent1.size());
                    int currentParent = 0; // Switch between parents at each crossover point
                    for (int j = 0; j < crossoverPoints.size() - 1; ++j) {
                        int from = crossoverPoints[j];
                        int to = crossoverPoints[j + 1];
                        std::copy(parent1.begin() + from, parent1.begin() + to, offspring1.begin() + from);
                        std::copy(parent2.begin() + from, parent2.begin() + to, offspring2.begin() + from);
                        currentParent = (currentParent + 1) % 2;
                    }
                    offspring.push_back(offspring1);
                    offspring.push_back(offspring2);
                }
            }
        }

        return offspring;
    }

    static std::vector<std::vector<int>> mutate(const std::vector<std::vector<int>>& population, double mutationRate, const std::string& mutationType) {
        std::vector<std::vector<int>> mutatedPopulation = population;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0); // Random float between 0 (inclusive) and 1 (exclusive)

        for (int i = 0; i < population.size(); ++i) {
            for (int j = 0; j < population[i].size(); ++j) {
                // Decide if mutation will happen based on mutation rate
                if (dis(gen) < mutationRate) {

                    if (mutationType == "flip") {
                        // Bit flip mutation
                        mutatedPopulation[i][j] = (mutatedPopulation[i][j] == 0) ? 1 : 0; // Flip the bit
                    } else if (mutationType == "swap") {
                        // Swap mutation
                        if (j + 1 < population[i].size()) {
                            int temp = mutatedPopulation[i][j];
                            mutatedPopulation[i][j] = mutatedPopulation[i][j + 1];
                            mutatedPopulation[i][j + 1] = temp;
                        }
                    }
                }
            }
        }

        return mutatedPopulation;
    }

};
#endif //MHE_GENETIC_H
