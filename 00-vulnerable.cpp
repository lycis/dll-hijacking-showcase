#include <windows.h>
#include <iostream>

typedef void (*logMessage)(const char*);

int main() {
    
    HINSTANCE hDLL = LoadLibrary("mylog.dll");


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
