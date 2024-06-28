#include <string>
#include <iostream>

#include "include/help.h"
#include "include/commands.h"

int main(int argc, char* argv[]) {
    // Main ought to be in its final form now - don't modify!
    using namespace commands;
    using namespace help;
    using std::string;
    using std::cout;

    if (argc < 2) {
        // Program has to have at least one argument!
        cout << default_message;
        return 1;
    }

    // Get the first argument (command); check if valid & throw error if not.
    string command = argv[1];
    auto it = program_commands.find(command);
    string message = it != program_commands.end() ? it->second : default_message;

    if (command == "help") {
        cout << help_general;
        return 0;
    } else if (command == "run") {
        run(argc, argv);
        return 0;
    } else if (command == "generate") {
        generate(argc, argv);
        return 0;
    } else {
        printUnknownCommandMessage();
        return 1;
    }
}

