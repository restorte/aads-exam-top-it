#include "person.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

static bool parseArgs(int argc, char* argv[], std::string& inFile, std::string& outFile, bool& hasIn, bool& hasOut) {
    if (argc > 3) {
        return false;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.compare(0, 3, "in:") == 0) {
            if (hasIn) return false;
            hasIn = true;
            inFile = arg.substr(3);
        } else if (arg.compare(0, 4, "out:") == 0) {
            if (hasOut) return false;
            hasOut = true;
            outFile = arg.substr(4);
        } else {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::string inFile, outFile;
    bool hasIn = false, hasOut = false;

    if (!parseArgs(argc, argv, inFile, outFile, hasIn, hasOut)) {
        return 1;
    }

    karpenko::Node* head = NULL;
    karpenko::Node* tail = NULL;
    size_t accepted = 0;
    size_t ignored = 0;

    std::string line;
    while (std::getline(std::cin, line)) {
        karpenko::Person p;
        bool ok = karpenko::parseLine(line, p);
        if (!ok) {
            ++ignored;
            continue;
        }
        bool added = karpenko::addPerson(head, tail, p);
        if (added) {
            ++accepted;
        } else {
            ++ignored;
        }
    }

    karpenko::printList(head, std::cout);
    karpenko::printStats(accepted, ignored, std::cerr);

    karpenko::clearList(head);
    return 0;
}
