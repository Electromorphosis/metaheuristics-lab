//
// Created by mechlon on 17.06.24.
//

#ifndef MHE_CLIMBING_H
#define MHE_CLIMBING_H
#include <string>
#include <vector>
#include <limits>
#include "Algo.h"
#include "../include/namespace.h"

class Climbing : public Algo {
public:
    static std::vector<int> calculate(const std::vector<std::vector<int>>& graph, const int k) {
        std::vector<int> check = checkIfValidInput(graph, k);
        if (check.at(0) == -1) {
            std::cerr << "Input is invalid!";
            return check;
        }

        // TODO: Might want check other strategies for first assignment ?
        std::vector<int> currentAssignment(randomAssign(graph, k));
//        mhe::printAssignment(currentAssignment);

        int minCost = std::numeric_limits<int>::max();
        std::cout << "minCost = " << minCost;
        bool improvementFound = true;

        while (improvementFound) {
            improvementFound = false;

            // Explore all neighbors and find the one with the minimal cost
            for (int i = 0; i < graph.size(); ++i) {
                for (int j = 0; j < k; ++j) {
                    if (currentAssignment[i] != j) {
                        // Create a copy of the current assignment
                        std::vector<int> neighbor(currentAssignment);
                        neighbor[i] = j;

                        int neighborCost = getAssignmentValue(graph, neighbor);
                        std::cout << "\n" << "neighbour cost = " << neighborCost;
                        if (neighborCost < minCost && isValidAssignment(neighbor, k)) {
                            currentAssignment = neighbor;
                            std::cout << "\n New best found!";
                            mhe::printAssignment(currentAssignment);
                            minCost = neighborCost;
                            improvementFound = true;
                            break; // Early termination if better neighbor found
                         }
                    }
                }
            }
        }

        return currentAssignment;
    }

};


#endif //MHE_CLIMBING_H
