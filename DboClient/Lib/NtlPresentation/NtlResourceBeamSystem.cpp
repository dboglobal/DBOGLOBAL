//***********************************************************************************
//	File		:	NtlResourceBeamSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlresourcebeamsystem.h"

#include "NtlDebug.h"

CNtlResourceBeamSystem::CNtlResourceBeamSystem(const RwChar* strName) : CNtlResourceImVertexSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_BEAM;

	memset(&m_EmitterStandard, 0, sizeof(SNtlBeamEmitterStandard));
}

CNtlResourceBeamSystem::~CNtlResourceBeamSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceBeamSystem::Destroy()
{
	CNtlResourceImVertexSystem::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceBeamSystem::Load(FILE* pFile)
{
	CNtlResourceImVertexSystem::Load(pFile);

	fread(&m_EmitterStandard,				sizeof(SNtlBeamEmitterStandard),		1,			pFile);

	return TRUE;
}