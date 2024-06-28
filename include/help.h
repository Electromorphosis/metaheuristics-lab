//
// Created by mechlon on 28.06.24.
//

#ifndef MHE_HELP_H
#define MHE_HELP_H

#include <string>
#include <unordered_map>

namespace help {
    using std::cout;
    using std::string;

    string help_general = "This program is intended as a solution to the minimum k-cut problem, using metaheuristic-type algorithms. It contains both methods of handling the problem itself in a form of adjacency matrix of weighed, undirected graphs and implementation of a certain algorithms themself.\n\n"
                            "In addition to that, the utility provided enables generating and exporting random examples of such graphs.\n\n"
                            "Below are list of commands available:\n"
                            "\tgenerate - generates a random weighed, undirected graph which is then exported in the form of a .csv file.\n"
                            "\trun - run a given algorithm on a graph (in csv format) provided as an input.\n"
                            "\thelp - print help for the program.\n\n"
                            "To get more detailed information on each command, please use 'graph [command] help.\n";

    string help_generate = "Generates a random graph. Currently supports only weighed, undirected graphs. By default, it is exported to the .csv file.\n"
                                "Parameters:\n"
                                "\t-o [path] - required, sets up the name of the output file.\n"
                                "\t-n [node_num] - optional, defines number of nodes in the graph. The default number is 10 nodes.\n"
                                "\t-w [minimal_weight],[maximum_weight] - optional, defines value range for connection weights. Please note that in the current version only positive, non-zero values are accepted as a weight range. Default: [1,10].\n";
                                // TODO: Implement more parameters such as connection density, advanced stat params of the graph etc.

    string help_run = "Runs a given algorithm on the graph to which (relative) path is given as a parameter.\n"
                           "Parameters:\n"
                           "\t-i [path] - required, path to input graph.\n"
                           "\t-t [algo_type] - required, type of algorithm to run.\n"
                           "\t-p [algo_params] - required, parameters passed to the algortihm given.\n"
                           "\t-o [path] - optional, if added then the report from the run will be written to the text file. By default it will be only printed on stdout.\n"
                           "\t-v - optional, if added, then the full validation suite will be run for a given algorithm and input. Default: false\n"
                           "\t-q - optional, quiet mode, meaning no output will be printed in stdout.\n\n"
                           "Currently available algorithms options are:\n\n"
                           "\tfull_review - iterate through whole solution space; deterministic, can crash with relatively big graphs (e.g. 50 nodes).\n\n\t\tParams: none.\n\n"
                           "\tclimbing - use climbing algorithm.\n\n\t\tParams: [neigh],[term]\n\n"
                           "\t\t\tneigh - neighbour definition used, default is 'flip' (i.e. the most conservative definition) options are: \n\t\t\t\t'modify' - one node can be assigned to any other partition;\n\t\t\t\t'flip' - one node can be incremented or decremented by one;\n\t\t\t\t'swap' - two nodes are exchanged between partitions.\n\n"
                           "\t\t\tterm - termination criterion used, default is 100 iterations without improvement. Options:\n\t\t\t\t'absolute=[n]' - terminate after a n number total of iterations;\n\t\t\t\t'stucked=[n]' - terminate after n iterations without improvement.\n\n"
                           "\tgenetic - use genetic algorithm.\n\n\t\tParams: [crossing],[mutation],[termination]\n\n"
                           "\t\t\tcrossing - defines type of crossover, default is one-point crossover. Options:\n\t\t\t\t[n] - number of crossover points, positive, integer.\n"
                           "\t\t\tmutation - type of mutation being used, default is bit-flip. Options:\n\t\t\t\t'flip' - bit-flip one value in the coding;\n\t\t\t\t'swap' - swap places of two values.\n\n"
                           "\t\t\ttermination - type of termination being used. Options:\n\t\t\t\t'progress=[n]' - progress for n number of generations;\n\t\t\t\t'epsilon=[e]' - stop when trans-generational improvement is lower than a given epsilon value.\n\n"
                           "Examples: you need to first enter the name of a given algorithm, then provide list of parameters as a comma separated values, e.g.:\n"
                           "\n\t'graph run -i graph.csv -t genetic -p 2,flip,epsilon=3' - runs genetic algorithm on a graph stored in graph.csv, using 2-point crossover, mutation type flip and termination when trans-generational improvement is less than 3.\n"
                           "\n\t'graph run -i graph.csv -t genetic -p ,swap,progress=1000' - runs genetic algorithm on a graph stored in graph.csv, using one-point (default) crossover, mutation type 'swap' and termination after 1000 generations.\n"
                           "\n\t'graph run -i graph.csv -t genetic -p ,,epsilon=5' - runs genetic algorithm with default crossover (1-p) and mutation (bit-flip) types and stop after improvement is less than epsilon=5.\n\n";

    string default_message = "Unknown command. Use 'help' for available options.\n";

    std::unordered_map<string, string> program_commands = {
            {"help", help_general},
            {"generate help", help_generate},
            {"run help", help_run},
    };

    void printUnknownCommandMessage() {
        cout << default_message;
    }
};

#endif //MHE_HELP_H
