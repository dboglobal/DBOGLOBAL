//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_RANDOM_SEQUENCE_HAS_BEEN_INCLUDED
#define I_RANDOM_SEQUENCE_HAS_BEEN_INCLUDED
class iRandomSequence
{
public:

    virtual ~iRandomSequence()
    {
    }
    
    virtual tSigned32 getSeed() const = 0;
    virtual void setSeed(tSigned32 value) = 0;
    virtual tSigned32 random15() = 0;
    virtual tSigned32 random31() = 0;
};
#endif
