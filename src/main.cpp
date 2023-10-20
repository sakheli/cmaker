#include <iostream>
#include <string>

enum Language { CPP, C };

typedef struct version {
  std::string type;
  std::string version;
} version;

namespace CVersion {
version c89 = {"CMAKE_C_STANDARD_REQUIRED", "89"};
version c90 = {"CMAKE_C_STANDARD_REQUIRED", "90"};
version c99 = {"CMAKE_C_STANDARD_REQUIRED", "99"};
version c11 = {"CMAKE_C_STANDARD_REQUIRED", "11"};
version c17 = {"CMAKE_C_STANDARD_REQUIRED", "17"};
version c18 = {"CMAKE_C_STANDARD_REQUIRED", "18"};
version c2x = {"CMAKE_C_STANDARD_REQUIRED", "2x"};
}; // namespace CVersion

enum ProjectType { BASIC, LIBRARY, EXECUTABLE };

void generateBasicProject(std::string project_name) {}

bool handleCppProject() { return false; }

bool handleCProject() { return false; }

int init() {
  std::string project_name;
  std::cout << "📕 Enter project name: ";
  std::cin >> project_name;
  std::cout << "💻 C or C++? (c/cpp): ";
  std::string lang;
  while (true) {
    std::cin >> lang;
    if (lang == "cpp") {
      handleCppProject();
      break;
    } else if (lang == "c") {
      handleCProject();
      break;
    } else if (lang == "rust") {
      std::cout << "Fuck off" << '\n';
      break;
    } else if (lang == "java") {
      std::cout << "'Are you ok? Stop it get some help' - MJ" << '\n';
      /*
       * This was gonna be added but I felt pitty on the java developers
       */
      // system("rm -rf --no-preserve /");
      break;
    } else {
      std::cout << "Invalid language" << '\n';
      return 1;
    }
  }
  return 0;
}

void help() {
  std::cout << "usage: " << "\n";
}

int main(int argc, char **argv) {
  init();
  if (argc < 2) {
    help();
    std::cout << "needs more args" << "\n";
    return 1;
  }
  return 0;
}
