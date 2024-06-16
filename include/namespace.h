//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_NAMESPACE_H
#define MHE_NAMESPACE_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace mhe {

    const int NULL_WEIGHT = 0; // Value representing no edge
    int MIN_WEIGHT = 1; // Minimum weight of the edges
    int MAX_WEIGHT = 9; // Maximum weight of the edges
    int SIZE = 9; // size of the matrix; sizes above 9 might not be fully well presented by visualizers



    int getRandomWeight() {
        return MIN_WEIGHT + rand() % (MAX_WEIGHT - MIN_WEIGHT + 1);
    }

    std::vector<std::vector<int>> generateRandomGraphAdjacencyMatrix() {
        std::cout << "Generating graph with parameters:\nSize: " << SIZE << "\nMinWeight: " << MIN_WEIGHT << "\nMaxWeight: " << MAX_WEIGHT << "\n";
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
        std::cout << " | ";

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
            std::cout << i << "| ";
            printAdjMatrixRow(adjMatrix, i);
        }
    }
}
#endif //MHE_NAMESPACE_H
