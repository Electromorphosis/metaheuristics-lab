//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_OBJECTIVEFUNCTION_H
#define MHE_OBJECTIVEFUNCTION_H
#include <vector>

class ObjectiveFunction {
public:
    static int getCostValue(std::vector<std::vector<int>> cutVectorWeighed) {
        // getCostValue requires as an input actual value transformation matrix (not the cut binary matrix!);
        // please keep in mind also that it doesn't make any assumptions on the data validity - this should
        // be implemented in a separate validator ideally...
        int sum = 0;
        for (const auto &row: cutVectorWeighed) {
            for (int value: row) {
                sum += value;
            }
        }
        return sum / 2;
    };

    static std::vector<std::vector<int>> calculateCostMatrix(const std::vector<std::vector<int>>& weighedGraph, const std::vector<int>& assignment) {
//        std::cout << "\n[INFO] Calculating Cost Matrix...\n\n";
        int size = weighedGraph.size();
        std::vector<std::vector<int>> calculatedCostMatrix(size, std::vector<int>(size, 0));

        for (int i = 0; i < size; ++i) {
            int currentNodeAssignment = assignment[i]; // Each row represent one node, always belonging to only one partition.
            for (int j = 0; j < size; ++j) {
                if (weighedGraph[i][j] != 0 && assignment[j] != currentNodeAssignment) {
                    // If a second node in pair is assigned to a different group, then the connection's value is added to cost matrix.
                    calculatedCostMatrix[i][j] = weighedGraph[i][j];
                }
            }
        }

        return calculatedCostMatrix;
    };
};

#endif //MHE_OBJECTIVEFUNCTION_H
