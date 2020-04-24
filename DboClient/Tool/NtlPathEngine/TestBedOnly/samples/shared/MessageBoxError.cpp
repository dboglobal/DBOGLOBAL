
#include "error.h"
#include "messagebox.h"
#include <sstream>
#include <stdlib.h>

void Error(const char* errorType, const char* errorString, const char** attributes)
{
    std::ostringstream message;
    message << "Error type: " << errorType << std::endl;
    message << "Error string: " << errorString << std::endl;
    if(attributes && *attributes)
    {
        message << "Error attributes:" << std::endl;
        do
        {
            message << " " << attributes[0] << " = " << attributes[1] << std::endl;
            attributes += 2;
        }
        while(*attributes);
    }
    ShowMessageBox("Error", message.str().c_str());
    if(strcmp(errorType, "Fatal") == 0)
    {
        exit(1);
    }
}
void ReportTestFailure(const char* file, long line, const char* expression)
{
    std::ostringstream message;
    message << file << "(" << line << "): test(" << expression << ")";
    ShowMessageBox("Test case failed, aborting.", message.str().c_str());
    exit(1);
}
bool ReportAssertionFailure(const char* file, long line, const char* expression)
{
    std::ostringstream message;
    message << file << "(" << line << "): assertD(" << expression << ")" << std::endl;
    message << std::endl;
    message << "Application execution will continue, but this may result in a crash." << std::endl;
    bool stayOn = ShowMessageBoxQuery("Assertion failed", message.str().c_str(), "Select Yes to continue to report this assertion in the future, or No to stop checking this.");
    return stayOn;
}
