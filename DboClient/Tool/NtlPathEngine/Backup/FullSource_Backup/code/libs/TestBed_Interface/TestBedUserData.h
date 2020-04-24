//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TEST_BED_USER_DATA_INCLUDED
#define TEST_BED_USER_DATA_INCLUDED

#include "LineCollidesWithTriangle.h"
#include "common/interface/iUserData.h"
#include "common/STL/vector.h"

class cMeshRenderInfo;
class cGraphicsInterfaceRecorder;
class cObstacleRenderInfo;
class cTestBedUserData : public iUserData
{
public:

	cTestBedUserData();
	~cTestBedUserData();

	cMeshRenderInfo* _meshRenderInfo;
	cGraphicsInterfaceRecorder* _baseCircuitsRecorder;
	cObstacleRenderInfo* _obstacleRednerInfo;
	std::vector<nLineCollidesWithTriangle::cTriPreprocess> _losPreprocess;
};

#endif
