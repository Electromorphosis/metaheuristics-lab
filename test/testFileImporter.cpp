//
// Created by mechlon on 16.06.24.
//

#include "../include/Importer.h"


int main() {
    std::string filepath = "../data/exampleMatrix.csv";

    // Read the CSV file into a 2D vector
    std::vector<std::vector<int>> matrix = readCSVFileTo2DVector(filepath);

    // Print the matrix (for verification)
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


