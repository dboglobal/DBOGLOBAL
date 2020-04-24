
#include "loadbinary.h"
#include "error.h"
#include <stdio.h>

void
LoadBinary(const char* fileName, std::vector<char>& buffer)
{
    buffer.clear();

    FILE* fp = fopen(fileName, "rb");
    if(!fp)
    {
        Error("NonFatal", "loadBinary: Failed to open file. Returning an empty buffer.", 0);
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer.resize(size);
    fread(&buffer.front(), 1, size, fp);
    fclose(fp);
}
