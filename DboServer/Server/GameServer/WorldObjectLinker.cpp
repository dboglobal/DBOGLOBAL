#include "stdafx.h"
#include "WorldObjectLinker.h"
#include "SpawnObject.h"


CWorldObjectLinker::CWorldObjectLinker()
{
	m_pOwner = NULL;
}

CWorldObjectLinker::~CWorldObjectLinker()
{
}


eOBJTYPE CWorldObjectLinker::GetObjType()
{
	if (!m_pOwner)
	{
		ERR_LOG(LOG_USER, "m_pOwner is NULL");
	}

	return m_pOwner->GetObjType();
}