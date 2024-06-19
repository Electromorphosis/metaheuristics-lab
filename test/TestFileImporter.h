//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_TESTFILEIMPORTER_H
#define MHE_TESTFILEIMPORTER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/namespace.h"
#include "../include/Importer.h"

//#include <unistd.h>
//#include <climits>

class TestFileImporter {
public:
    static int testImportCsvToAdjMatrix()  {
//        char buffer[PATH_MAX];
//        if (getcwd(buffer, sizeof(buffer)) != NULL) {
//            std::cout << "Current path: " << buffer << std::endl;
//        } else {
//            perror("getcwd() error");
//            return 1;
//        }
        std::string filepath = "test/data/exampleMatrix.csv";

    // Read the CSV file into a 2D vector
        std::vector<std::vector<int>> matrix = Importer::readCSVFileTo2DVector(filepath);
        if(matrix.empty()) {
            std::cerr << "Empty matrix returned!";
            return 1;
        }

        std::cout << "testImportCsvToAdjMatrix succeeded!\n";
        // Print the matrix (for verification)
        std::cout << "\n\tResult:\n";
        mhe::printPrettyMatrix(matrix);
        return 0;
    }
};


#endif //MHE_TESTFILEIMPORTER_H
