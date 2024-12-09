#include <complex>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

using Args = std::vector<std::string>;

Args split(const std::string &s, const char delim) {
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

void type(const Args& tokens) {
  const Builtin arg = parseCommand(tokens[1]);
  std::string pathToArg;

  if (const char* pathEnvVar = std::getenv("PATH"); pathEnvVar) {
    const std::string path = pathEnvVar;
    std::vector<std::string> paths = split(path, ':');
    for (const std::string& path : paths) {
      if (!pathToArg.empty()) break;
      for (const auto& entry : fs::directory_iterator(path)) {
        if (!pathToArg.empty()) break;
        auto vecDir = split(entry.path().string(), '/');
        const auto& lastItem = vecDir[vecDir.size() - 1];
        if (lastItem == tokens[1]) {
          pathToArg = entry.path().string();
          break;
        }
      }
    }
  }

  if (arg != Builtin::unknown) {
    std::cout << tokens[1] + " is a shell builtin\n";
  } else {
    if (pathToArg.empty()) {
      std::cout << tokens[1] + ": not found\n";
    } else {
      std::cout << tokens[1] + " is " + pathToArg + "\n";
    }
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

    if (std::vector<std::string> tokens = split(input, ' '); !tokens.empty()) {
      if (const Builtin command = parseCommand(tokens[0]); command == Builtin::echo) {
        std::string params;
        for (int i = 1; i < tokens.size(); i++) {
          params += i == 1 ? tokens[i] : " " + tokens[i];
        }
        echo(params);
      } else if (command == Builtin::exit) {
        return std::stoi(tokens[1]);
      } else if (command == Builtin::type) {
        type(tokens);
      } else {
        std::cout << input << ": command not found\n";
      }
    }
  }
}