//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MAYA_ERROR_HANDLER_INCLUDED
#define MAYA_ERROR_HANDLER_INCLUDED

#include "i_pathengine.h"
#include "common/STL/string.h"
#include "common/STL/vector.h"

class MDagPath;
class cCombinedFaceVertexMeshes;

class cMayaErrorHandler : public iErrorHandler
{
    std::string _contextDescription;
    std::vector<MDagPath>* _contextDagPaths;
    cCombinedFaceVertexMeshes* _contextCombinedMeshes;
    bool _objectsSelected;

    cMayaErrorHandler()
    {
    }

public:

    static cMayaErrorHandler& refInstance();

    void setContext(const std::string& description, std::vector<MDagPath>* dagPaths, cCombinedFaceVertexMeshes* combinedMeshes = 0);
    void clearContext();

// interface for iErrorHandler

    eAction handle(const char* errorType, const char* errorString, const char *const* attributes);
};

#endif
