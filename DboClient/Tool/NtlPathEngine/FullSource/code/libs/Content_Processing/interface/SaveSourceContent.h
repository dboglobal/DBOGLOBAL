//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SAVE_SOURCE_CONTENT_INCLUDED
#define SAVE_SOURCE_CONTENT_INCLUDED

#include "common/STL/vector.h"

class iFaceVertexMesh;
class iAnchorsAndPinnedShapes;
class iXMLOutputStream;

void
SaveSourceContent(
        const std::vector<const iFaceVertexMesh*>& meshes,
        const iAnchorsAndPinnedShapes* anchorsAndShapes,
        iXMLOutputStream& xos
        );

void
SaveSourceContent(
        const std::vector<const iFaceVertexMesh*>& meshes,
		const std::vector<const iAnchorsAndPinnedShapes*>& anchorsAndShapes,
        iXMLOutputStream& xos
        );

#endif
