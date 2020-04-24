//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER__LOGGER_INCLUDED
#define QUERY_LOGGING_WRAPPER__LOGGER_INCLUDED

#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include <stdio.h>

class iFaceVertexMesh;
class iAgent;
class iContentChunkInstance;
class cPosition;
class cHorizontalRange;

class cLogger
{
    cMutex _mutex;
    FILE* _file;
    std::vector<char> _buffer;
    tUnsigned32 _queryCount;
    tUnsigned32 _fileIndex_DataBuffer;
    tUnsigned32 _fileIndex_SourceContent;

    void writeBuffer();
    void signedToBuffer(tSigned32 value, tSigned32 tokenSize);
    void unsignedToBuffer(tUnsigned32 value, tSigned32 tokenSize);
    void boolToBuffer(bool value);
    void pointerToBuffer(void* voidPointer);

public:

    cLogger() :
      _buffer(500)
    {
        _buffer.resize(0);
    }

    void initialise();
    void shutdown();

    tUnsigned32 methodEntry(tSigned32 interfaceIndex, tSigned32 methodIndex, void* interfacePointer);
    void methodEntry_Completed();
    void methodExit(tSigned32 interfaceIndex, tSigned32 methodIndex, tUnsigned32 queryCount, void* interfacePointer);
    void methodExit_Completed();

    void log(bool value);
    void log(tSigned32 value);
    void log(tUnsigned32 value);
    void log(float value);
    void log(const char* string);
    void log(const char *const* attributes);
    void log(const tSigned32* point);
    void log(const float* point);
    void log(const cPosition& position);
    void log(const cHorizontalRange& range);
    void log(void* voidPointer);
    void logCollidingLine(tSigned32* collidingLine);
    void logCollisionShape(tSigned32 numberOfPoints, const tSigned32* coordinateData);
    void logDataBuffer(const char* dataBuffer, tUnsigned32 bufferSize);
    void logPathBuffers(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength);
    void logSourceContent(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects);
    void logAgentVector(iAgent** resultsBuffer, tSigned32& numberOverlapped);
    void logContentChunkInstanceVector(const iContentChunkInstance* const* instancePointers, tSigned32 numberOfInstances);
    void logLongVector(tSigned32* resultsBuffer, tSigned32& numberOverlapped);
};

extern cLogger gLogger;

#endif
