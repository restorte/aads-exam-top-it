#include "person.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

int main() {
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
