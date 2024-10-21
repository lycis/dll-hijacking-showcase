#include <windows.h>
#include <iostream>
#include <string>
#include "checksums.h"

typedef void (*logMessage)(const char*);

std::string md5Checksum(const std::string& filepath);

int main() {
    const char* libPath = "C:\\Users\\danieleder\\projects\\dll-hj\\libs\\mylog.dll"; 

    std::string libCheckSum = md5Checksum(libPath);
    if(libCheckSum.compare(MYLOG_CHECKSUM) != 0) {
      std::cerr << "Library was modified." << std::endl;
      return 1;
    }
    
    HINSTANCE hDLL = LoadLibrary(libPath);


    if (hDLL != NULL) {
        logMessage log = (logMessage)GetProcAddress(hDLL, "logMessage");
        if (log) {
            log("Application is running.");
        } else {
          std::cerr << "Failed to find log-function" << std::endl;
        }
        FreeLibrary(hDLL);
    } else {
        std::cerr << "Failed to load DLL" << std::endl;
    }

    system("pause");

    return 0;
}
