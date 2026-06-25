#include "meeting_processor.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>

static bool parseArgs(int argc, char *argv[], std::string &inFile, std::string &dataFile, bool &hasIn, bool &hasData)
{
  if (argc < 2 || argc > 3)
  {
    return false;
  }

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];
    if (arg.compare(0, 3, "in:") == 0)
    {
      if (hasIn)
        return false;
      hasIn = true;
      inFile = arg.substr(3);
    }
    else if (arg.compare(0, 5, "data:") == 0)
    {
      if (hasData)
        return false;
      hasData = true;
      dataFile = arg.substr(5);
    }
    else
    {
      return false;
    }
  }
  return hasData;
}

int main(int argc, char *argv[])
{
  std::string inFile, dataFile;
  bool hasIn = false, hasData = false;

  if (!parseArgs(argc, argv, inFile, dataFile, hasIn, hasData))
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  karpenko::PersonTable table(1009);

  if (hasIn)
  {
    if (!karpenko::loadPersons(table, inFile))
    {
      std::cerr << "Cannot open input file\n";
      return 2;
    }
  }

  if (!karpenko::loadMeetings(table, dataFile))
  {
    std::cerr << "Cannot open data file or invalid data format\n";
    return 3;
  }

  std::string line;
  while (std::getline(std::cin, line))
  {
    size_t start = 0;
    while (start < line.size() && std::isspace(static_cast< unsigned char >(line[start])))
    {
      ++start;
    }
    size_t end = line.size();
    while (end > start && std::isspace(static_cast< unsigned char >(line[end - 1])))
    {
      --end;
    }
    if (start == end)
    {
      continue;
    }
    std::string trimmed = line.substr(start, end - start);

    size_t pos = trimmed.find(' ');
    std::string cmd;
    std::string args;
    if (pos == std::string::npos)
    {
      cmd = trimmed;
    }
    else
    {
      cmd = trimmed.substr(0, pos);
      args = trimmed.substr(pos + 1);
    }

    if (cmd == "anons")
    {
      karpenko::printAnons(table, std::cout);
    }
    else if (cmd == "deanon")
    {
      size_t space = args.find(' ');
      if (space == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string s1 = args.substr(0, space);
      std::string s2 = args.substr(space + 1);
      size_t p1 = 0;
      while (p1 < s1.size() && std::isspace(static_cast< unsigned char >(s1[p1])))
        ++p1;
      size_t p2 = 0;
      while (p2 < s2.size() && std::isspace(static_cast< unsigned char >(s2[p2])))
        ++p2;
      if (p1 == s1.size() || p2 == s2.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t id1, id2;
      try
      {
        id1 = static_cast< size_t >(std::stoul(s1.substr(p1)));
        id2 = static_cast< size_t >(std::stoul(s2.substr(p2)));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      karpenko::deanon(table, id1, id2);
    }
    else if (cmd == "desc")
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t p = 0;
      while (p < args.size() && std::isspace(static_cast< unsigned char >(args[p])))
        ++p;
      if (p == args.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t id;
      try
      {
        id = static_cast< size_t >(std::stoul(args.substr(p)));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      karpenko::printDescription(table, id, std::cout);
    }
    else if (cmd == "redesc")
    {
      size_t space = args.find(' ');
      if (space == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string sId = args.substr(0, space);
      std::string rest = args.substr(space + 1);
      size_t q1 = rest.find('"');
      if (q1 == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t q2 = rest.find('"', q1 + 1);
      if (q2 == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string desc = rest.substr(q1 + 1, q2 - q1 - 1);
      size_t id;
      try
      {
        id = static_cast<size_t>(std::stoul(sId));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      karpenko::redesc(table, id, desc);
    }
    else if (cmd == "meets")
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t p = 0;
      while (p < args.size() && std::isspace(static_cast< unsigned char >(args[p])))
        ++p;
      if (p == args.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t id;
      try
      {
        id = static_cast< size_t >(std::stoul(args.substr(p)));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      karpenko::printMeets(table, id, std::cout);
    }
    else if (cmd == "commons")
    {
      size_t space = args.find(' ');
      if (space == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string s1 = args.substr(0, space);
      std::string s2 = args.substr(space + 1);
      size_t p1 = 0;
      while (p1 < s1.size() && std::isspace(static_cast< unsigned char >(s1[p1])))
        ++p1;
      size_t p2 = 0;
      while (p2 < s2.size() && std::isspace(static_cast< unsigned char >(s2[p2])))
        ++p2;
      if (p1 == s1.size() || p2 == s2.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t id1, id2;
      try
      {
        id1 = static_cast< size_t >(std::stoul(s1.substr(p1)));
        id2 = static_cast< size_t >(std::stoul(s2.substr(p2)));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      karpenko::printCommons(table, id1, id2, std::cout);
    }
    else if (cmd == "less" || cmd == "greater")
    {
      size_t space = args.find(' ');
      if (space == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string sTime = args.substr(0, space);
      std::string sId = args.substr(space + 1);
      size_t p1 = 0;
      while (p1 < sTime.size() && std::isspace(static_cast< unsigned char >(sTime[p1])))
        ++p1;
      size_t p2 = 0;
      while (p2 < sId.size() && std::isspace(static_cast< unsigned char >(sId[p2])))
        ++p2;
      if (p1 == sTime.size() || p2 == sId.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t time, id;
      try
      {
        time = static_cast< size_t >(std::stoul(sTime.substr(p1)));
        id = static_cast< size_t >(std::stoul(sId.substr(p2)));
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      if (cmd == "less")
      {
        karpenko::printLess(table, id, time, std::cout);
      }
      else
      {
        karpenko::printGreater(table, id, time, std::cout);
      }
    }
    else if (cmd == "out-persons")
    {
      if (args.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      size_t p = 0;
      while (p < args.size() && std::isspace(static_cast< unsigned char >(args[p])))
        ++p;
      if (p == args.size())
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string filename = args.substr(p);
      karpenko::outPersons(table, filename);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
