#ifndef MEETING_PROCESSOR_HPP
#define MEETING_PROCESSOR_HPP

#include <string>
#include <iosfwd>
#include "../common/list.hpp"
#include "../common/hash.hpp"

namespace karpenko
{

  struct MeetingInfo
  {
    size_t otherId;
    size_t time;
  };

  struct PersonData
  {
    std::string description;
    bool hasDescription;
    ListNode<MeetingInfo> *meetings;
  };

  typedef HashTable<size_t, PersonData> PersonTable;

  bool loadPersons(PersonTable &table, const std::string &filename);
  bool loadMeetings(PersonTable &table, const std::string &filename);

  void printAnons(const PersonTable &table, std::ostream &out);
  void deanon(PersonTable &table, size_t anonId, size_t newId);
  void printDescription(const PersonTable &table, size_t id, std::ostream &out);
  void redesc(PersonTable &table, size_t id, const std::string &newDesc);
  void printMeets(const PersonTable &table, size_t id, std::ostream &out);
  void printCommons(const PersonTable &table, size_t id1, size_t id2, std::ostream &out);
  void printLess(const PersonTable &table, size_t id, size_t time, std::ostream &out);
  void printGreater(const PersonTable &table, size_t id, size_t time, std::ostream &out);
  void outPersons(const PersonTable &table, const std::string &filename);

}

#endif
