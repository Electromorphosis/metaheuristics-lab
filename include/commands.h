//
// Created by mechlon on 28.06.24.
//

#ifndef MHE_COMMANDS_H
#define MHE_COMMANDS_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <filesystem>
#include <sstream>
#include <algorithm>

#include "help.h"
#include "namespace.h"
#include "Exporter.h"
#include "Importer.h"
#include "../algo/BruteForce.h"
#include "../algo/Climbing.h"
#include "../algo/Genetic.h"

namespace commands {
    using std::string;
    using std::cout;
    using std::cerr;
    using namespace help;
    using namespace mhe;
    namespace fs = std::filesystem;

//    bool isValidFilePath(const string& path) {
//        // (Works for Linux only)
//        std::regex pathRegex("^/([a-zA-Z0-9_\\-]+/?)+$");
//        if (!std::regex_match(path, pathRegex)) {
//            return false; // Path format is invalid
//        }
//        // Extract the directory path without the filename
//        fs::path dirPath = fs::path(path).parent_path();
//        return fs::exists(dirPath) && fs::is_directory(dirPath);
//    }
// TODO: Fix or dump

    void coutIf(bool quiet, const std::string& s) {
        if(!quiet) {
            cout << s;
        }
    }

    bool isExistingFile(const std::string& path) {
        fs::path filePath(path);

        // Check if the file exists and is a regular file
        return fs::exists(filePath) && fs::is_regular_file(filePath);
    }

    bool isValidMatrixSize(const vector<vector<int>> testedMatrix) {
        size_t matrixSize = testedMatrix.size();
        if (matrixSize == 0) {
            cerr << "Matrix is empty!\n";
            return false;
        }

        return std::all_of(testedMatrix.begin(), testedMatrix.end(), [matrixSize](const std::vector<int>& row) {
            return row.size() == matrixSize;
        });
    }

    bool isMatrixSymmetrical(const std::vector<std::vector<int>>& testedMatrix) {
        // Check if the matrix is square - probably redundant, but better safe than sorry
        size_t numRows = testedMatrix.size();
        size_t numCols = testedMatrix[0].size();
        if (numRows != numCols) return false;

        // Check symmetry and diagonal zeros
        for (size_t i = 0; i < numRows; ++i) {
            for (size_t j = 0; j < numCols; ++j) {
                if (i == j && testedMatrix[i][j] != 0) {
                    return false; // Diagonal elements must be zero
                }
                if (testedMatrix[i][j] != testedMatrix[j][i]) {
                    return false; // Symmetry condition
                }
            }
        }

        return true;
    }

    bool allNonNegative(const std::vector<std::vector<int>>& matrix) {
        return std::all_of(matrix.begin(), matrix.end(), [](const std::vector<int>& row) {
            return std::all_of(row.begin(), row.end(), [](int value) {
                return value >= 0;
            });
        });
    }

    std::vector<std::string> splitString(const std::string& input) {
        // Split comma-separated parameters into value vector
        std::vector<std::string> result;
        std::istringstream iss(input);
        std::string token;

        while (std::getline(iss, token, ',')) {
            if (token.empty()) {
                result.push_back("default");
            } else {
                result.push_back(token);
            }
        }

        return result;
    }

