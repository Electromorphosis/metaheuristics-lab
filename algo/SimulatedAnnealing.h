//
// Created by mechlon on 19.06.24.
//

#ifndef MHE_SIMULATEDANNEALING_H
#define MHE_SIMULATEDANNEALING_H
#include <vector>
#include <random>
#include <limits>
#include "Algo.h"
#include "../include/namespace.h"

class SimulatedAnnealing : public Algo{
public:
    static std::vector<int> calculate(const std::vector<std::vector<int>>& graph, int k, int maxIterations, double initialTemperature, double coolingRate) {
        int n = graph.size();
        std::vector<int> currentAssignment(n, 0);
        std::vector<int> bestAssignment = currentAssignment;
        for (int i = 0; i <= k; i++) {
            // Populate with at least one node of each group - first combination.
            bestAssignment.at(i) = i;
        }

        int bestCost = getAssignmentValue(graph, currentAssignment);

        double currentTemperature = initialTemperature;

        for (int i = 0; i < maxIterations; ++i) {
            // Generate a random neighbor
            std::vector<int> neighbor = generateRandomNeighbor(currentAssignment);

            // Calculate neighbor cost
            int neighborCost = getAssignmentValue(graph, neighbor);

            // Decide whether to accept the neighbor
            double deltaCost = neighborCost - bestCost;
            if (deltaCost < 0 || acceptNeighbor(deltaCost, currentTemperature)) {
                currentAssignment = neighbor;
                if (neighborCost < bestCost) {
                    bestAssignment = neighbor;
                    bestCost = neighborCost;
                }
            }

            // Update temperature
            currentTemperature *= coolingRate;
        }

        return bestAssignment;
    }

private:
    static std::vector<int> generateRandomNeighbor(const std::vector<int>& assignment) {
        int n = assignment.size();

        // Select two random nodes to swap their partitions
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        int node1 = dis(gen);
        int node2 = dis(gen);
        while (node1 == node2) { // Ensure distinct nodes are chosen
            node2 = dis(gen);
        }

        // Create a copy of the current assignment
        std::vector<int> neighbor(assignment);

        // Swap the partitions of the two randomly chosen nodes
        int temp = neighbor[node1];
        neighbor[node1] = neighbor[node2];
        neighbor[node2] = temp;

        return neighbor;
    }



    // Function to decide whether to accept a neighbor based on cost difference and temperature
    static bool acceptNeighbor(double deltaCost, double temperature) {
        if (deltaCost < 0) { // Always accept improvement
            return true;
        }

        // Use Metropolis acceptance criterion
        std::uniform_real_distribution<> dis(0.0, 1.0);
        double probability = std::exp(-deltaCost / temperature);
        return dis(mhe::mt) < probability;
    }
};

#endif //MHE_SIMULATEDANNEALING_H
