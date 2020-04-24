//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_BUILDING_OUTPUT_STREAM
#define VECTOR_BUILDING_OUTPUT_STREAM

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cVectorBuildingOutputStream : public iOutputStream
{
public:

    std::vector<char> _buffer;

// iOutputStream

    void put(const char* data, tUnsigned32 dataSize)
    {
        size_t sizeBefore = _buffer.size();
        size_t sizeAfter = _buffer.size() + dataSize;
        _buffer.resize(sizeAfter);
        size_t i;
        for(i = sizeBefore; i < sizeAfter; ++i)
        {
            _buffer[i] = *data++;
        }
    }
};

#endif
