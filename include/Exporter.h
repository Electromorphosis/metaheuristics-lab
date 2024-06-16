//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_EXPORTER_H
#define MHE_EXPORTER_H
#include <iostream>
#include <fstream>
#include <vector>

class Exporter {
public:
    static void exportToCSV(const std::vector<std::vector<int>>& data, const std::string& filename) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                // Add comma if it's not the last element in the row
                if (i != row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n"; // End of row
        }

        file.close();
        std::cout << "CSV file exported successfully: " << filename << std::endl;
    }

};
#endif //MHE_EXPORTER_H
