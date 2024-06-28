//
// Created by mechlon on 16.06.24.
//
#include <algorithm>
#include "include/namespace.h"
#include "include/Exporter.h"
#include "include/Importer.h"
#include "include/ObjectiveFunction.h"

#include "algo/BruteForce.h"
#include "algo/Climbing.h"

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
    // Parse args
    if (!parseArguments(argc, argv, SIZE, MIN_WEIGHT, MAX_WEIGHT) || SIZE <= 0 || MIN_WEIGHT > MAX_WEIGHT) {
        cerr << "Usage: " << argv[0] << " --size=<size> --min_weight=<min_weight> --max_weight=<max_weight>" << std::endl;
        cerr << "Ensure size is positive and min_weight <= max_weight. Use only integers." << std::endl;
        return 1;
    }

    // Prepare new report file
    std::time_t result = std::time(nullptr);
    std::string timestamp = std::asctime(std::localtime(&result));
    std::replace(timestamp.begin(), timestamp.end(), ' ', '-');
    std::string filename = "reports/MHE_raport_pmechlinski_" + timestamp + ".md";
    Exporter::createNewFile(filename);

//    // Generate random weighed graph in a form of Adjacency Matrix
//    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
//    vector<vector<int>> adjMatrix = generateRandomGraphAdjacencyMatrix();
// Commented: Will use predefined graph to provide consistency during the tests

//    // Visualize initial results
//    // printPrettyMatrix(adjMatrix);
//    cout << "Graph snippet:\n";
//    printAdjMatrixRow(adjMatrix, 0);
//    printAdjMatrixRow(adjMatrix, 1);
//    cout << "...\n";


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


//    cout << "Comprehensible (manually calculable) example:\n\t5 nodes\n\t2 partitions\n";
//    k = 2;
//    MIN_WEIGHT = 1;
//    MAX_WEIGHT = 9;
//    SIZE = 3;
//    vector<vector<int>> smallMatrix = generateRandomGraphAdjacencyMatrix();

        vector<vector<int>> adjMatrix = Importer::readCSVFileTo2DVector("test/data/exampleMatrixBig.csv");
SIZE = 50;
k = 3;
//    printPrettyMatrix(adjMatrix);
    Exporter::writeNewline("# Metaheuristics Report\nAuthor Paweł Mechliński\n## Randomly generated matrix:\n```", filename);
    Exporter::writeNewline("Matrix has 50 nodes and we'll be looking to split it into 3 particles. In experiments with all algorithms, the same matrix and conditions will be utilized.", filename);

    Exporter::writeNewline(stringPrettyMatrix(adjMatrix), filename);
    Exporter::writeNewline("\n```", filename);
    vector<int> bigMatrixRandomAssignment = Algo::randomAssign(adjMatrix, k);

    // Test of generation of all possible assignments
    std::vector<int> bestAssignment(SIZE); // initalize with zeroes for the first assignments
    vector<vector<int>> allAssignments; // Vector of int-vector, will be used to store all possible combinations of assigments
//    cout << "\n[TEST] All possible assignments from generateAllAssignments:\n";
//    BruteForce::generateAllAssignments(SIZE, k, 0, bestAssignment, allAssignments); // This method will populate 2D assignments vector; bestAssignment variable is used only as a starting point.



//    vector<int> bruteForceBestAssignment = BruteForce::calculate(adjMatrix, k);
    // At this graph volume BruteForce is practically not possible to be run on my machine; it chokes even when trying to populate the vector of all possible assignments
    // which are - if my maths is correct - 1.2676506e+30 , so more than decillion of different combinations.
    // to make it work with big graphs (it worked for small ones previously...) I might need to write it line-by-line to the file... and then verify, also line-by-line.

    Exporter::writeNewline("\nBruteForce is practically impossible to run for such a big graph.", filename);

    Exporter::writeNewline("\nNext to test is climbing algorithm.", filename);

    vector<int> climbingAlgoBestAssignment = Climbing::calculate(adjMatrix, k);
    printAssignment(climbingAlgoBestAssignment);
    std::cout << "\nValue: " << Algo::getAssignmentValue(adjMatrix,climbingAlgoBestAssignment);

}