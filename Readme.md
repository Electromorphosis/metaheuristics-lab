# Metaheuristics ofr minimum k-cut problem

## Intro

**Minimum k-cut** - combinatorial optimization problem that requires finding a set of edges whose removal would partition the graph to at least k connected components. These edges are referred to as **k-cut**. The goal is to find the **minimum-weight k-cut**. This partitioning can have applications in VLSI design, data-mining, finite elements and communication in parallel computing.

The problem is NP-complete if k is part of the input. It is also NP-complete if we specify k vertices and ask for the minimum k-cut which separates these vertices among each of the sets.

## Repo structure
- **data/** - directory with test data
- **graphs.cpp** - used for graph generation and visualisation; temporarily main equivalent.
