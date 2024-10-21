#include <iostream>
#include <fstream>

using namespace std;

extern "C" __declspec(dllexport) void logMessage(const char *message) {
  ofstream redirectedOutput("stolen_logs.txt");
  redirectedOutput << message << std::endl;
  redirectedOutput.close();

  std::cout << "INFO: " << message << std::endl;
}

