//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COUNTING_OUTPUT_STREAM
#define COUNTING_OUTPUT_STREAM

#include "i_pathengine.h"

class cCountingOutputStream : public iOutputStream
{
public:

    cCountingOutputStream()
    {
        m_size = 0;
    }

    tUnsigned32 m_size;

    virtual void put(const char *data, tUnsigned32 data_size)
    {
        m_size += data_size;
    }
};

#endif
