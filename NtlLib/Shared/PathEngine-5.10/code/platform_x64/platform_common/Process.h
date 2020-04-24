//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/WindowsOSHeader.h"

inline void Platform_DebugBreak()
{
    DebugBreak();
}
inline void Platform_Abort()
{
    _exit(1);
}

