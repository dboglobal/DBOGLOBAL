//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_EXTENDED_RESOURCE_LOADER_INCLUDED
#define I_EXTENDED_RESOURCE_LOADER_INCLUDED

class iExtendedResourceLoader
{
public:
    virtual ~iExtendedResourceLoader() {}
    virtual void saveResource(const char* filename, const char* buffer, tUnsigned32 bufferSize) = 0;
    virtual void getDirectoryListing(const char* path, const char*& listing, tUnsigned32& listingSize) = 0;
};

#endif
