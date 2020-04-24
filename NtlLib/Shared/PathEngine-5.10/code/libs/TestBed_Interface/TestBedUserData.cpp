//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "TestBedUserData.h"
#include "interface/MeshRenderInfo.h"
#include "GraphicsInterfaceRecorder.h"

cTestBedUserData::cTestBedUserData()
{
//    _meshRenderInfo = new cMeshRenderInfo;
//    _baseCircuitsRecorder = new cGraphicsInterfaceRecorder(gip);
    _meshRenderInfo = 0;
    _baseCircuitsRecorder = 0;
}

cTestBedUserData::~cTestBedUserData()
{
    delete _meshRenderInfo;
    delete _baseCircuitsRecorder;
}
