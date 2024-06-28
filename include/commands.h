//
// Created by mechlon on 28.06.24.
//

#ifndef MHE_COMMANDS_H
#define MHE_COMMANDS_H

#include <string>
#include <iostream>

#include "help.h"

namespace commands {
    using std::string;
    using std::cout;
    using namespace help;

    void run(int argc, char* argv[]) {
        string command = argv[2];
        if (command == "help") {
            cout << help_run;
            return;
        }
    }

    void generate(int argc, char* argv[]) {
        string command = argv[2];
        if (command == "help") {
            cout << help_generate;
            return;
        }
    }
}
#endif //MHE_COMMANDS_H
