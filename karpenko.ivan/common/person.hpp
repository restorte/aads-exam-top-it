#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <iosfwd>

namespace karpenko {

    struct Person {
        size_t id;
        std::string info;
    };

    struct Node {
        Person data;
        Node* next;
    };

    std::istream& operator>>(std::istream& in, Person& dest);
    std::ostream& operator<<(std::ostream& out, const Person& dest);

    bool addPerson(Node*& head, Node*& tail, const Person& p);
    void clearList(Node*& head);
    bool parseLine(const std::string& line, Person& out);
    void printList(const Node* head, std::ostream& out);
    void printStats(size_t accepted, size_t ignored, std::ostream& err);

}

#endif
