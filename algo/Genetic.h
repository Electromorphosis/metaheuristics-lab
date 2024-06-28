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

class GeneticAlgorithm : public Algo {
public:
//    enum EndingCondition {
//        MAX_GEN_NUM,
//        CONVERGE_BY_CONSTANT,
//        TIME_CONSTRAINT
//    };

    enum Mutation {
        BIT_FLIP,
        SWAP
    };

//    enum Crossover {
//        ONE_POINT,
//        MULTI_POINT
//    };

    // TODO: (very optional) Tournament selection can be implemented and added to it, enum for parentSelection

    struct Parameters {
        int populationSize; // Number of individuals in the population
        int maxGenerations; // Maximum number of generations
        double crossoverRate; // Probability of crossover between parents
        double mutationRate; // Probability of mutation in an individual
//        EndingCondition endingCondition;
        Mutation mutationType;
        int crossoverPoints;
    };

    static std::vector<int> calculate(const std::vector<std::vector<int>> &graph, int k, const Parameters &params) {
        int n = graph.size();

        // Generate initial population
        std::vector<std::vector<int>> population = generatePopulation(n, k, params.populationSize);

        // Main loop for genetic algorithm
        for (int generation = 0; generation < params.maxGenerations; ++generation) {
            // Fitness evaluation
            std::vector<int> fitnessValues(population.size());
            for (int i = 0; i < population.size(); ++i) {
                fitnessValues[i] = getAssignmentValue(graph, population[i]);
            }

            // Selection (choose parents for next generation)
            std::vector<std::vector<int>> parents = selectParents(population, fitnessValues, params.populationSize);

            // Crossover (generate offspring)
            std::vector<std::vector<int>> offspring = crossover(parents, params.crossoverRate, params.crossoverPoints);

            // Mutation (introduce random changes)
            mutate(offspring, params.mutationRate, params.mutationType);

            // Combine offspring with parents to form new population
            population.insert(population.end(), offspring.begin(), offspring.end());

            // TODO: Elitism (optional: keep best individuals from previous generation)

        }

        // Identify best individual from final population
        int bestIndex = 0;
        int bestCost = std::numeric_limits<int>::max();
        for (int i = 0; i < population.size(); ++i) {
            int cost = getAssignmentValue(graph, population[i]);
            if (cost < bestCost) {
                bestIndex = i;
                bestCost = cost;
            }
        }

        return population[bestIndex];
    }

private:
    static std::vector<std::vector<int>> generatePopulation(int n, int k, int populationSize) {
        // This implementation provides at least one node in each partition - please note that:
        // TODO: There might be more different approaches for creating random start pop. with good qualities, such as variability
        // (for small k and big n this method may favor first partition, which might skew variability!!
        std::vector<std::vector<int>> population(populationSize, std::vector<int>(n, 0)); // Initialize empty population
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, k - 1);

        for (int i = 0; i < populationSize; ++i) {
            // Ensure each partition has at least one node
            std::vector<bool> assigned(k, false); // Use vector with size k
            for (int j = 0; j < n; ++j) {
                int partition;
                do {
                    partition = dis(gen); // Randomly choose a partition
                } while (assigned[partition]); // Keep trying if partition already has a node
                assigned[partition] = true;
                population[i][j] = partition;
            }
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

    static std::vector<std::vector<int>> mutate(const std::vector<std::vector<int>>& population, double mutationRate, Mutation mutationType) {
        std::vector<std::vector<int>> mutatedPopulation = population;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0); // Random float between 0 (inclusive) and 1 (exclusive)

        for (int i = 0; i < population.size(); ++i) {
            for (int j = 0; j < population[i].size(); ++j) {
                // Decide if mutation will happen based on mutation rate
                if (dis(gen) < mutationRate) {

                    if (mutationType == BIT_FLIP) {
                        // Bit flip mutation
                        mutatedPopulation[i][j] = (mutatedPopulation[i][j] == 0) ? 1 : 0; // Flip the bit
                    } else if (mutationType == SWAP) {
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
