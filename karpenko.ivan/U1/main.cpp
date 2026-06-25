#include "person.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

static bool parseArgs(int argc, char *argv[], std::string &inFile, std::string &outFile, bool &hasIn, bool &hasOut)
{
  if (argc > 3)
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
    else if (arg.compare(0, 4, "out:") == 0)
    {
      if (hasOut)
        return false;
      hasOut = true;
      outFile = arg.substr(4);
    }
    else
    {
      return false;
    }
  }
  return true;
}

static bool isWhitespaceLine(const std::string &line)
{
  for (size_t i = 0; i < line.size(); ++i)
  {
    if (!std::isspace(static_cast<unsigned char>(line[i])))
    {
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[])
{
  std::string inFile, outFile;
  bool hasIn = false, hasOut = false;

  if (!parseArgs(argc, argv, inFile, outFile, hasIn, hasOut))
  {
    return 1;
  }

  std::istream *inPtr = &std::cin;
  std::ifstream inStream;
  if (hasIn)
  {
    inStream.open(inFile.c_str());
    if (!inStream.is_open())
    {
      return 2;
    }
    inPtr = &inStream;
  }

  karpenko::Node *head = NULL;
  karpenko::Node *tail = NULL;
  size_t accepted = 0;
  size_t ignored = 0;

  try
  {
    std::string line;
    while (std::getline(*inPtr, line))
    {
      if (isWhitespaceLine(line))
      {
        continue;
      }

      karpenko::Person p;
      bool ok = karpenko::parseLine(line, p);
      if (!ok)
      {
        ++ignored;
        continue;
      }
      bool added = karpenko::addPerson(head, tail, p);
      if (added)
      {
        ++accepted;
      }
      else
      {
        ++ignored;
      }
    }
  }
  catch (...)
  {
    karpenko::clearList(head);
    if (hasIn && inStream.is_open())
      inStream.close();
    return 2;
  }

  if (hasIn && inStream.is_open())
  {
    inStream.close();
  }

  std::ostream *outPtr = &std::cout;
  std::ofstream outStream;
  if (hasOut)
  {
    outStream.open(outFile.c_str());
    if (!outStream.is_open())
    {
      karpenko::clearList(head);
      return 2;
    }
    outPtr = &outStream;
  }

  try
  {
    karpenko::printList(head, *outPtr);
    karpenko::printStats(accepted, ignored, std::cerr);
  }
  catch (...)
  {
    karpenko::clearList(head);
    if (hasOut && outStream.is_open())
      outStream.close();
    return 2;
  }

  karpenko::clearList(head);

  if (hasOut && outStream.is_open())
  {
    outStream.close();
  }

  return 0;
}
