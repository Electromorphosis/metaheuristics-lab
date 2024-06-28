//
// Created by mechlon on 28.06.24.
//

#ifndef MHE_COMMANDS_H
#define MHE_COMMANDS_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "help.h"

namespace commands {
    using std::string;
    using std::cout;
    using std::cerr;
    using namespace help;


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
        std::cout << "Parameters:" << std::endl;
        for (const auto& [key, value] : taggedParams) {
            std::cout << "  " << key << ": " << value << std::endl;
        }

        if (!all_required_found) {
            std::cerr << "Error: Missing required parameters." << std::endl;
            return {};
        }

        return params;
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
        return params;
    }
    // With enough tinkering I could probably refactor those two into one method, but as of now seems like too much hassle...

    void run(int argc, char* argv[]) {
        std::map<string, string> params = parseRunParams(argc, argv);
    }


    void generate(int argc, char* argv[]) {
        std::map<string, string> params = parseGenParams(argc, argv);
    }
}
#endif //MHE_COMMANDS_H
