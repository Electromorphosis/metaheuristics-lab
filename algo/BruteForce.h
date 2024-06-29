//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_BRUTEFORCE_H
#define MHE_BRUTEFORCE_H
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "Algo.h"
#include "../include/namespace.h"

class BruteForce : public Algo{
public:
    static std::vector<std::string> calculate(const std::vector<std::vector<int>>& graph,const int k, bool quietMode, bool runValidation) {
        std::vector<std::vector<int>> allAssignments;
        std::vector<int> bestAssignment(graph.size(), 0); // initalize with zeroes for the first assignments
        for(int i=0; i <= k; i++) {
            // Populate with at least one node of each group - first combination.
            bestAssignment.at(i) = i;
        }

        generateAllAssignments(graph.size(), k, 0, bestAssignment, allAssignments);
        if(!quietMode) {
//            std::cout << "There are " << allAssignments.size() << " possible assignemnts.\n";
            // This is redundant, but confirms that my stirling's implementation <<might> be correct.
        }

        bestAssignment = findBestAssignment(graph, allAssignments, quietMode);
        std::vector<std::string> outcome;
        outcome.push_back(Algo::vectorToString(bestAssignment));
        outcome.push_back(std::to_string(getAssignmentValue(graph, bestAssignment)));
        return outcome;
    }

    static void generateAllAssignments(int n, int k, int idx, std::vector<int>& currentAssignment, std::vector<std::vector<int>>& result) {
        // Function to generate all possible assignments
        // n - total number of nodes
        // k - required number of partitions
        // idx - current InDeX of the node being assigned to partition
        // currentAssignment - vector of a current assignment
        // result - a vector (list) of all generated assignments.
        if (idx == n) {
            std::vector<int> count(k, 0);
            for (int i : currentAssignment) {
                count[i]++;
            }
            for (int i = 0; i < k; ++i) {
                if (count[i] == 0) {
                    return; // At least one partition is empty, invalid
                }
            }
            result.push_back(currentAssignment);
            return;
        }
        for (int i = 0; i < k; ++i) {
            currentAssignment[idx] = i;
            generateAllAssignments(n, k, idx + 1, currentAssignment, result);
        }
    }

    // Method to review all assignments and choose the best one
    static std::vector<int> findBestAssignment(const std::vector<std::vector<int>>& graph, const std::vector<std::vector<int>>& assignments, bool quietMode) {
        quietMode = true; // I could intro 'verbose' param, but for now, full logs is too much.
        int minCost = std::numeric_limits<int>::max();
        std::vector<int> bestAssignment;

        for (const auto& assignment : assignments) {
            int currentCost = getAssignmentValue(graph, assignment);
            if(!quietMode) {
                std::cout << "\nFor assignment:";
                mhe::printAssignment(assignment, false, false);
                std::cout << " the cost value is: " << currentCost;
            }
            if (currentCost < minCost) {
                if(!quietMode) {
//                    std::cout << " which is LESS than current minimum!";
                }
                minCost = currentCost;
                bestAssignment = assignment;
            } else {
                if(!quietMode) {
//                    std::cout << " which is more (or the same) than current minimum...";
                }
            }
        }

        return bestAssignment;
    }
};


#endif //MHE_BRUTEFORCE_H
