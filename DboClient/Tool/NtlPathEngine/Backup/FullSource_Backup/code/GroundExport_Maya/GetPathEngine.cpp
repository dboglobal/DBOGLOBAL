//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Content_Processing/interface/GetPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"

iPathEngine&
GetPathEngine()
{
    return nSingletons::pathEngineI();
}
