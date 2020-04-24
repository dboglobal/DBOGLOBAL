#include "stdafx.h"
#include "WorldFactory.h"
#include "World.h"


CWorldFactory::CWorldFactory()
{
}

CWorldFactory::~CWorldFactory()
{
}


CWorld* CWorldFactory::Create()
{
	CWorld* pWorld = new CWorld;

	return pWorld;
}

void CWorldFactory::Destroy(CWorld *pWorld)
{
	SAFE_DELETE(pWorld);
}
