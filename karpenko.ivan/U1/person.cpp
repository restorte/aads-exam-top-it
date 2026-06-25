#include "person.hpp"
#include <cstddef>
#include <cctype>
#include <stdexcept>
#include <iostream>

namespace karpenko {

    static bool hasId(const Node* head, size_t id) {
        const Node* cur = head;
        while (cur != NULL) {
            if (cur->data.id == id) {
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    bool addPerson(Node*& head, Node*& tail, const Person& p) {
        if (hasId(head, p.id)) {
            return false;
        }

        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = NULL;

        if (tail == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        return true;
    }

}
