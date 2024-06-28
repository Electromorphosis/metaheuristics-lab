//
// Created by mechlon on 19.06.24.
//

#ifndef MHE_TABU_H
#define MHE_TABU_H


#include <vector>
#include <random>
#include <limits>
#include "Algo.h"
#include "../include/namespace.h"

class Tabu : public Algo {
public:
    static std::vector<int>
    calculate(const std::vector<std::vector<int>> &graph, int k, int maxIterations, int tabooSize = 0) {
        int n = graph.size();
        std::vector<int> currentAssignment(n, 0);
        std::vector<int> bestAssignment = currentAssignment;
        int bestCost = getAssignmentValue(graph, currentAssignment);
        for (int i = 0; i <= k; i++) { // TODO: redundancy, move to algo.h
            // Populate with at least one node of each group - first combination.
            bestAssignment.at(i) = i;
        }

        // Tabu list to store recently visited solutions (empty for infinite taboo)
        std::vector<std::vector<int>> tabuList;
        int currentTabuSize = tabooSize > 0 ? tabooSize : n * n; // Infinite taboo by default

        // Random number generator for random neighbor selection
        std::uniform_int_distribution<> dis(0, n - 1);

        for (int i = 0; i < maxIterations; ++i) {
            int bestNeighborCost = std::numeric_limits<int>::max();
            std::vector<int> bestNeighbor;

            // Explore all neighbors
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < k; ++j) {
                    if (currentAssignment[i] != j) {
                        // Create a copy of the current assignment
                        std::vector<int> neighbor(currentAssignment);
                        neighbor[i] = j;

                        // Check if neighbor is not in the tabu list
                        bool isTabu = false;
                        if (currentTabuSize > 0) {
                            for (const auto &tabuEntry: tabuList) {
                                if (neighbor == tabuEntry) {
                                    isTabu = true;
                                    break;
                                }
                            }
                        }

                        // Evaluate neighbor if not tabu
                        if (!isTabu) {
                            int neighborCost = getAssignmentValue(graph, neighbor);
                            if (neighborCost < bestNeighborCost) {
                                bestNeighborCost = neighborCost;
                                bestNeighbor = neighbor;
                            }
                        }
                    }
                }
            }

            // Update solution if a better neighbor found
            if (bestNeighborCost < bestCost) {
                currentAssignment = bestNeighbor;
                bestCost = bestNeighborCost;

                // Update tabu list (remove oldest entry if full)
                if (currentTabuSize > 0) {
                    if (tabuList.size() == currentTabuSize) {
                        tabuList.erase(tabuList.begin());
                    }
                    tabuList.push_back(currentAssignment);
                }
            }
        }

        return bestAssignment;
    }
};


#endif //MHE_TABU_H
