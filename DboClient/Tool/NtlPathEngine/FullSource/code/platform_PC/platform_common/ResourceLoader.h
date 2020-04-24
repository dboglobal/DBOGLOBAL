//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef RESOURCE_LOADER_INCLUDED
#define RESOURCE_LOADER_INCLUDED

#include "platform_common/LoadSaveBinary.h"
#include "common/interface/iResourceLoader.h"
#include "common/STL/string.h"

class cResourceLoader : public iResourceLoader
{
    cResourceLoader() {}

public:

    void loadResource(const char* filename, const char*& buffer, tUnsigned32& bufferSize)
    {
        std::string fullFileName = "..\\resource\\";
        fullFileName += filename;
        buffer = LoadBinary_AllocatedBuffer(fullFileName.c_str(), bufferSize);
    }
    void freeBuffer(const char *buffer)
    {
        delete [] buffer;
    }

    static cResourceLoader& refInstance()
    {
        static cResourceLoader theInstance;
        return theInstance;
    }
};

#endif
