//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_BRUTEFORCE_H
#define MHE_BRUTEFORCE_H
#include <vector>
#include <cmath>
#include <iostream>
#include "Algo.h"
#include "../include/namespace.h"

class BruteForce : public Algo{
public:
    static std::vector<int> calculate(const std::vector<std::vector<int>>& graph,const int k) {
        std::vector<int> check = checkIfValidInput(graph, k);
        if (check.at(0) == -1) {
            return check;
        }
        int numberOfPossibleAssignments = pow(k, static_cast<int>(graph.size()))-k; // bigger k because arithmetic number of partitions, not the index
        std::cout << "\nBrute Force (Full Review) algorithm starts...\nThere are (" << k+1 << "^" << graph.size() << "-k=)" << numberOfPossibleAssignments << " possible assignments to review. Take your time :-).";

        std::vector<std::vector<int>> allAssignments;
        std::vector<int> bestAssignment(graph.size(), 0); // initalize with zeroes for the first assignments
        for(int i=0; i <= k; i++) {
            // Populate with at least one node of each group - first combination.
            bestAssignment.at(i) = i;
        }

        generateAllAssignments(graph.size(), k, 0, bestAssignment, allAssignments);

        bestAssignment = findBestAssignment(graph, allAssignments);
        return bestAssignment;
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
    static std::vector<int> findBestAssignment(const std::vector<std::vector<int>>& graph, const std::vector<std::vector<int>>& assignments) {
        int minCost = std::numeric_limits<int>::max();
        std::vector<int> bestAssignment;

        for (const auto& assignment : assignments) {
            int currentCost = getAssignmentValue(graph, assignment);
            std::cout << "\nFor assignment:";
            mhe::printAssignment(assignment, false, false);
            std::cout << " the cost value is: " << currentCost;
            if (currentCost < minCost) {
                std::cout << " which is LESS than current minimum!";
                minCost = currentCost;
                bestAssignment = assignment;
            } else {
                std::cout << " which is MORE (or the same) than current minimum...";
            }
        }

        return bestAssignment;
    }
private:

};


#endif //MHE_BRUTEFORCE_H
