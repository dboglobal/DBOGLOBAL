//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj

#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cAgent.h"
#include "libs/PathEngine_Interface/interface/cCollisionContext.h"
#include "libs/PathEngine_Interface/interface/cObstacleSet.h"
#include "libs/PathEngine_Interface/interface/cPath.h"
#include "libs/PathEngine_Interface/interface/cCollisionInfo.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "libs/PathEngine_Interface/interface/cContentChunkInstance.h"
#include "libs/PathEngine_Interface/interface/cMeshFederation.h"

extern "C"
{
    iAPIObjectWrapper** __stdcall DLLExport_RefWrapperPointer(tSigned32 interfaceIndex, void* p)
    {
        switch(interfaceIndex)
        {
        default:
            invalid();
            return 0;
        case 0:
            invalid();
            return 0;
        case 1:
            return &static_cast<cShape*>((iShape*)p)->_wrapper;
        case 2:
            return &static_cast<cMesh*>((iMesh*)p)->_wrapper;
        case 3:
            return &static_cast<cAgent*>((iAgent*)p)->_wrapper;
        case 4:
            return &static_cast<cCollisionContext*>((iCollisionContext*)p)->_wrapper;
        case 5:
            return &static_cast<cObstacleSet*>((iObstacleSet*)p)->_wrapper;
        case 6:
            return &static_cast<cPath*>((iPath*)p)->_wrapper;
        case 7:
            return &static_cast<cCollisionInfo*>((iCollisionInfo*)p)->_wrapper;
        case 8:
            invalid();
            return 0;
        case 9:
            invalid();
            return 0;
        case 10:
            return &static_cast<cContentChunk*>((iContentChunk*)p)->_wrapper;
        case 11:
            return &static_cast<cContentChunkInstance*>((iContentChunkInstance*)p)->_wrapper;
        case 12:
            return &static_cast<cMeshFederation*>((iMeshFederation*)p)->_wrapper;
        }
    }
}
