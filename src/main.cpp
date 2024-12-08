#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::string element;
  std::istringstream elemsStream(s);

  while (std::getline(elemsStream, element, delim)) {
    elems.push_back(element);
  }

  return elems;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = split(input, ' ');

    if (!tokens.empty()) {
      if (tokens[0] == "exit") {
        return std::stoi(tokens[1]);
      }
    }

    std::cout << input << ": command not found\n";
  }
}