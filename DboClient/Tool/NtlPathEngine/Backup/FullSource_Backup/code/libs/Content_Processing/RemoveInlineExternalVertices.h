//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class iFacesCanBeJoinedCallBack
{
public:
    virtual ~iFacesCanBeJoinedCallBack()
    {
    }
    virtual bool facesCanBeJoined(tEdge_3D_FaceTracked e) const = 0;
};

void RemoveInlineExternalVertices(
                tMesh_3D_FaceTracked& mesh,
                float horizontalThreshold,
                float verticalThreshold,
                iFacesCanBeJoinedCallBack& facesCallBack
                );
