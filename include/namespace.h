//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_NAMESPACE_H
#define MHE_NAMESPACE_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

namespace mhe {
    using std::cout;
    using std::vector;
    const int NULL_WEIGHT = 0; // Value representing no edge
    int MIN_WEIGHT = 1; // Minimum weight of the edges
    int MAX_WEIGHT = 9; // Maximum weight of the edges
    int SIZE = 9; // size of the matrix; sizes above 9 might not be fully well presented by visualizers
    std::random_device rd;
    std::mt19937 mt(rd());


    int getRandomWeight() {
        std::uniform_int_distribution<int> distr(MIN_WEIGHT, MAX_WEIGHT);
        return distr(mt);
    }

    std::vector<std::vector<int>> generateRandomGraphAdjacencyMatrix() {
        std::cout << "\nGenerating graph with parameters:\n\tSize: " << SIZE << "\n\tMinWeight: " << MIN_WEIGHT << "\n\tMaxWeight: " << MAX_WEIGHT << "\n\n";
        std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, NULL_WEIGHT));

        for (int i = 0; i < SIZE; ++i) {
            for (int j = i + 1; j < SIZE; ++j) {
                int weight = getRandomWeight();
                matrix[i][j] = weight;
                matrix[j][i] = weight; // Ensure the matrix is symmetrical
            }
        }

        return matrix;
    }

    std::vector<int> generateNeighbourSolution(const std::vector<int>& inputSolution, int k) {
        std::vector<int> result = inputSolution;
        // In a most "minimal" way possible - flip one random vertice to the next (in a sense of modular arithmetic...)
        // partition
        int nodesNum = inputSolution.size();
        std::uniform_int_distribution<int> distrNodeRange(0, nodesNum - 1);
        int randomNodeNum = distrNodeRange(mt);
        result.at(randomNodeNum) = (result.at(randomNodeNum) + 1);
        if (result.at(randomNodeNum) > k) result.at(randomNodeNum) = 0;
        return result;
    }


    // Function to print the adjacency matrix
    void printAdjMatrix(const std::vector<std::vector<int>>& matrix) {
        for (const auto& row : matrix) {
            for (const auto& val : row) {
                std::cout << val << " ";
                if (val < 10) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    // Function to print the adjacency matrix
    void printAdjMatrixRow(const std::vector<std::vector<int>>& matrix, int rowNum) {
        for (const auto& val : matrix.at(rowNum)) {
            std::cout << val << "   ";
            if (val < 10) std::cout << " ";
        }
        std::cout << std::endl;
    }

    void printPrettyMatrix(const std::vector<std::vector<int>>& adjMatrix) {
        // Nicer method for printing matrix values in a visual table

        // Write horizontal
        std::cout << "\n  | ";

        for (int i = 0; i < SIZE; i++) {
            std::cout << i << "   ";
            if(i < 10) std::cout << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < SIZE+1; i++) {
            std::cout << "----";
            if(i%2 == 0) std::cout << "-";
            if(i > 12) std::cout << "-";
        }

        std::cout << "\n";

        // Write vertical
        for (int i = 0; i < SIZE; i++) {
            if (i < 10) {
                std::cout << i << " | ";
            } else {
                std::cout << i << "| ";
            }
            printAdjMatrixRow(adjMatrix, i);
        }
    }

    std::vector<int> randomAssign(const std::vector<std::vector<int>>&inputGraph, int k) {
        std::vector<int> nodeAssginment;
        // This method assigns randomly each node in the graph to one of the partitions (i.e. making random cut).
        if(k < 0) {
            std::cerr << "\nProvided k value must be positive!";
            return nodeAssginment;
        }

        std::uniform_int_distribution<int> distrPartRange(0, k);
        for (int i = 0; i < inputGraph.size(); i++) {
            nodeAssginment.push_back(distrPartRange(mt));
        }

        return nodeAssginment;
    }

    void printAssignment(const std::vector<int>&assignment, bool dict = false, bool fullPrint = false) {
        if (fullPrint) {
            cout << "\nAssignment ";
        }
        cout << "= { ";
            for (int i = 0; i < assignment.size(); i++) {
                if(assignment.size() != i+1) {
                    if (dict) {
                        cout << i << ":" << assignment.at(i) << ", ";
                    }
                    cout << assignment.at(i) << ", ";
                } else {
                    cout << assignment.at(i);
                }
            }
            cout << " }";
    }

    void compareAssignments(vector<int> firstAssignment, vector<int> secondAssignment) {
        if (firstAssignment.size() != secondAssignment.size()) {
            cout << "\nAssignments are not the same size! First one is " << firstAssignment.size() << ", second one is " << secondAssignment.size() << ".";
            return;
        }
        int sumOfDifferences = 0;
        int proximity = 0;

        for (int i = 0; i < firstAssignment.size(); i++) {
            if(firstAssignment.at(i) != secondAssignment.at(i)) {
                cout << "\nDifference occur in the affiliation of the node number " << i << " (" << firstAssignment.at(i) << ", " << secondAssignment.at(i) << " )";
                sumOfDifferences++;
                proximity += abs(firstAssignment.at(i)-secondAssignment.at(i));
            }
        }

        if (sumOfDifferences == 0) {
            cout << "\nBoth assignments are the same assignment!";
        } else {
            cout << "\nThere are " << sumOfDifferences << " difference(s) between both assignments.";
            cout << "\nThe value of their Proximity score is: " << proximity;
        }
    }
}
#endif //MHE_NAMESPACE_H
