//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_RESOURCE_LOADER_INCLUDED
#define I_RESOURCE_LOADER_INCLUDED

class iResourceLoader
{
public:
    virtual ~iResourceLoader() {}
    virtual void loadResource(const char* filename, const char*& buffer, tUnsigned32& bufferSize) = 0;
    virtual void freeBuffer(const char *buffer) = 0;
};

#endif
