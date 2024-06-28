//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_EXPORTER_H
#define MHE_EXPORTER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

    // TODO: exporter to DOT is WIP as it's not a core functionality for the classroom project
//    static void exportAdjMatrixToDOT(const std::vector<std::vector<int>>& inputAdjacencyMatrix, const std::string& filepath) {
//        if (filepath.empty() || filepath == "/") {
//            std::cerr << "Filepath provided is not valid!";
//            return;
//        }
//        createNewFile(filepath);
//
//        // Header
//        writeNewline( "graph G {", filepath);
//        // Nodes
//        for (int i = 0; i < inputAdjacencyMatrix.size(); i++) {
//            std::string i_string = std::to_string(i);
//            std::string content = i_string;
//            content.append("[label=\"Node ");
//            content.append(i_string);
//            content.append("\"]");
//            writeNewline( content, filepath);
//        }
//        // Edges
//        // Footer
//    }

    // Function to create a new file
    static void createNewFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::out); // Create a new file (overwrite if exists)
        if (!file) {
            std::cerr << "Error creating file: " << filename << std::endl;
            return;
        }
        file.close();
    }


    // Function to write a newline to the file
    static void writeNewline(const std::string& content, const std::string& filename) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        file << std::endl << content;
        file.close();
    }

};
#endif //MHE_EXPORTER_H
