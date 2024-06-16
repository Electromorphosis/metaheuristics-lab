//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_IMPORTER_H
#define MHE_IMPORTER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Importer {
public:
    Importer();

    std::vector<std::vector<int>> readCSVFileTo2DVector(const std::string& filepath) {
        std::vector<std::vector<int>> data;

        // Open the CSV file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filepath << std::endl;
            return data;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            std::string token;

            while (std::getline(iss, token, ',')) {
                // Convert the token to integer and add to the row vector
                row.push_back(std::stoi(token));
            }

            // Add the row vector to the data vector
            data.push_back(row);
        }

        // Close the file
        file.close();

        return data;
    }

};

#endif //MHE_IMPORTER_H
