//
// Created by mechlon on 16.06.24.
//
#include "include/namespace.h"
#include "include/Exporter.h"
#include "include/ObjectiveFunction.h"

#include "algo/BruteForce.h"

using namespace mhe;
using std::cout;
using std::cerr;
using std::vector;

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
        cerr << "Usage: " << argv[0] << " --size=<size> --min_weight=<min_weight> --max_weight=<max_weight>" << std::endl;
        cerr << "Ensure size is positive and min_weight <= max_weight. Use only integers." << std::endl;
        return 1;
    }
    SIZE = 20;
    // Generate random weighed graph in a form of Adjacency Matrix
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    vector<vector<int>> adjMatrix = generateRandomGraphAdjacencyMatrix();


    // Visualize initial results
    // printPrettyMatrix(adjMatrix);
    cout << "Graph snippet:\n";
    printAdjMatrixRow(adjMatrix, 0);
    printAdjMatrixRow(adjMatrix, 1);
    cout << "...\n";


    int k = 3;
//    cout << "============\nObjective:\n\tK number: " << k << "\n";
//    vector<int> nodeRandomAssignment = randomAssign(adjMatrix, k);
//    cout << "\nRandom assignment\t\t";
//    printAssignment(nodeRandomAssignment);
//
//    cout << "\nRandom neighbour 1\t\t";
//    vector<int> nodeRandomNeighbour = generateNeighbourSolution(nodeRandomAssignment, k);
//    printAssignment(nodeRandomNeighbour);
//    compareAssignments(nodeRandomAssignment, nodeRandomNeighbour);
//
//    cout << "\n\nRandom neighbour 1\t\t";
//    printAssignment(nodeRandomNeighbour);
//    cout << "\nNeighbour 2nd gen\t\t";
//    vector<int> nodeRandomNeighbour2 = generateNeighbourSolution(nodeRandomNeighbour, k);
//    printAssignment(nodeRandomNeighbour2);
//    compareAssignments(nodeRandomNeighbour, nodeRandomNeighbour2);

//    cout << "\n\n=============\n|Cost Matrix|\n=============\n";
//    vector<vector<int>> randomAssignmentCostMatrix = ObjectiveFunction::calculateCostMatrix(adjMatrix, nodeRandomAssignment);
//    int randomAssignmentCostValue = ObjectiveFunction::getCostValue(randomAssignmentCostMatrix);
//    printPrettyMatrix(randomAssignmentCostMatrix);

//    cout << "\nCost value for random assignment (k=" << k << ") = " << randomAssignmentCostValue;
//    k = 2;
//    vector<int> nodeRandomAssignmentK2 = randomAssign(adjMatrix, k);
//    vector<vector<int>> randomAssignmentCostMatrixK2 = ObjectiveFunction::calculateCostMatrix(adjMatrix, nodeRandomAssignmentK2);
//    int randomAssignmentCostValueK2 = ObjectiveFunction::getCostValue(randomAssignmentCostMatrixK2);
//    cout << "\nCost value for random assignment (k=" << k << ") = " << randomAssignmentCostValueK2;
//    k = 1;
//    vector<int> nodeRandomAssignmentK1 = randomAssign(adjMatrix, k);
//    vector<vector<int>> randomAssignmentCostMatrixK1 = ObjectiveFunction::calculateCostMatrix(adjMatrix, nodeRandomAssignmentK1);
//    int randomAssignmentCostValueK1 = ObjectiveFunction::getCostValue(randomAssignmentCostMatrixK1);
//    cout << "\nCost value for random assignment (k=" << k << ") = " << randomAssignmentCostValueK1;
//    k = 0;
//    vector<int> nodeRandomAssignmentK0 = randomAssign(adjMatrix, k);
//    vector<vector<int>> randomAssignmentCostMatrixK0 = ObjectiveFunction::calculateCostMatrix(adjMatrix, nodeRandomAssignmentK0);
//    int randomAssignmentCostValueK0 = ObjectiveFunction::getCostValue(randomAssignmentCostMatrixK0);
//    cout << "\nCost value for random assignment (k=" << k << ") = " << randomAssignmentCostValueK0;


    cout << "Comprehensible (manually calculable) example:\n\t5 nodes\n\t2 partitions\n";
    k = 1;
    MIN_WEIGHT = 1;
    MAX_WEIGHT = 9;
    SIZE = 5;
    vector<vector<int>> smallMatrix = generateRandomGraphAdjacencyMatrix();
    printPrettyMatrix(smallMatrix);
    vector<int> smallMatrixRandomAssignment = randomAssign(adjMatrix, k);
    // Todo: There might be a bug within random assignment algorithm - not sure if it may provide assymetrically cut graphs!? Too tired to check it now, but should do tomorrow...
//    cout << "\nRandom Assignment for small matrix ";
//    printAssignment(smallMatrixRandomAssignment);
//    vector<vector<int>> smallMatrixRandAssignmentCostMatrix = ObjectiveFunction::calculateCostMatrix(smallMatrix, smallMatrixRandomAssignment);
//    printPrettyMatrix(smallMatrixRandAssignmentCostMatrix);
//    int smallMatrixCostValue = ObjectiveFunction::getCostValue(smallMatrixRandAssignmentCostMatrix);
//    cout << "Cost value: " << smallMatrixCostValue;

    vector<int> bruteForceBestAssignment = BruteForce::calculate(smallMatrix, k);
    printAssignment(bruteForceBestAssignment, false, true);
    vector<vector<int>> smallMatrixRandAssignmentCostMatrixBF = ObjectiveFunction::calculateCostMatrix(smallMatrix, bruteForceBestAssignment);
    printPrettyMatrix(smallMatrixRandAssignmentCostMatrixBF);
    int smallMatrixCostValueBF = ObjectiveFunction::getCostValue(smallMatrixRandAssignmentCostMatrixBF);
    cout << "\nCost value (Brute Force/Full Review): " << smallMatrixCostValueBF;
}