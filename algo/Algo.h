//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_ALGO_H
#define MHE_ALGO_H
#include <vector>

class Algo {
public:
    static std::vector<int> checkIfValidInput(std::vector<std::vector<int>> graph, int k) {
        if (graph.size() < k || k < 1) {
            std::cerr << "\nThere are fewer nodes than partitions expected n=" << graph.size() << " vs. k=" << k << "). No valid assignment cannot be created with those requirements!";
            return std::vector<int>(graph.size(), -1);
        } else {
            return std::vector<int>(graph.size(), 0);
        }

    }

};


#endif //MHE_ALGO_H
