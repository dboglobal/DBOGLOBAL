//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
#define I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
class iOutputStream
{
public:
    virtual ~iOutputStream() {};
    virtual void put(const char *data, tUnsigned32 data_size)=0;
};
class cNullOutputStream : public iOutputStream
{
public:
    void put(const char *data, tUnsigned32 data_size) {}
};
#endif
