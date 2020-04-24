//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MEMORY_REPORTS_INCLUDED
#define MEMORY_REPORTS_INCLUDED

#ifdef MEMORY_REPORTS_ON

#include "libs/Instrumentation/interface/Log.h"

tUnsigned32 GetTotalMemoryAllocated();

class cMemoryReportScope
{
    tUnsigned32 _allocatedAtStart;
    const char* _id;

public:

    cMemoryReportScope(const char* id)
    {
        _allocatedAtStart = GetTotalMemoryAllocated();
        _id = id;
    }
    ~cMemoryReportScope()
    {
        tUnsigned32 allocated = GetTotalMemoryAllocated();
        if(allocated > _allocatedAtStart)
        {
            allocated -= _allocatedAtStart;
            Log("MEMORY ALLOCATED FOR '");
        }
        else
        {
            allocated = _allocatedAtStart - allocated;
            Log("MEMORY FREED FOR '");
        }
        Log(_id);
        Log("' = ");
        Log(allocated);
        Log("\n");
    }
};

#define MEMORY_REPORT_SCOPE(id) cMemoryReportScope memory_report_scope_object(id)

#else // MEMORY_REPORTS_ON
    
#define MEMORY_REPORT_SCOPE(id) do{}while(0)

#endif // MEMORY_REPORTS_ON

#endif // include guard

