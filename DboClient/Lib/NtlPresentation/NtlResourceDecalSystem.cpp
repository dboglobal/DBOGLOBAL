#include "precomp_ntlpresentation.h"
#include "NtlResourceDecalSystem.h"
#include "NtlResourceEffect.h"

CNtlResourceDecalSystem::CNtlResourceDecalSystem(const RwChar* strName) : CNtlResourceImVertexSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_DECAL;

	ZeroMemory(&m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate));
}

CNtlResourceDecalSystem::~CNtlResourceDecalSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceDecalSystem::Destroy()
{
	CNtlResourceImVertexSystem::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceDecalSystem::Load(FILE* pFile)
{
	CNtlResourceImVertexSystem::Load(pFile);

    // YOffset 추가 (버전 8)
    if(CNtlResourceEffect::m_nVersion > 0x00000007)
    {
        fread(&m_EmitterStandard, sizeof(SNtlDecalEmitterStandard), 1, pFile);
    }
    else
    {
        fread(&m_EmitterStandard, sizeof(SNtlDecalEmitterStandard_Old), 1, pFile);
        m_EmitterStandard.fYOffset = 0.02f;
    }

	fread(&m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate), 1, pFile);
	fread(&m_EmitterUVAnim, sizeof(SNtlPrtStdEmitterPrtUVAnim), 1, pFile);

	return TRUE;
}