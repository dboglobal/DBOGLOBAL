//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef RANDOM_SEQUENCE_INCLUDED
#define RANDOM_SEQUENCE_INCLUDED

#include "common/interface/iRandomSequence.h"

class cRandomSequence : public iRandomSequence
{
    tSigned32 _seed;

public:

    cRandomSequence();

// iRandomSequence

    tSigned32 getSeed() const;
    void setSeed(tSigned32 value);
    tSigned32 random15();
    tSigned32 random31();
};

#endif
