//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOAD_ANCHORS_AND_SHAPES_SET_INCLUDED
#define LOAD_ANCHORS_AND_SHAPES_SET_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class iAnchorsAndPinnedShapes;
class iXMLInputStream;

class cAnchorsAndShapesSetLoader : public iXMLHandler
{
    std::vector<const iAnchorsAndPinnedShapes*> _result;

public:

    ~cAnchorsAndShapesSetLoader();

    tSigned32 size() const
    {
        return SizeL(_result);
    }
    const iAnchorsAndPinnedShapes* operator[](tSigned32 i) const
    {
        return _result[i];
    }

// iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);
};

#endif
