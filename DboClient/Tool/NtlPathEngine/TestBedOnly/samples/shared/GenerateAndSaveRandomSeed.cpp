
#include "GenerateAndSaveRandomSeed.h"
#include "Error.h"
#include <fstream>
#include <windows.h>

unsigned long
GenerateAndSaveRandomSeed()
{
    unsigned long result = timeGetTime();

    std::ofstream os("savedRandomSeed.txt");
    os << result;
    if(!os)
    {
        Error("NonFatal", "An error occurred saving the random seed.", 0);
    }
    return result;
}
