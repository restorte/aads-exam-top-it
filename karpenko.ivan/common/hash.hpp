#ifndef COMMON_HASH_HPP
#define COMMON_HASH_HPP

#include <cstddef>
#include <utility>
#include "list.hpp"

namespace karpenko
{

  template < typename Key >
  struct DefaultHash
  {
    size_t operator()(const Key &key) const
    {
      return static_cast<size_t>(key);
    }
  };

  template < typename Key, typename Value, typename Hash = DefaultHash< Key > >
  struct HashTable
  {
    typedef std::pair< Key, Value > Pair;
    typedef ListNode<Pair> Node;

    size_t tableSize;
    Node **table;
    Hash hash;

    explicit HashTable(size_t size = 1013)
        : tableSize(size), table(new Node *[size]()), hash()
    {
      for (size_t i = 0; i < tableSize; ++i)
      {
        table[i] = NULL;
      }
    }

    ~HashTable()
    {
      clear();
      delete[] table;
    }

    void insert(const Key &key, const Value &value)
    {
      size_t index = hash(key) % tableSize;
      Node *cur = table[index];
      while (cur != NULL)
      {
        if (cur->data.first == key)
        {
          cur->data.second = value;
          return;
        }
        cur = cur->next;
      }
      Pair newPair(key, value);
      listPushFront(table[index], newPair);
    }

    Value *find(const Key &key)
    {
      size_t index = hash(key) % tableSize;
      Node *cur = table[index];
      while (cur != NULL)
      {
        if (cur->data.first == key)
        {
          return &(cur->data.second);
        }
        cur = cur->next;
      }
      return NULL;
    }

    const Value *find(const Key &key) const
    {
      size_t index = hash(key) % tableSize;
      const Node *cur = table[index];
      while (cur != NULL)
      {
        if (cur->data.first == key)
        {
          return &(cur->data.second);
        }
        cur = cur->next;
      }
      return NULL;
    }

    bool remove(const Key &key)
    {
      size_t index = hash(key) % tableSize;
      Node *cur = table[index];
      Node *prev = NULL;
      while (cur != NULL)
      {
        if (cur->data.first == key)
        {
          if (prev == NULL)
          {
            table[index] = cur->next;
          }
          else
          {
            prev->next = cur->next;
          }
          delete cur;
          return true;
        }
        prev = cur;
        cur = cur->next;
      }
      return false;
    }

    void clear()
    {
      for (size_t i = 0; i < tableSize; ++i)
      {
        listClear(table[i]);
      }
    }
  };

}

#endif
