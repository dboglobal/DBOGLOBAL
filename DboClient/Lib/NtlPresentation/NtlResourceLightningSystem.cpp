//***********************************************************************************
//	File		:	NtlResourceLightningSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlresourcelightningsystem.h"

#include "NtlDebug.h"

CNtlResourceLightningSystem::CNtlResourceLightningSystem(const RwChar* strName) : CNtlResourceImVertexSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_LIGHTNING;

	memset(&m_EmitterStandard, 0, sizeof(SNtlLightningEmitterStandard));       

    // 사용하지 않는 Rotate값을 EndPoint 값으로 사용한다.
    m_vRotate.x = m_vRotate.y = 0.0f;
    m_vRotate.z = 5.0f;
}

CNtlResourceLightningSystem::~CNtlResourceLightningSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceLightningSystem::Destroy()
{
	CNtlResourceImVertexSystem::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceLightningSystem::Load(FILE* pFile)
{
	CNtlResourceImVertexSystem::Load(pFile);

	fread(&m_EmitterStandard,				sizeof(SNtlLightningEmitterStandard),		1,			pFile);

	return TRUE;
}