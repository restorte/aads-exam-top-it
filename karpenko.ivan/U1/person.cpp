#include "person.hpp"
#include <cstddef>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <limits>

namespace karpenko
{

  std::istream& operator>>(std::istream& in, Person& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    Person input;
    if (!(in >> input.id)) {
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return in;
    }

    while (in.good() && std::isspace(static_cast<unsigned char>(in.peek())) && in.peek() != '\n') {
      in.get();
    }

    std::getline(in, input.info);

    size_t start = 0;
    while (start < input.info.length() && std::isspace(static_cast<unsigned char>(input.info[start]))) {
      ++start;
    }
    size_t end = input.info.length();
    while (end > start && std::isspace(static_cast<unsigned char>(input.info[end - 1]))) {
      --end;
    }
    input.info = input.info.substr(start, end - start);

    if (input.info.empty()) {
      return in;
    }

    dest = input;
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Person& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    out << dest.id << ' ' << dest.info;
    return out;
  }

  static bool hasId(const Node *head, size_t id)
  {
    const Node *cur = head;
    while (cur != NULL)
    {
      if (cur->data.id == id)
      {
        return true;
      }
      cur = cur->next;
    }
    return false;
  }

  bool addPerson(Node *&head, Node *&tail, const Person &p)
  {
    if (hasId(head, p.id))
    {
      return false;
    }

    Node *newNode = new Node;
    try
    {
      newNode->data = p;
    }
    catch (...)
    {
      delete newNode;
      throw;
    }
    newNode->next = NULL;

    if (tail == NULL)
    {
      head = tail = newNode;
    }
    else
    {
      tail->next = newNode;
      tail = newNode;
    }
    return true;
  }

  void clearList(Node *&head)
  {
    Node *cur = head;
    while (cur != NULL)
    {
      Node *next = cur->next;
      delete cur;
      cur = next;
    }
    head = NULL;
  }

  void printList(const Node *head, std::ostream &out)
  {
    const Node *cur = head;
    while (cur != NULL)
    {
      out << cur->data << '\n';
      cur = cur->next;
    }
  }

  void printStats(size_t accepted, size_t ignored, std::ostream &err)
  {
    err << accepted << ' ' << ignored << '\n';
  }

}
