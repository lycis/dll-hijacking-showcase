#include <iostream>
extern "C" __declspec(dllexport) void logMessage(const char *message) {
    std::cout << "INFO: " << message << std::endl;
}

