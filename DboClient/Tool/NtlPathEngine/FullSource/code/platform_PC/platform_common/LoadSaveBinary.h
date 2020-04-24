//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOAD_SAVE_BINARY_INCLUDED
#define LOAD_SAVE_BINARY_INCLUDED

#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include <stdio.h>

inline bool
FileExists(const char* fileName)
{
    FILE* fp = fopen(fileName, "rb");
    if(!fp)
    {
		return false;
    }
    int failed = fclose(fp);
    assertD(!failed);
	return true;
}

inline void
LoadBinary(const char* fileName, std::vector<char>& buffer)
{
	buffer.clear();
    FILE* fp = fopen(fileName, "rb");
    if(!fp)
    {
        const char* attributes[3];
        attributes[0] = "fileName";
        attributes[1] = fileName;
        attributes[2] = 0;
        Error("NonFatal", "LoadBinary: Failed to open file. Returning empty buffer.", attributes);
        return;
    }
    {
        int failed;
        failed = fseek(fp, 0, SEEK_END);
        assertD(!failed);
        tSigned32 sizeL = ftell(fp);
        assertD(sizeL >= 0); // -1 indicates error
	    buffer.resize(sizeL);
        failed = fseek(fp, 0, SEEK_SET);
        assertD(!failed);
    }
    if(!buffer.empty())
    {
        size_t amountRead = fread(&buffer.front(), 1, buffer.size(), fp);
        assertD(amountRead == buffer.size());
    }
    int failed = fclose(fp);
    assertD(!failed);
}

// caller must delete [] the returned buffer
inline char*
LoadBinary_AllocatedBuffer(const char* fileName, tUnsigned32& size)
{
    size = 0;
    FILE* fp = fopen(fileName, "rb");
    if(!fp)
    {
        const char* attributes[3];
        attributes[0] = "fileName";
        attributes[1] = fileName;
        attributes[2] = 0;
        Error("NonFatal", "LoadBinary_AllocatedBuffer: Failed to open file. Returning 0.", attributes);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = new char[size + 1];
    fread(buffer, 1, size, fp);
    buffer[size] = 0;
    fclose(fp);

    return buffer;
}

inline bool
SaveBinary(const char* fileName, const char* buffer, tUnsigned32 bufferSize)
{
    FILE* fp = fopen(fileName, "wb");
    if(!fp)
    {
        const char* attributes[3];
        attributes[0] = "fileName";
        attributes[1] = fileName;
        attributes[2] = 0;
        Error("NonFatal", "SaveBinary: Failed to open file. Returning failure.", attributes);
        return false;
    }
    if(bufferSize > 0)
    {
        tSigned32 amountWritten = static_cast<tSigned32>(fwrite(buffer, 1, bufferSize, fp));
        if(amountWritten != bufferSize)
        {
            const char* attributes[3];
            attributes[0] = "fileName";
            attributes[1] = fileName;
            attributes[2] = 0;
            Error("NonFatal", "SaveBinary: Failed writing file.", attributes);
            fclose(fp);
            return false;
        }
    }
    int failure = fclose(fp);
    if(failure)
    {
        const char* attributes[3];
        attributes[0] = "fileName";
        attributes[1] = fileName;
        attributes[2] = 0;
        Error("NonFatal", "SaveBinary: Failed closing file.", attributes);
        return false;
    }
    return true;
}

inline bool
SaveBinary(const char* fileName, const std::vector<char>& buffer)
{
    if(buffer.empty())
    {
        return SaveBinary(fileName, 0, static_cast<tUnsigned32>(buffer.size()));
    }
    return SaveBinary(fileName, &buffer.front(), static_cast<tUnsigned32>(buffer.size()));
}

#endif
