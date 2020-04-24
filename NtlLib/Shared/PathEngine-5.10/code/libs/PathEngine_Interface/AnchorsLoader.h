//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ANCHORS_LOADER_INCLUDED
#define ANCHORS_LOADER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "i_pathengine.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

class cAnchorInfo
{
public:

    std::string _id;
    cPosition _position;
    tSigned32 _orientation;

    cAnchorInfo(const std::string& id, const cPosition& position, tSigned32 orientation) :
      _id(id),
      _position(position),
      _orientation(orientation)
    {
    }
};

class cAnchorsLoader : public iXMLHandler
{
public:

    std::vector<cAnchorInfo> _infos;

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
