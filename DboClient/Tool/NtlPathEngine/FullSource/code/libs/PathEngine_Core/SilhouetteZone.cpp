//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SilhouetteZone.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"

eSilhouetteZone ZoneFor(
        const tLine &before, const tLine &after,
        const tPoint &p
        )
{
    if(before.side(p)!=SIDE_RIGHT)
    {
        if(after.side(p)==SIDE_RIGHT)
            return SZ_LEFTSILHOUETTE;
        return SZ_NOTSILHOUETTE;
    }
    else
    {
        if(after.side(p)!=SIDE_RIGHT)
            return SZ_RIGHTSILHOUETTE;
        return SZ_NOTSILHOUETTE;
    }
}

eSilhouetteZone ZoneFor(
        const tPoint &axis_before, const tPoint &axis_after,
        const tPoint &offset
        )
{
    tLine before(-axis_before,tPoint(0,0,0));
    tLine after(tPoint(0,0,0),axis_after);
    return ZoneFor(before,after,offset);
}

