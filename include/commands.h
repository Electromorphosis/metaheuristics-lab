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

#include "help.h"
#include "namespace.h"
#include "Exporter.h"

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
            if (arg == "-i" || arg == "-t" || arg == "-p" || arg == "-o") {
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
                {"-q", "Quiet"}
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
        if (minimumWeight < 1) {
            std::cerr << "Minimum weight must be a positive, non-zero number!";
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

        coutIf(quietMode, "Parsing completed. Input validation starts...\n\n");

        if(!isExistingFile(params["Input"])) {
            std::cerr << "[ERROR] Provided input file does not exist! Exiting...\n"; // Ofc quiet mode is not related to error logging...
            return;
        }

        // Import input file
        // Validate if correct graph
        // (Optional) Print basic graph stats



        if(params["AlgoType"] == "full_review") {
            // vector<string> outcome = runFullReview(importedGraph, quietMode);
            // ifPrint(outcome);
            // ifAppend(outcome);
            // return;
        } else if(params["AlgoType"] == "climbing") {
            vector <string> climbingParams = splitString(params["AlgoParams"]);
            string neighStrategy = climbingParams.at(0);
            // Validate neighStrat val
            string terminationStrategy = climbingParams.at(1);
            // Validate
            // split into map<string, int> termStratParsed
            // vector<string> outcome = runClimbingAlgorithm(importedGraph, quietMode, neighStrat, termStratParsed.key, termStratParsed.value);
            // ifPrint(outcome);
            // ifAppend(outcome);
            // return;
        } else if(params["AlgoType"] == "genetic") {
            vector <string> geneticParams = splitString(params["AlgoParams"]);
            int crossoverStrat = stoi(geneticParams.at(0)); // TODO: This should be throw-catched
            // validate mutation
            // for termination validate, then split into string-int map, then validate again
            // map<string, int> termStratParsed
            // vector<string> outcome = runGeneticAlgorithm(importedGraph, quietMode, crossoverStrat, mutationStrat, termStratParsed.key, termStratParsed.value);
            // ifPrint(outcome);
            // ifAppend(outcome);
            // return;
        }
    }
}
#endif //MHE_COMMANDS_H
