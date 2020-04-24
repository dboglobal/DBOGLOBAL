//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "common/CLib/stdlib.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class Interface;
class cMeshesForSelectionSet;
class cCombinedFaceVertexMeshes;

class cValidationHandler : public iErrorHandler
{
    const char* _contextDescription;
    cMeshesForSelectionSet* _meshes;
    cCombinedFaceVertexMeshes* _contextCombinedMeshes;
    Interface* _ip;
    bool _logToFileEnabled;
    std::string _logFileName;
    bool _attemptedToOpenLogFile;
    FILE* _logFile;
    bool _skipFurtherErrorsInThisContext;
    bool _facesSelectedForThisContext;
    bool _vertsSelectedForThisContext;
    bool _wasConvertedErrorEmittedForThisContext;

    void logString(const std::string& toLog);
    void logErrorIfLogEnabled(const std::string& title, const std::string& description);

public:

    cValidationHandler()
    {
        _contextDescription = 0;
        _meshes = 0;
        _contextCombinedMeshes = 0;
        _logToFileEnabled = false;
        _skipFurtherErrorsInThisContext = false;
    }
    ~cValidationHandler();

    void setContext(
        const char* description,
        cMeshesForSelectionSet* meshes,
        Interface* ip,
        cCombinedFaceVertexMeshes* combinedMeshes = 0
        )
    {
        _contextDescription = description;
        _meshes = meshes;
        _contextCombinedMeshes = combinedMeshes;
        _ip = ip;
        _facesSelectedForThisContext = false;
        _vertsSelectedForThisContext = false;
        _wasConvertedErrorEmittedForThisContext = false;
    }
    void setContext(const char* description)
    {
        _contextDescription = description;
        _meshes = 0;
        _contextCombinedMeshes = 0;
        _facesSelectedForThisContext = false;
        _vertsSelectedForThisContext = false;
        _wasConvertedErrorEmittedForThisContext = false;
    }
    void clearContext()
    {
        _contextDescription = 0;
        _meshes = 0;
        _contextCombinedMeshes = 0;
        _skipFurtherErrorsInThisContext = false;
    }

    void enableLogToFile(const std::string& logFileName);
    void disableLogToFile();

// interface for iErrorHandler

    eAction handle(const char* errorType, const char* errorString, const char *const* attributes);
};
