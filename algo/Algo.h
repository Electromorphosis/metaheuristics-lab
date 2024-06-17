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

    // Wrapper to get cost value when I don't care about visualising all the middle steps, i.e. in algos that is.
    static int getAssignmentValue(const std::vector<std::vector<int>>& graph, const std::vector<int>& assignment) {
        std::vector<std::vector<int>> costMatrix = ObjectiveFunction::calculateCostMatrix(graph, assignment);
        int costValue = ObjectiveFunction::getCostValue(costMatrix);
        return costValue;
    }

};


#endif //MHE_ALGO_H
