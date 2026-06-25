#include "meeting_processor.hpp"
#include <cstddef>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <limits>

namespace karpenko
{

  static bool parsePersonLine(const std::string & line, size_t & outId, std::string & outInfo)
  {
    size_t pos = 0;
    while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
    {
      ++pos;
    }
    if (pos == line.size())
    {
      return false;
    }

    size_t start = pos;
    while (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))
    {
      ++pos;
    }
    std::string idStr = line.substr(start, pos - start);

    size_t id;
    try
    {
      id = static_cast< size_t >(std::stoul(idStr));
    }
    catch (const std::exception &)
    {
      return false;
    }

    while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
    {
      ++pos;
    }
    if (pos == line.size())
    {
      return false;
    }

    std::string info = line.substr(pos);
    while (!info.empty() && std::isspace(static_cast< unsigned char >(info.back())))
    {
      info.pop_back();
    }
    if (info.empty())
    {
      return false;
    }

    outId = id;
    outInfo = info;
    return true;
  }

  bool loadPersons(PersonTable & table, const std::string & filename)
  {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
      return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
      size_t id;
      std::string info;
      if (parsePersonLine(line, id, info))
      {
        PersonData data;
        data.description = info;
        data.hasDescription = true;
        data.meetings = NULL;
        table.insert(id, data);
      }
    }
    file.close();
    return true;
  }

  bool loadMeetings(PersonTable & table, const std::string & filename)
  {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
      return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
      size_t pos = 0;
      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      if (pos == line.size())
      {
        return false;
      }
      size_t start = pos;
      while (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      std::string num1 = line.substr(start, pos - start);
      size_t id1;
      try
      {
        id1 = static_cast< size_t >(std::stoul(num1));
      }
      catch (...)
      {
        return false;
      }

      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      if (pos == line.size())
      {
        return false;
      }
      start = pos;
      while (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      std::string num2 = line.substr(start, pos - start);
      size_t id2;
      try
      {
        id2 = static_cast< size_t >(std::stoul(num2));
      }
      catch (...)
      {
        return false;
      }

      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      if (pos == line.size())
      {
        return false;
      }
      start = pos;
      while (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      std::string num3 = line.substr(start, pos - start);
      size_t time;
      try
      {
        time = static_cast< size_t >(std::stoul(num3));
      }
      catch (...)
      {
        return false;
      }

      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      {
        ++pos;
      }
      if (pos != line.size())
      {
        return false;
      }

      if (id1 == id2)
      {
        continue;
      }

      PersonData * data1 = table.find(id1);
      if (data1 == NULL)
      {
        PersonData empty;
        empty.description = "";
        empty.hasDescription = false;
        empty.meetings = NULL;
        table.insert(id1, empty);
        data1 = table.find(id1);
      }
      MeetingInfo m1;
      m1.otherId = id2;
      m1.time = time;
      listPushFront(data1->meetings, m1);

      PersonData * data2 = table.find(id2);
      if (data2 == NULL)
      {
        PersonData empty;
        empty.description = "";
        empty.hasDescription = false;
        empty.meetings = NULL;
        table.insert(id2, empty);
        data2 = table.find(id2);
      }
      MeetingInfo m2;
      m2.otherId = id1;
      m2.time = time;
      listPushFront(data2->meetings, m2);
    }
    file.close();
    return true;
  }

  static MeetingInfo * getMeetingsArray(const ListNode< MeetingInfo > * head, size_t & count)
  {
    count = listSize(head);
    if (count == 0)
    {
      return NULL;
    }
    MeetingInfo * arr = new MeetingInfo[count];
    size_t i = 0;
    const ListNode< MeetingInfo > * cur = head;
    while (cur != NULL)
    {
      arr[i] = cur->data;
      ++i;
      cur = cur->next;
    }
    return arr;
  }

  static void freeMeetingsArray(MeetingInfo * arr)
  {
    delete[] arr;
  }

  static void sortMeetings(MeetingInfo * arr, size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      for (size_t j = i + 1; j < count; ++j)
      {
        bool needSwap = false;
        if (arr[i].otherId > arr[j].otherId)
        {
          needSwap = true;
        }
        else if (arr[i].otherId == arr[j].otherId && arr[i].time > arr[j].time)
        {
          needSwap = true;
        }
        if (needSwap)
        {
          MeetingInfo tmp = arr[i];
          arr[i] = arr[j];
          arr[j] = tmp;
        }
      }
    }
  }

  static void printSortedMeetings(const ListNode< MeetingInfo > * head, std::ostream & out)
  {
    size_t count;
    MeetingInfo * arr = getMeetingsArray(head, count);
    if (count == 0)
    {
      freeMeetingsArray(arr);
      return;
    }
    sortMeetings(arr, count);
    for (size_t i = 0; i < count; ++i)
    {
      out << arr[i].otherId << ' ' << arr[i].time << '\n';
    }
    freeMeetingsArray(arr);
  }

  void printAnons(const PersonTable & table, std::ostream & out)
  {
    size_t * ids = new size_t[table.tableSize * 10];
    size_t count = 0;
    try
    {
      for (size_t i = 0; i < table.tableSize; ++i)
      {
        const PersonTable::Node * cur = table.table[i];
        while (cur != NULL)
        {
          if (!cur->data.second.hasDescription)
          {
            ids[count++] = cur->data.first;
          }
          cur = cur->next;
        }
      }
      for (size_t i = 0; i < count; ++i)
      {
        for (size_t j = i + 1; j < count; ++j)
        {
          if (ids[i] > ids[j])
          {
            size_t tmp = ids[i];
            ids[i] = ids[j];
            ids[j] = tmp;
          }
        }
      }
      for (size_t i = 0; i < count; ++i)
      {
        out << ids[i] << '\n';
      }
    }
    catch (...)
    {
      delete[] ids;
      throw;
    }
    delete[] ids;
  }

  void deanon(PersonTable & table, size_t anonId, size_t newId)
  {
    PersonData * anonData = table.find(anonId);
    PersonData * newPersonData = table.find(newId);
    if (anonData == NULL || newPersonData == NULL)
    {
      return;
    }
    if (anonData->hasDescription || !newPersonData->hasDescription)
    {
      return;
    }

    ListNode< MeetingInfo > * cur = anonData->meetings;
    while (cur != NULL)
    {
      ListNode< MeetingInfo > * next = cur->next;
      if (cur->data.otherId != newId)
      {
        listPushFront(newPersonData->meetings, cur->data);
        PersonData * otherData = table.find(cur->data.otherId);
        if (otherData != NULL)
        {
          MeetingInfo rev;
          rev.otherId = newId;
          rev.time = cur->data.time;
          listPushFront(otherData->meetings, rev);
        }
      }
      delete cur;
      cur = next;
    }
    anonData->meetings = NULL;
    table.remove(anonId);
  }

  void printDescription(const PersonTable & table, size_t id, std::ostream & out)
  {
    const PersonData * data = table.find(id);
    if (data == NULL)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (data->hasDescription)
    {
      out << data->description << '\n';
    }
    else
    {
      out << "<ANON>\n";
    }
  }

  void redesc(PersonTable & table, size_t id, const std::string & newDesc)
  {
    PersonData * data = table.find(id);
    if (data == NULL)
    {
      PersonData empty;
      empty.description = newDesc;
      empty.hasDescription = true;
      empty.meetings = NULL;
      table.insert(id, empty);
    }
    else
    {
      data->description = newDesc;
      data->hasDescription = true;
    }
  }

  void printMeets(const PersonTable & table, size_t id, std::ostream & out)
  {
    const PersonData * data = table.find(id);
    if (data == NULL)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    printSortedMeetings(data->meetings, out);
  }

  void printCommons(const PersonTable & table, size_t id1, size_t id2, std::ostream & out)
  {
    const PersonData * data1 = table.find(id1);
    const PersonData * data2 = table.find(id2);
    if (data1 == NULL || data2 == NULL)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    size_t count1 = listSize(data1->meetings);
    size_t * ids1 = new size_t[count1];
    size_t idx = 0;
    try
    {
      const ListNode< MeetingInfo > * cur = data1->meetings;
      while (cur != NULL)
      {
        ids1[idx++] = cur->data.otherId;
        cur = cur->next;
      }

      size_t count2 = listSize(data2->meetings);
      size_t * ids2 = new size_t[count2];
      idx = 0;
      cur = data2->meetings;
      while (cur != NULL)
      {
        ids2[idx++] = cur->data.otherId;
        cur = cur->next;
      }

      for (size_t i = 0; i < count1; ++i)
      {
        for (size_t j = i + 1; j < count1; ++j)
        {
          if (ids1[i] > ids1[j])
          {
            size_t tmp = ids1[i];
            ids1[i] = ids1[j];
            ids1[j] = tmp;
          }
        }
      }
      for (size_t i = 0; i < count2; ++i)
      {
        for (size_t j = i + 1; j < count2; ++j)
        {
          if (ids2[i] > ids2[j])
          {
            size_t tmp = ids2[i];
            ids2[i] = ids2[j];
            ids2[j] = tmp;
          }
        }
      }

      size_t i = 0, j = 0;
      while (i < count1 && j < count2)
      {
        if (ids1[i] == ids2[j])
        {
          out << ids1[i] << '\n';
          size_t val = ids1[i];
          while (i < count1 && ids1[i] == val)
          {
            ++i;
          }
          while (j < count2 && ids2[j] == val)
          {
            ++j;
          }
        }
        else if (ids1[i] < ids2[j])
        {
          ++i;
        }
        else
        {
          ++j;
        }
      }

      delete[] ids2;
    }
    catch (...)
    {
      delete[] ids1;
      throw;
    }
    delete[] ids1;
  }

  void printLess(const PersonTable & table, size_t id, size_t time, std::ostream & out)
  {
    const PersonData * data = table.find(id);
    if (data == NULL)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    size_t count = listSize(data->meetings);
    MeetingInfo * arr = new MeetingInfo[count];
    size_t idx = 0;
    try
    {
      const ListNode< MeetingInfo > * cur = data->meetings;
      while (cur != NULL)
      {
        if (cur->data.time < time)
        {
          arr[idx++] = cur->data;
        }
        cur = cur->next;
      }
      sortMeetings(arr, idx);
      for (size_t i = 0; i < idx; ++i)
      {
        out << arr[i].otherId << ' ' << arr[i].time << '\n';
      }
    }
    catch (...)
    {
      delete[] arr;
      throw;
    }
    delete[] arr;
  }

  void printGreater(const PersonTable & table, size_t id, size_t time, std::ostream & out)
  {
    const PersonData * data = table.find(id);
    if (data == NULL)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    size_t count = listSize(data->meetings);
    MeetingInfo * arr = new MeetingInfo[count];
    size_t idx = 0;
    try
    {
      const ListNode< MeetingInfo > * cur = data->meetings;
      while (cur != NULL)
      {
        if (cur->data.time > time)
        {
          arr[idx++] = cur->data;
        }
        cur = cur->next;
      }
      sortMeetings(arr, idx);
      for (size_t i = 0; i < idx; ++i)
      {
        out << arr[i].otherId << ' ' << arr[i].time << '\n';
      }
    }
    catch (...)
    {
      delete[] arr;
      throw;
    }
    delete[] arr;
  }

  void outPersons(const PersonTable & table, const std::string & filename)
  {
    std::ofstream file(filename.c_str());
    if (!file.is_open())
    {
      return;
    }
    for (size_t i = 0; i < table.tableSize; ++i)
    {
      const PersonTable::Node * cur = table.table[i];
      while (cur != NULL)
      {
        if (cur->data.second.hasDescription)
        {
          file << cur->data.first << ' ' << cur->data.second.description << '\n';
        }
        cur = cur->next;
      }
    }
    file.close();
  }

}
