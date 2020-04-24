//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_FACES_CAN_BE_COMBINED_CALLBACK_INCLUDED
#define I_FACES_CAN_BE_COMBINED_CALLBACK_INCLUDED

#include "tMesh_Dynamic.h"

class iFacesCanBeCombinedCallback
{
public:
    virtual ~iFacesCanBeCombinedCallback()
    {
    }
    virtual bool operator()(tFace_Dynamic f1, tFace_Dynamic f2) = 0;
};

#endif