    std::map<string, string> parseRunParams(int argc, char* argv[]) {
        // Check if help is needed first
        string command = argv[2];
        if (command == "help") {
            cout << help_run;
            return {};
        }

        // Handle core args & params
        std::map<std::string, std::string> params = {
                {"-i", "None"}, // Input file, required, needs value
                {"-t", "None"}, // Algo type, required, needs value
                {"-p", "None"}, // Algo params, required, needs value
                {"-o", "false"}, // Output, optional, needs value
                {"-k", "2"}, // Number of partitions, optional, default: 2
                {"-v", "false"}, // Validation, optional, no value, default: false
                {"-q", "false"} // Quiet mode, optional, no value, default: false
        };

        // Check for min number of parameters needed
        if (argc < 7) {
            std::cerr << "Error: Missing arguments for 'run' command." << std::endl;
            return {};
        }

        // Process arguments procedurally
        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];
            // Check if argument with value has one
            if (arg == "-i" || arg == "-t" || arg == "-p" || arg == "-o" || arg == "-k") {
                if (i + 1 >= argc) {
                    std::cerr << "Error: Missing value after " << arg << std::endl;
                    return {};
                }

                params[arg] = argv[i + 1];
                i++; // Skip the assigned value
            } else if (arg == "-v" || arg == "-q") {
                params[arg] = "true";
            }
        }

        // Check for missing required parameters
        bool all_required_found = true;
        if(params["-p"] == "None" && params["-t"] == "full_review") {
            params["-p"] = "Default";
        }

        for (const auto& [key, value] : params) {
            if (value == "None") {
                all_required_found = false;
                break;
            }
        }
        std::map<string, string> taggedParams;
        std::unordered_map<std::string, std::string> keyToTag = {
                {"-i", "Input"},
                {"-t", "AlgoType"},
                {"-p", "AlgoParams"},
                {"-o", "Output"},
                {"-v", "Validation"},
                {"-q", "Quiet"},
                {"-k", "Partitions"}
        };

        for (const auto& [key, value] : params) {
            taggedParams[keyToTag[key]] = value;
        }

        // Print parameters
        if(taggedParams["Quiet"] == "false") { // redundant, no better idea for now here
            std::cout << "Parameters:" << std::endl;
            for (const auto& [key, value] : taggedParams) {
                std::cout << "  " << key << ": " << value << std::endl;
            }
        }

        if (!all_required_found) {
            std::cerr << "Error: Missing required parameters." << std::endl;
            return {};
        }

        return taggedParams;
    }

    std::map<string, string> parseGenParams(int argc, char* argv[]) {
        // Check if help is needed first
        string command = argv[2];
        if (command == "help") {
            cout << help_generate;
            return {};
        }

        // Handle core args & params
        std::map<std::string, std::string> params = {
                {"-o", "None"}, // Output file, required, needs value
                {"-n", "10"}, // Node number, optional, needs value, default: 10
                {"-w", "1,10"}, // Weight range, optional, needs value, default: 1-10
        };

        // Check for min number of parameters needed
        if (argc < 3) {
            std::cerr << "Error: Missing arguments for 'generate' command." << std::endl;
            return {};
        }

        // Process arguments procedurally
        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];
            // Check if argument with value has one
            if (arg == "-o" || arg == "-n" || arg == "-w") {
                if (i + 1 >= argc) {
                    std::cerr << "Error: Missing value after " << arg << std::endl;
                    return {};
                }

                params[arg] = argv[i + 1];
                i++; // Skip the assigned value
            }
        }

        // Check for missing required parameters
        bool all_required_found = true;
        for (const auto& [key, value] : params) {
            if (value == "None") {
                all_required_found = false;
                break;
            }
        }
        std::map<std::string, std::string> taggedParams;
        std::unordered_map<std::string, std::string> keyToTag = {
                {"-o", "Output"},
                {"-n", "Nodes"},
                {"-w", "Weights"}
        };

        for (const auto& [key, value] : params) {
            taggedParams[keyToTag[key]] = value;
        }

        // Print parameters
        std::cout << "Parameters:" << std::endl;
        for (const auto& pair : taggedParams) {
            std::cout << "\t" << pair.first << ": " << pair.second << std::endl;
        }

        if (!all_required_found) {
            std::cerr << "Error: Missing required parameters." << std::endl;
            return {};
        }
        return taggedParams;
    }
    // With enough tinkering I could probably refactor those two into one method, but as of now seems like too much hassle...

    vector<string> runFullReview(const vector<vector<int>>& graph, int k, bool quietMode, bool runValidation) {
        return BruteForce::calculate(graph, k, quietMode, runValidation);
    }

    vector<string> runClimbingAlgorithm(const vector<vector<int>>& graph, int k, bool quietMode, bool runValidation, string neighStrat, string termStrat, int termValue) {
        return Climbing::calculate(graph, k, quietMode, runValidation, neighStrat, termStrat, termValue);
    }

    vector<string>runGeneticAlgorithm(const vector<vector<int>>& graph, int k, bool quietMode, bool runValidation, int crossingPoints, const string& mutationType, const string& terminationStrategy, int terminationValue) {
        return Genetic::calculate(graph, k, quietMode, runValidation, crossingPoints, mutationType, terminationStrategy, terminationValue);
    }

    void generate(int argc, char* argv[]) {
        std::map<string, string> params = parseGenParams(argc, argv);

//        // Check if output filepath is set up correctly
//        if (!isValidFilePath(params["Output"])) {
//            std::cerr << "Filepath provided is not valid!";
//            return;
//        }

        // Handle node number param
        int nodes = 10;
        if(!(params["Nodes"] == "10")) {
            try {
                nodes = std::stoi(params["Nodes"]);
            } catch (const std::invalid_argument &ia) {
                std::cerr << "Invalid argument - number of nodes must be a numerical value, got: '" << params["Nodes"]
                          << "' as integer.\n";
                return;
            } catch (const std::out_of_range oor) {
                std::cerr << "Out of range error for number of nodes: " << oor.what() << "\n";
                return;
            }
            nodes--; // Convert from "human" value to range counted from zero
            if (nodes < 1) {
                std::cerr << "There must be at least two nodes to make a graph!";
                return;
            }
        }

        // Handle range param
        int minimumWeight = 0;
        int maximumWeight = 0;
        std::vector<string> minMaxRange = splitString(params["Weights"]);
        try {
            minimumWeight = std::stoi(minMaxRange.at(0));
        } catch (const std::invalid_argument &ia) {
            std::cerr << "Invalid argument - minimum weight must be a numerical value, got: '" << minMaxRange.at(0)
                      << "' as integer.\n";
            return;
        } catch (const std::out_of_range oor) {
            std::cerr << "Out of range error for minimum weight: " << oor.what() << "\n";
            return;
        }
        if (minimumWeight < 0) {
            std::cerr << "Minimum weight must be a positive number or zero!";
            return;
        }

        try {
            maximumWeight = std::stoi(minMaxRange.at(1));
        } catch (const std::invalid_argument &ia) {
            std::cerr << "Invalid argument - maximum weight must be a numerical value, got: '" << minMaxRange.at(1)
                      << "' as integer.\n";
            return;
        } catch (const std::out_of_range oor) {
            std::cerr << "Out of range error for maximum weight: " << oor.what() << "\n";
            return;
        }
        if (maximumWeight < 1) {
            std::cerr << "Maximum weight must be a positive, non-zero number!";
            return;
        } else if (maximumWeight < minimumWeight) {
            std::cerr << "Maximum weight must be smaller than the minimal weight!";
            return;
        }

        std::vector<std::vector<int>> randomGraph = generateRandomGraphAdjacencyMatrix(nodes, minimumWeight, maximumWeight);
        printAdjMatrix(randomGraph);
        Exporter::exportToCSV(randomGraph, params["Output"]);
    }

    void run(int argc, char* argv[]) {
        bool quietMode = false;
        bool outputToFile = false;
        bool validation = false;
        std::map<string, string> params = parseRunParams(argc, argv);
        if(params.empty()) return;
        if(params["Quiet"] == "true") quietMode = true; // this need to be checked first
        if(params["Output"] == "true") outputToFile = true; // actually this too, bc similarly I want to append some data to the output file all along
        if(params["Validation"] == "true") validation = true; // this should be also added for completion... more printing ahead...!

        coutIf(quietMode, "[INFO] Parsing completed. Input validation starts...\n");
        int k = 2;
        if(!(params["Partitions"] == "2")) {
            try {
                k = std::stoi(params["Partitions"]);
            } catch (const std::invalid_argument &ia) {
                std::cerr << "Invalid argument - number of partitions must be a numerical value, got: '" << params["Partitions"]
                          << "' as integer.\n";
                return;
            } catch (const std::out_of_range oor) {
                std::cerr << "Out of range error for number of partitions: " << oor.what() << "\n";
                return;
            }

            if (k < 1) {
                std::cerr << "There must be at least two partitions to make a calculation!";
                return;
            }
        }

        if(!isExistingFile(params["Input"])) {
            std::cerr << "[ERROR] Provided input file does not exist! Exiting...\n"; // Ofc quiet mode is not related to error logging...
            return;
        }

        vector<vector<int>> importedGraph = Importer::readCSVFileTo2DVector(params["Input"]);
        if(!isValidMatrixSize(importedGraph)) {
            cerr << "[ERROR] The matrix imported is invalid!";
            return;
        } else {
            coutIf(quietMode, "[INFO] The matrix imported is of a valid size!\n");
        }

        if(!isMatrixSymmetrical(importedGraph)) {
            cerr << "[ERROR] The matrix is not symmetrical!";
            return;
        } else {
            coutIf(quietMode, "[INFO] The matrix imported is symmetrical & have a valid diagonal values.\n");
        }

        if(importedGraph.size() < k) {
            cerr << "[ERROR] There can' be more partitions than nodes in the graph!\n";
            return;
        } else {
            coutIf(quietMode, "[INFO] There is a valid number of partitions in the input.\n");
        }

        if(!allNonNegative(importedGraph)) {
            cerr << "[WARN] The current version of the program is not optimized for negative graph weights! Unexpected behavior might occur.\n";
        }

        // TODO (Optional) Print basic graph stats

        string filename;
        if(outputToFile) {
            // If set to true, use default filename
            filename = Exporter::generateNewReportFilename();
            Exporter::createNewFile(filename);
        }
        if(params["Output"] != "false") {
            // If set to value other than false, use custom filename
            filename = params["Output"];
            outputToFile = true;
        }

        if(params["AlgoType"] == "full_review") {
            if(!quietMode) {
                cout << "\nSTARTING FULL REVIEW ALGORITHM!\n";
                cout << "There are (" << Algo::partitionWays(importedGraph.size(), k) << ") possible assignments, based on the given number of nodes (" << importedGraph.size() << ") and required partitions (" << k << ").\n";
            }
            vector<string> outcomeFullReview = runFullReview(importedGraph, k, quietMode, validation);
            coutIf(quietMode, "The full review output is: " + Algo::vectorToString(outcomeFullReview) + "\n");
            if(outputToFile) {
                Exporter::writeNewline(Algo::vectorToString(outcomeFullReview), filename);
            }
            return;
        } else if(params["AlgoType"] == "climbing") {
            coutIf(quietMode, "\nSTARTING CLIMBING ALGORITHM!\n");
            vector <string> climbingParams = splitString(params["AlgoParams"]);
            string neighStrategy = climbingParams.at(0);
            // TODO: Validate neighStrat val
            // ^ optional, for now assume it's proper
            // ^ same as above
            std::pair<string, int> termStrat = Algo::parseKeyValue(climbingParams.at(1));
            if(!quietMode) {
                cout << "Neighbour strategy: " << neighStrategy << "\n";
                cout << "Termination strategy: " << climbingParams.at(1) << "\n";
            }
            vector<string> outcome = runClimbingAlgorithm(importedGraph, k, quietMode, validation, neighStrategy, termStrat.first, termStrat.second);
            coutIf(quietMode, "The climbing algorithm output is: " + Algo::vectorToString(outcome) + "\n");
            if(outputToFile) {
                Exporter::writeNewline(Algo::vectorToString(outcome), filename);
            }
            return;
        } else if(params["AlgoType"] == "genetic") {
            coutIf(quietMode, "\nSTARTING GENETIC ALGORITHM!\n");
            // TODO - BUG, default parameters doesnt work :C
            cout << "test";
            vector <string> geneticParams = splitString(params["AlgoParams"]);
            int crossoverPoints;
            if(geneticParams.at(0).empty()) {
                crossoverPoints = 1;
            } else {
                crossoverPoints = stoi(geneticParams.at(0)); // TODO: This should be throw-catched
            }
            // TODO validate params
            string mutationStrat = geneticParams.at(1);
            std::pair<string, int> termination = Algo::parseKeyValue(geneticParams.at(2));
            if(!quietMode) {
                cout << "Crossover: " << crossoverPoints << "\n";
                cout << "Mutation: " << mutationStrat << "\n";
                cout << "Termination strategy: " << termination.first << "\n";
                cout << "Termination value: " << termination.second << "\n";
            }
            vector<string> outcome = runGeneticAlgorithm(importedGraph, k, quietMode, validation, crossoverPoints, mutationStrat, termination.first, termination.second);
            coutIf(quietMode, "The genetic algorithm output is: " + Algo::vectorToString(outcome) + "\n");
            if(outputToFile) {
                Exporter::writeNewline(Algo::vectorToString(outcome), filename);
            }
            return;
        }
    }
}
#endif //MHE_COMMANDS_H
