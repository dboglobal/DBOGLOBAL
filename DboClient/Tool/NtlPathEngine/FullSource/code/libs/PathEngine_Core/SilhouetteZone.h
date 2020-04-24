//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SILHOUETTEZONE_INCLUDED
#define SILHOUETTEZONE_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "common/interface/Assert.h"

enum eSilhouetteZone
{
    SZ_NOTSILHOUETTE,
    SZ_LEFTSILHOUETTE,
    SZ_RIGHTSILHOUETTE,
};
eSilhouetteZone ZoneFor(
        const tLine &before, const tLine &after,
        const tPoint &p
        );
eSilhouetteZone ZoneFor(
        const tPoint &axis_before, const tPoint &axis_after,
        const tPoint &offset
        );

inline eSilhouetteZone OppositeZone(eSilhouetteZone zone)
{
    assertD(zone!=SZ_NOTSILHOUETTE);
    if(zone==SZ_LEFTSILHOUETTE)
        return SZ_RIGHTSILHOUETTE;
    return SZ_LEFTSILHOUETTE;
}

#endif //include guard
