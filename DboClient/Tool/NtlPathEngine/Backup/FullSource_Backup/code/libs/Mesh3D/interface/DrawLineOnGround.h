//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"

class iPointLike;
class cVector2F;
class iRender3DLinesCallBack;

void
DrawLineOnGround(tFace_3D f, const tLine& line, eSide spinThroughSide,
                 iPointLike& target, const cVector2F& endP,
                 const tPoint& origin,
                 iRender3DLinesCallBack& callBack);

void
DrawLineOnGround(tFace_3D f, const tLine& line, eSide spinThroughSide,
                 const tPoint& origin, bool connectsToPrevious,
                 iRender3DLinesCallBack& callBack);

