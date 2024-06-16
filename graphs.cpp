//
// Created by mechlon on 16.06.24.
//
#include "include/namespace.h"
#include "include/Exporter.h"
using namespace mhe;

bool parseArguments(int argc, char* argv[], int& size, int& minWeight, int& maxWeight) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--size=") == 0) {
            SIZE = std::stoi(arg.substr(7));
        } else if (arg.find("--min_weight=") == 0) {
            MIN_WEIGHT = std::stoi(arg.substr(13));
        } else if (arg.find("--max_weight=") == 0) {
            MAX_WEIGHT = std::stoi(arg.substr(13));
        } else {
            return false; // Unknown argument
        }
    }
    return true;
}

int main(int argc, char* argv[]) {

    if (!parseArguments(argc, argv, SIZE, MIN_WEIGHT, MAX_WEIGHT) || SIZE <= 0 || MIN_WEIGHT > MAX_WEIGHT) {
        std::cerr << "Usage: " << argv[0] << " --size=<size> --min_weight=<min_weight> --max_weight=<max_weight>" << std::endl;
        std::cerr << "Ensure size is positive and min_weight <= max_weight. Use only integers." << std::endl;
        return 1;
    }
    // Generate random weighed graph in a form of Adjacency Matrix
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    std::vector<std::vector<int>> adjMatrix = generateRandomGraphAdjacencyMatrix();


//    // Visualize initial results
//    printPrettyMatrix(adjMatrix);

}