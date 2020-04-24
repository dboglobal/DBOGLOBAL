//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CornerProvider.h"
#include "platform_common/CoordinateRanges.h"
#include "common/STL_Helpers.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Attributes.h"

cCornerProvider::cCornerProvider(tMesh* mesh) :
 _mesh(mesh)
{
}

void
cCornerProvider::clearWithoutFreeingBuffers()
{
    _pointAndAxisData.resize(0);
    _faces.resize(0);
}

void
cCornerProvider::completelyClear()
{
    CompletelyClear(_pointAndAxisData);
    CompletelyClear(_faces);
}

void
cCornerProvider::shrinkToFit()
{
    ShrinkToFit(_pointAndAxisData);
    ShrinkToFit(_faces);
}

void
cCornerProvider::add(const cCornerProvider& rhs)
{
    tSigned32 sizeBefore = size();
    _pointAndAxisData.resize((sizeBefore + rhs.size()) * 3);\
    _faces.resize(sizeBefore + rhs.size());
    tSigned32 i;
    for(i = 0; i < rhs.size(); ++i)
    {
        _pointAndAxisData[(sizeBefore + i) * 3 + 0] = rhs._pointAndAxisData[i * 3 + 0];
        _pointAndAxisData[(sizeBefore + i) * 3 + 1] = rhs._pointAndAxisData[i * 3 + 1];
        _pointAndAxisData[(sizeBefore + i) * 3 + 2] = rhs._pointAndAxisData[i * 3 + 2];
        _faces[sizeBefore + i] = rhs._faces[i];
    }
}

void
cCornerProvider::addFiltered(const cCornerProvider& rhs, const tPoint& boundsMin, const tPoint& boundsMax)
{
    tSigned32 sizeBefore = size();
    _pointAndAxisData.reserve((sizeBefore + rhs.size()) * 3);\
    _faces.reserve(sizeBefore + rhs.size());
    tSigned32 i;
    for(i = 0; i < rhs.size(); ++i)
    {
        if(rhs.cornerPoint(i).insideOrOnBounds(boundsMin, boundsMax))
        {
            _pointAndAxisData.push_back(rhs._pointAndAxisData[i * 3 + 0]);
            _pointAndAxisData.push_back(rhs._pointAndAxisData[i * 3 + 1]);
            _pointAndAxisData.push_back(rhs._pointAndAxisData[i * 3 + 2]);
            _faces.push_back(rhs._faces[i]);
        }
    }
}

void
cCornerProvider::sort()
{
    std::vector<tSigned32> indices(size());
    tSigned32 i;
    for(i = 0; i != size(); ++i)
    {
        indices[i] = i;
    }
    std::sort(indices.begin(), indices.end(), *this);

//.. optimise - could rearrange in place

    std::vector<tPoint> newPointAndAxisData(size() * 3);
    std::vector<tSigned32> newFaces(size());
    for(i = 0; i != size(); ++i)
    {
        newPointAndAxisData[i * 3] = _pointAndAxisData[indices[i] * 3];
        newPointAndAxisData[i * 3 + 1] = _pointAndAxisData[indices[i] * 3 + 1];
        newPointAndAxisData[i * 3 + 2] = _pointAndAxisData[indices[i] * 3 + 2];
        newFaces[i] = _faces[indices[i]];
    }
    _pointAndAxisData.swap(newPointAndAxisData);
    _faces.swap(newFaces);
}

void
cCornerProvider::save(tSigned32 numberToSave, iXMLOutputStream& os) const
{
    tSigned32 i;
    for(i = 0; i != numberToSave; ++i)
    {
        os.openElement("corner");
        os.addAttribute("face", cornerFace(i).index());
        os.addAttribute("x", cornerPoint(i).getX());
        os.addAttribute("y", cornerPoint(i).getY());
        os.addAttribute("axisBeforeX", axisBefore(i).getX());
        os.addAttribute("axisBeforeY", axisBefore(i).getY());
        os.addAttribute("axisAfterX", axisAfter(i).getX());
        os.addAttribute("axisAfterY", axisAfter(i).getY());
        os.closeElement("corner");
    }
}
void
cCornerProvider::load(iXMLInputStream& is)
{
    const char* nextElement = is.peekNextElement();
    while(nextElement && strcmp(nextElement, "corner") == 0)
    {
        const char *const* attributes = is.openNextElement();
        tFace f = _mesh->faceAt(ValueAsLong(attributes, "face"));
        tSigned32 x, y;
        x = ValueAsLong(attributes, "x");
        y = ValueAsLong(attributes, "y");
        tPoint p(x, y, WORLD_RANGE);
        x = ValueAsLong(attributes, "axisBeforeX");
        y = ValueAsLong(attributes, "axisBeforeY");
        tPoint axisBefore(x, y, WORLD_RANGE * 2);
        x = ValueAsLong(attributes, "axisAfterX");
        y = ValueAsLong(attributes, "axisAfterY");
        tPoint axisAfter(x, y, WORLD_RANGE * 2);
        push_back(f, p, axisBefore, axisAfter);
        is.closeElement();
        nextElement = is.peekNextElement();
    }
}

static int
CompareDirections(const tPoint& lhs, const tPoint& rhs)
{
    const tPoint zeroAxis(0, 1, 1);
    eDirection lhsDir = zeroAxis.direction(lhs);
    eDirection rhsDir = zeroAxis.direction(rhs);
    if(lhsDir == rhsDir)
    {
        eSide side = lhs.side(rhs);
        if(side == SIDE_CENTRE)
        {
            return 0;
        }
        if(side == SIDE_LEFT)
        {
            return -1;
        }
        return 1;
    }
    if(rhsDir > lhsDir)
    {
        return 1;
    }
    return -1;
}

bool
cCornerProvider::operator()(tSigned32 lhsIndex, tSigned32 rhsIndex) const
{
    if(lhsIndex == rhsIndex)
    {
        return false;
    }
    if(_faces[lhsIndex] != _faces[rhsIndex])
    {
        return _faces[rhsIndex] > _faces[lhsIndex]; 
    }
    tPoint lhsP = cornerPoint(lhsIndex);
    tPoint rhsP = cornerPoint(rhsIndex);
    if(lhsP.getX() != rhsP.getX())
    {
        return rhsP.getX() > lhsP.getX();
    }
    if(lhsP.getY() != rhsP.getY())
    {
        return rhsP.getY() > lhsP.getY();
    }
    int comparison;
    comparison = CompareDirections(axisBefore(lhsIndex), axisBefore(rhsIndex));
    if(comparison != 0)
    {
        return comparison == 1;
    }
    comparison = CompareDirections(axisAfter(lhsIndex), axisAfter(rhsIndex));
    return comparison == 1;
}
