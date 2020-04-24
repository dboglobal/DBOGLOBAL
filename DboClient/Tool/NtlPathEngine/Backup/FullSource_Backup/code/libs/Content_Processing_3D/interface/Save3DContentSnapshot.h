//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class iFaceVertexMesh;
class iSolidObjects;
class iOutputStream;

void
Save3DContentSnapshot(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        const char* format,
        iOutputStream* os
        );
