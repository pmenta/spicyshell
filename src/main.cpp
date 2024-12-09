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

void echo(const std::string& text) {
  std::cout << text << std::endl;
}

enum class Builtin { echo, exit, type, unknown };
Builtin parseCommand(const std::string& command) {
  if (command == "echo") {
    return Builtin::echo;
  } else if (command == "exit") {
    return Builtin::exit;
  } else if (command == "type") {
    return Builtin::type;
  } else {
    return Builtin::unknown;
  }
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
      Builtin command = parseCommand(tokens[0]);
      if (command == Builtin::echo) {
        std::string params;
        for (int i = 1; i < tokens.size(); i++) {
          params += i == 1 ? tokens[i] : " " + tokens[i];
        }
        echo(params);
      } else if (command == Builtin::exit) {
        return std::stoi(tokens[1]);
      } else if (command == Builtin::type) {
        Builtin arg = parseCommand(tokens[1]);
        if (arg == Builtin::unknown) {
            std::cout << tokens[1] + ": not found\n";
        } else {
          std::cout << tokens[1] + " is a shell builtin\n";
        }


      } else {
        std::cout << input << ": command not found\n";
      }
    }
  }
}