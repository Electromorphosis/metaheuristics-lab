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

    // TODO: Those assignment logic below and in BruteForce.h needs refactoring - I need to get more thought about how to sensible generate ALL possible assignments or at least all of them one by one...
    // self-explanatory title...
    bool isValidAssignment(const std::vector<int>& assignment, int k) {
        std::vector<int> count(k, 0);
        for (int part : assignment) {
            count[part]++;
        }
        for (int i = 0; i < k; ++i) {
            if (count[i] == 0) {
                return false;
            }
        }
        return true;
    }

    // also self-explanatory - checks if next assignment exists
    bool hasNextAssignment(std::vector<int>& assignment, int k) {
        int n = assignment.size();

        // Try to increment the assignment like a k-ary counter
        for (int i = n - 1; i >= 0; --i) {
            if (assignment[i] < k - 1) {
                assignment[i]++;
                // Reset all elements to the right
                for (int j = i + 1; j < n; ++j) {
                    assignment[j] = 0;
                }
                // Check if the new assignment is valid
                if (isValidAssignment(assignment, k)) {
                    return true;
                }
                i = n; // Restart the loop to recheck from the end
            }
        }
        return false; // No more valid assignments
    }

};


#endif //MHE_ALGO_H
