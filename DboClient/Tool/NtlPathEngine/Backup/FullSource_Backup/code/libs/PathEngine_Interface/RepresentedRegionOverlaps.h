//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPRESENTED_REGION_OVERLAPS_INCLUDED
#define REPRESENTED_REGION_OVERLAPS_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/Containers/CollapsedVectorVector.h"

class iXMLOutputStream;

class cRepresentedRegionOverlaps
{
    friend class cRepresentedRegionOverlapsLoader;
    cCollapsedVectorVector<tSigned32> _overlaps;

    cRepresentedRegionOverlaps()
    {
    }

public:

    cRepresentedRegionOverlaps(
            const cCollapsedVectorVector<tSigned32>& subsetsLookup,
            tSigned32 subsetIndex
            );

    void save(iXMLOutputStream& os) const;

    tSigned32 lookupOverlappedFace(tSigned32 sourceFaceIndex, tSigned32 targetRegion) const
    {
        assertD(sourceFaceIndex >= 0 && sourceFaceIndex < _overlaps.size());
        const tSigned32 *const subVector = _overlaps[sourceFaceIndex];
        for(tSigned32 i = 0; i != _overlaps.subVectorSize(sourceFaceIndex); i += 2)
        {
            if(subVector[i] == targetRegion)
            {
                return subVector[i + 1];
            }
        }
        invalid();
        return -1;
    }
};

class cRepresentedRegionOverlapsLoader : public iXMLHandler
{
    cRepresentedRegionOverlaps* _result;
    tSigned32 _loadedSize;
    tSigned32 _currentFace;

public:

    cRepresentedRegionOverlapsLoader();
    ~cRepresentedRegionOverlapsLoader();

    const cRepresentedRegionOverlaps* takeResult();

// interface for iXMLHandler

    void init(const char *const* attr);
    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
