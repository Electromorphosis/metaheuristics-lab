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
    static std::vector<std::string> calculate(const std::vector<std::vector<int>>& graph, const int k, bool quietMode, bool runValidation, std::string neighStrat, std::string termStrat, int termValue) {
        int minCost = std::numeric_limits<int>::max();
        bool improvementFound = true;

        // TODO: Might want check other strategies for first assignment ?
        // Initialize first assignment randomly
        std::vector<int> currentAssignment = randomAssign(graph, k);

        if (!quietMode) {
            std::cout << "Initial minCost = infinite\n";
        }

        if(termStrat == "absolute") {
            for(int iterations = 0; iterations < termValue; iterations++) {
                currentAssignment = getBestNeighbour(graph, currentAssignment, k, neighStrat, quietMode);
                if(!quietMode) {
                    std::cout << "Iteration: " << iterations << std::endl;
                }
            }
        } else if(termStrat == "stucked") {
            int stuckedCount = 0;
            int iterations = 0;
            while(stuckedCount < termValue) {
                auto oldAssignment = currentAssignment;
                currentAssignment = getBestNeighbour(graph, currentAssignment, k, neighStrat, quietMode);
                if(std::equal(oldAssignment.begin(), oldAssignment.end(),currentAssignment.begin(), currentAssignment.end())) {
                    stuckedCount++;
                    if(!quietMode) {
                        std::cout << "Stucked count: " << stuckedCount << std::endl;
                    }
                }
                if(!quietMode) {
                    std::cout << "Iterations: " << iterations << std::endl;
                }
                iterations++;
            }
        }

        std::vector<std::string> outcome;
        outcome.push_back(Algo::vectorToString(currentAssignment));
        outcome.push_back(std::to_string(getAssignmentValue(graph, currentAssignment)));
        return outcome;
    }

static std::vector<int> getBestNeighbour(const std::vector<std::vector<int>>& graph, const std::vector<int>& initialAssignment, const int k, std::string neighStrat, bool quietMode) {
        auto currentAssignment = initialAssignment;
    if(neighStrat == "modify") {
    // TODO Try assigning one node to any other partition
    } else if(neighStrat == "flip") {
        for (int i = 0; i < currentAssignment.size(); i++) {
            std::vector<int> copyAssignment = currentAssignment;
            copyAssignment.at(i) += 1;
            if(copyAssignment.at(i) > k+1) continue;
            if(isNewAssignmentBetter(graph, currentAssignment, copyAssignment) && Algo::isValidAssignment(copyAssignment, k)) {
                if(!quietMode) {
                    std::cout << "Better assignment found!" << std::endl;
                    std::cout << Algo::vectorToString(copyAssignment) << " Value = " << std::to_string(getAssignmentValue(graph, copyAssignment)) << std::endl;
                }
                currentAssignment = copyAssignment;
            }

            copyAssignment.at(i) -= 2;
            if(copyAssignment.at(i) < 0) continue;
            if(isNewAssignmentBetter(graph, currentAssignment, copyAssignment) && Algo::isValidAssignment(copyAssignment, k)) {
                currentAssignment = copyAssignment;
            }
        }
        // Decrement/increment one node by one index of partition
    } else if(neighStrat == "swap") {
        // TODO Change position between two nodes
    }

    return currentAssignment;
}

static bool isNewAssignmentBetter(const std::vector<std::vector<int>>& graph, const std::vector<int> oldAssignment, const std::vector<int> newAssignment) {
    int oldValue = getAssignmentValue(graph, oldAssignment);
    int newValue = getAssignmentValue(graph, newAssignment);
    return newValue < oldValue;
}

};


#endif //MHE_CLIMBING_H
