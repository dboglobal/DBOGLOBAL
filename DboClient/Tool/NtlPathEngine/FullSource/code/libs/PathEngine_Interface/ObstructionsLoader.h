//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OBSTRUCTIONS_LOADER_INCLUDED
#define OBSTRUCTIONS_LOADER_INCLUDED

#include "libs/PathEngine_Core/interface/ObstacleInfo.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "i_pathengine.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

class cObstructionsLoader : public iXMLHandler
{
public:

    std::vector<std::string> _ids;
    std::vector<cObstacleInfo> _infos;    // _position is not filled in
    std::vector<cConvexPoly> _shapes;
    std::vector<cPosition> _loadedPositions;

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
