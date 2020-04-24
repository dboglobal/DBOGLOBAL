//***********************************************************************************
//	File		:	NtlResourceHurricaneSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlresourcehurricanesystem.h"

#include "NtlDebug.h"

CNtlResourceHurricaneSystem::CNtlResourceHurricaneSystem(const RwChar* strName) : CNtlResourceImVertexSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_HURRICANE;

	memset(&m_EmitterStandard, 0, sizeof(SNtlHurricaneEmitterStandard));
}

CNtlResourceHurricaneSystem::~CNtlResourceHurricaneSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceHurricaneSystem::Destroy()
{
	CNtlResourceImVertexSystem::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceHurricaneSystem::Load(FILE* pFile)
{
	CNtlResourceImVertexSystem::Load(pFile);

	fread(&m_EmitterStandard,				sizeof(SNtlHurricaneEmitterStandard),		1,			pFile);

	return TRUE;
}