//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_ALGO_H
#define MHE_ALGO_H
#include <vector>
#include "../include/ObjectiveFunction.h"

class Algo {
public:
    static std::vector<int> checkIfValidInput(const std::vector<std::vector<int>>& graph, int k) {
        if (graph.size() < k || k < 1) {
            std::cerr << "\nThere are fewer nodes than partitions expected n=" << graph.size() << " vs. k=" << k << "). No valid assignment cannot be created with those requirements!";
            return std::vector<int>(graph.size(), -1);
        } else {
            return std::vector<int>(graph.size(), 0);
        }

    }

    static bool isValidAssignment(const std::vector<int>& vec, int k) {
        // Create a boolean array to track presence of numbers from 0 to k
        std::vector<bool> found(k + 1, false); // Initialize all entries as false

        // Mark numbers from vec that are within range 0 to k
        for (int num : vec) {
            if (num >= 0 && num <= k) {
                found[num] = true;
            }
        }

        // Check if any number from 0 to k is found
        return std::any_of(found.begin(), found.end(), [](bool val) { return val; });
    }

    // Wrapper to get cost value when I don't care about visualising all the middle steps, i.e. in algos that is.
    static int getAssignmentValue(const std::vector<std::vector<int>>& graph, const std::vector<int>& assignment) {
        std::vector<std::vector<int>> costMatrix = ObjectiveFunction::calculateCostMatrix(graph, assignment);
        int costValue = ObjectiveFunction::getCostValue(costMatrix);
        return costValue;
    }

    static std::vector<int> randomAssign(const std::vector<std::vector<int>>&inputGraph, int k) {
        std::vector<int> nodeAssginment;
        // This method assigns randomly each node in the graph to one of the partitions (i.e. making random cut).
        if(k < 0) {
            std::cerr << "\nProvided k value must be positive!";
            return nodeAssginment;
        }

        std::uniform_int_distribution<int> distrPartRange(0, k-1);
        for (int i = 0; i < inputGraph.size(); i++) {
            nodeAssginment.push_back(distrPartRange(mhe::mt));
        }

        return nodeAssginment;
    }
};


#endif //MHE_ALGO_H
