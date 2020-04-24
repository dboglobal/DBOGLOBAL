//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "RepresentedRegionOverlaps.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLOutputStream.h"

cRepresentedRegionOverlaps::cRepresentedRegionOverlaps(
            const cCollapsedVectorVector<tSigned32>& subsetsLookup,
            tSigned32 subsetIndex
            )
{
    for(tSigned32 i = 0; i != subsetsLookup.size(); ++i)
    {
        const tSigned32 *const subVector = subsetsLookup[i];
        tSigned32 j;
        for(j = 0; j != subsetsLookup.subVectorSize(i); j += 2)
        {
            if(subVector[j] == subsetIndex)
            {
                break;
            }
        }
        if(j == subsetsLookup.subVectorSize(i))
        {
          // face i in the world mesh does not overlap subsetIndex
            continue;
        }
        assertD(subVector[j + 1] == _overlaps.size());
        _overlaps.pushBackSubVector();
        for(tSigned32 j = 0; j != subsetsLookup.subVectorSize(i); j += 2)
        {
            if(subVector[j] == subsetIndex)
            {
                continue;
            }
            // copy tile overlapped and index in tile overlapped
            _overlaps.pushBackInLastSubVector(subVector[j]);
            _overlaps.pushBackInLastSubVector(subVector[j + 1]);
        }
    }
}

void
cRepresentedRegionOverlaps::save(iXMLOutputStream& os) const
{
    os.addAttribute("size", _overlaps.size());
    for(tSigned32 i = 0; i != _overlaps.size(); ++i)
    {
        if(_overlaps.subVectorSize(i) == 0)
        {
            continue;
        }
        os.openElement("faceOverlaps");
        os.addAttribute("sourceIndex", i);
        for(tSigned32 j = 0; j != _overlaps.subVectorSize(i); j += 2)
        {
            os.openElement("target");
            os.addAttribute("tileIndex", _overlaps[i][j]);
            os.addAttribute("indexInTile", _overlaps[i][j + 1]);
            os.closeElement("target");
        }
        os.closeElement("faceOverlaps");
    }
}

cRepresentedRegionOverlapsLoader::cRepresentedRegionOverlapsLoader()
{
    _loadedSize = -1;
    _result = 0;
}
cRepresentedRegionOverlapsLoader::~cRepresentedRegionOverlapsLoader()
{
    delete _result;
}
const cRepresentedRegionOverlaps*
cRepresentedRegionOverlapsLoader::takeResult()
{
    if(_result == 0)
    {
        return 0;
    }
    assertD(_result->_overlaps.size() <= _loadedSize);
    while(_result->_overlaps.size() < _loadedSize)
    {
        _result->_overlaps.pushBackSubVector();
    }
    const cRepresentedRegionOverlaps* result = _result;
    _result = 0;
    return result;
}

void
cRepresentedRegionOverlapsLoader::init(const char *const* attr)
{
    _loadedSize = ValueAsLong(attr, "size");
}

iXMLHandler*
cRepresentedRegionOverlapsLoader::element(const char* el, const char *const* attributes)
{
    assertD(_loadedSize != -1); // check init() was called
    if(getDepth() == 0)
    {
        if(strcmp(el, "faceOverlaps") == 0)
        {
            if(_result == 0)
            {
                _result = new cRepresentedRegionOverlaps;
            }
            _currentFace = ValueAsLong(attributes, "sourceIndex");
            while(_result->_overlaps.size() <= _currentFace)
            {
                _result->_overlaps.pushBackSubVector();
            }
            return this;
        }
        return 0;
    }
    if(strcmp(el, "target") != 0)
    {
        return 0;
    }
    _result->_overlaps.pushBackInLastSubVector(ValueAsLong(attributes, "tileIndex"));
    _result->_overlaps.pushBackInLastSubVector(ValueAsLong(attributes, "indexInTile"));
    return 0;
}
