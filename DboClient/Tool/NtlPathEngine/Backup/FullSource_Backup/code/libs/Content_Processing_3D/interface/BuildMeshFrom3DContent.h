//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__CONTENT_PROCESSING_3D_INCLUDED
#define CONTENT_PROCESSING_3D__CONTENT_PROCESSING_3D_INCLUDED

class iMesh;
class iFaceVertexMesh;
class iProgressCallBack;
class iSolidObjects;

iMesh*
BuildMeshFrom3DContent(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        iProgressCallBack* progressCallBack
        );

#endif
