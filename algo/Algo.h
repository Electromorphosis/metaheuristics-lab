//
// Created by mechlon on 16.06.24.
//

#ifndef MHE_ALGO_H
#define MHE_ALGO_H
#include <vector>
#include "../include/ObjectiveFunction.h"

class Algo {
public:
    static std::vector<int> checkIfValidInput(const std::vector<std::vector<int>>& graph, int k) {
        if (graph.size() < k || k < 1) {
            std::cerr << "\nThere are fewer nodes than partitions expected n=" << graph.size() << " vs. k=" << k << "). No valid assignment cannot be created with those requirements!";
            return std::vector<int>(graph.size(), -1);
        } else {
            return std::vector<int>(graph.size(), 0);
        }

    }

    static bool isValidAssignment(const std::vector<int>& vec, int k) {
        std::vector<int> count(k + 1, 0);

        // Mark numbers from vec that are within range 0 to k
        for (int num : vec) {
            if (num >= 0 && num <= k) {
                count[num]++;
            }
        }

        // Check if all counts are at least 1
        for (int i = 0; i <= k; ++i) {
            if (count[i] == 0) {
                return false;
            }
        }

        return true;
    }

    // Wrapper to get cost value when I don't care about visualising all the middle steps, i.e. in algos that is.
    static int getAssignmentValue(const std::vector<std::vector<int>>& graph, const std::vector<int>& assignment) {
        std::vector<std::vector<int>> costMatrix = ObjectiveFunction::calculateCostMatrix(graph, assignment);
        int costValue = ObjectiveFunction::getCostValue(costMatrix);
        return costValue;
    }

    static std::vector<int> randomAssign(const std::vector<std::vector<int>>&inputGraph, int k) {
        std::vector<int> nodeAssginment;
        // This method assigns randomly each node in the graph to one of the partitions (i.e. making random cut).
        if(k < 0) {
            std::cerr << "\nProvided k value must be positive!";
            return nodeAssginment;
        }

        std::uniform_int_distribution<int> distrPartRange(0, k-1);
        for (int i = 0; i < inputGraph.size(); i++) {
            nodeAssginment.push_back(distrPartRange(mhe::mt));
        }

        return nodeAssginment;
    }

    // Function to compute factorial
    static unsigned long long factorial(int n) {
        unsigned long long result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

// Function to compute Stirling numbers of the second kind - not 100% sure if it's correct though...
    static unsigned long long stirlingNumber(int n, int k) {
        std::vector<std::vector<unsigned long long>> dp(n + 1, std::vector<unsigned long long>(k + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= k; ++j) {
                dp[i][j] = j * dp[i - 1][j] + dp[i - 1][j - 1];
            }
        }

        return dp[n][k];
    }

// Function to compute the total number of ways to partition n nodes into k partitions
    static unsigned long long partitionWays(int n, int k) {
        if (k == 0 || k > n) {
            return 0; // No way to partition if k is 0 or greater than n
        }

        unsigned long long stirling = stirlingNumber(n, k);
        unsigned long long permutations = factorial(k);

        return permutations * stirling;
    }

    static std::string vectorToString(const std::vector<int>& vec) {
        std::ostringstream oss;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i != 0) {
                oss << ", ";
            }
            oss << vec[i];
        }
        return oss.str();
    }

    static std::string vectorToString(const std::vector<std::string>& vec) {
        std::ostringstream oss;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i != 0) {
                oss << "; ";
            }
            oss << vec[i];
        }
        return oss.str();
    }

    static std::pair<std::string, int> parseKeyValue(const std::string& input) {
        // Find the position of '='
        size_t pos = input.find('=');
        if (pos == std::string::npos) {
            throw std::invalid_argument("Invalid input format. '=' not found.");
        }

        // Extract key and value
        std::string key = input.substr(0, pos);
        std::string valueStr = input.substr(pos + 1);

        // Convert value to integer
        int value;
        std::istringstream iss(valueStr);
        if (!(iss >> value)) {
            throw std::invalid_argument("Failed to convert value to integer.");
        }

        return {key, value};
    }
};


#endif //MHE_ALGO_H
