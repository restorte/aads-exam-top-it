#include "../common/person.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>

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

int main(int argc, char *argv[])
{
  if (argc > 3)
  {
    std::cerr << "Too many arguments\n";
    return 0;
  }

  std::string inFile, outFile;
  bool hasIn = false, hasOut = false;

  if (!parseArgs(argc, argv, inFile, outFile, hasIn, hasOut))
  {
    return 1;
  }

  std::ifstream inStream;
  std::ofstream outStream;
  std::istream *inPtr = &std::cin;
  std::ostream *outPtr = &std::cout;

  if (hasIn)
  {
    inStream.open(inFile.c_str());
    if (!inStream.is_open())
    {
      std::cerr << "Cannot open input file\n";
      return 2;
    }
    inPtr = &inStream;
  }

  karpenko::Node *head = NULL;
  karpenko::Node *tail = NULL;
  size_t accepted = 0;
  size_t ignored = 0;
  size_t count = 0;

  try
  {
    while (inPtr->good() && !inPtr->eof())
    {
      karpenko::Person p;
      if (*inPtr >> p)
      {
        bool added = karpenko::addPerson(head, tail, p);
        if (added)
        {
          ++accepted;
          ++count;
        }
        else
        {
          ++ignored;
        }
      }
      else
      {
        if (!inPtr->eof())
        {
          ++ignored;
          inPtr->clear();
          if (inPtr->fail())
          {
            inPtr->ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          }
        }
      }
    }
  }
  catch (...)
  {
    karpenko::clearList(head);
    if (hasIn && inStream.is_open())
      inStream.close();
    if (hasOut && outStream.is_open())
      outStream.close();
    return 2;
  }

  if (hasIn && inStream.is_open())
  {
    inStream.close();
  }

  if (hasOut)
  {
    outStream.open(outFile.c_str());
    if (!outStream.is_open())
    {
      karpenko::clearList(head);
      std::cerr << "Cannot open output file\n";
      return 2;
    }
    outPtr = &outStream;
  }

  try
  {
    if (count == 0)
    {
      *outPtr << '\n';
    }
    else
    {
      karpenko::printList(head, *outPtr);
    }
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
