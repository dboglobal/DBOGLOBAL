//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXTENDED_RESOURCE_LOADER_INCLUDED
#define EXTENDED_RESOURCE_LOADER_INCLUDED

#include "platform_common/LoadSaveBinary.h"
#include "common/interface/iExtendedResourceLoader.h"
#include "common/STL/string.h"
#include <windows.h>
#include <stdio.h>

class cExtendedResourceLoader : public iExtendedResourceLoader
{
    cExtendedResourceLoader() {}

public:

    void saveResource(const char* filename, const char* buffer, tUnsigned32 bufferSize)
    {
        std::string fullFileName = "..\\resource\\";
        fullFileName += filename;
        SaveBinary(fullFileName.c_str(), buffer, bufferSize);
    }
    void getDirectoryListing(const char* path, const char*& listingBuffer, tUnsigned32& listingSize)
    {
        std::string fullPath = "..\\resource\\";
        fullPath += path;
        fullPath += "\\*";

        std::vector<std::string> listing;
        tUnsigned32 bufferSize = 0;
        listingSize = 0;

        WIN32_FIND_DATA data; 
        HANDLE handle; 
 
        handle = FindFirstFile(fullPath.c_str(), &data); 
        assertD(handle != INVALID_HANDLE_VALUE);
        do
        { 
            if(data.cFileName[0] == '.')
            {
                if(data.cFileName[1] == 0 || (data.cFileName[1] == '.' && data.cFileName[2] == 0))
                {
                    continue;
                }
            }
            if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }
            listing.push_back(data.cFileName);
            bufferSize += SizeL(listing.back()) + 1;
            listingSize++;
        }
        while(FindNextFile(handle, &data)) ;
        assertD(GetLastError() == ERROR_NO_MORE_FILES);
        char* result = new char[bufferSize];
        tSigned32 i;
        tUnsigned32 pos = 0;
        for(i = 0; i != listingSize; ++i)
        {
            tSigned32 j;
            for(j = 0; j != listing[i].size(); ++j)
            {
                result[pos++] = listing[i][j];
            }
            result[pos++] = 0;
        }
        listingBuffer = const_cast<const char*>(result);
    }

    static cExtendedResourceLoader& refInstance()
    {
        static cExtendedResourceLoader theInstance;
        return theInstance;
    }
};

#endif
