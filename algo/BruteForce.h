//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_BRUTEFORCE_H
#define MHE_BRUTEFORCE_H
#include <vector>
#include <cmath>
#include <iostream>
#include "Algo.h"

class BruteForce : public Algo{
public:
    static std::vector<int> calculate(const std::vector<std::vector<int>>& graph,const int k) {
        std::vector<int> check = checkIfValidInput(graph, k);
        if (check.at(0) == -1) {
            return check;
        }
        int numberOfPossibleAssignments = pow(k+1, static_cast<int>(graph.size())); // bigger k because arithmetic number of partitions, not the index
        std::cout << "\nBrute Force (Full Review) algorithm starts...\nThere are (" << k+1 << "^" << graph.size() << "=)" << numberOfPossibleAssignments << " possible assignments to review. Take your time :-).";

        std::vector<int> bestAssignment(graph.size(), 0); // initalize with zeroes
        for(int i=0; i <= k; i++) {
            // Populate with at least one node of each group - first combination.
            bestAssignment.at(i) = i;
        }



        return bestAssignment;
    }
private:
    static std::vector<int> iterateAssignments(const std::vector<int> initialAssignment) {

    }
};


#endif //MHE_BRUTEFORCE_H
