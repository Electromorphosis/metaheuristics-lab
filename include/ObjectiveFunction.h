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
        return sum;
    };
};

#endif //MHE_OBJECTIVEFUNCTION_H
