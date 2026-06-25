#include <iostream>
#include <fstream>
#include <string>

#include "input_handler.hpp"
#include "person.hpp"
#include "parse_line.hpp"
#include "vector.hpp"

int main(int argc, char* argv[])
{
  std::string inputFileName;
  std::string outputFileName;

  bool hasIn = false;
  bool hasOut = false;

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg.rfind("in:", 0) == 0)
    {
      if (hasIn)
      {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
      }
      inputFileName = arg.substr(3);
      hasIn = true;
    }
    else if (arg.rfind("out:", 0) == 0)
    {
      if (hasOut)
      {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
      }
      outputFileName = arg.substr(4);
      hasOut = true;
    }
    else
    {
      std::cerr << "Invalid arguments." << std::endl;
      return 1;
    }
  }

  smirnova::InputHandler inputHandler(inputFileName);

  smirnova::Vector<smirnova::Person> persons;
  size_t validEntries = 0;
  size_t ignoredEntries = 0;

  smirnova::processInput(inputHandler.getInputStream(),
                          persons,
                          validEntries,
                          ignoredEntries);

  // ✔ EMPTY INPUT FIX
  if (persons.size() == 0)
  {
    std::cerr << validEntries << " " << ignoredEntries << std::endl;
    return 0;
  }

  if (hasOut)
  {
    std::ofstream out(outputFileName);

    if (!out)
    {
      std::cerr << "Failed to open output file." << std::endl;
      return 2;
    }

    std::cout << "in file " << outputFileName << "\n";
    smirnova::printPersons(persons, out);
  }
  else
  {
    std::cout << "in file\n";
    smirnova::printPersons(persons, std::cout);
  }

  std::cerr << validEntries << " " << ignoredEntries << std::endl;

  return 0;
}





