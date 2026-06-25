#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace karpenko
{

  template < typename T >
  struct ListNode
  {
    T data;
    ListNode *next;
  };

  template < typename T >
  void listPushFront(ListNode<T> *&head, const T &value)
  {
    ListNode<T> *newNode = new ListNode<T>;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
  }

  template < typename T >
  void listClear(ListNode<T> *&head)
  {
    while (head != NULL)
    {
      ListNode<T> *next = head->next;
      delete head;
      head = next;
    }
  }

  template < typename T >
  size_t listSize(const ListNode<T> *head)
  {
    size_t count = 0;
    while (head != NULL)
    {
      ++count;
      head = head->next;
    }
    return count;
  }

}

#endif
