//***********************************************************************************
//	File		:	NtlResourceLightningSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlResourcePostEffectSystem.h"

CNtlResourcePostEffectSystem::CNtlResourcePostEffectSystem(const RwChar* strName) : CNtlResourceComponentSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_POST_EFFECT;	    

    ZeroMemory(&m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
    ZeroMemory(&m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
}

CNtlResourcePostEffectSystem::~CNtlResourcePostEffectSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourcePostEffectSystem::Destroy()
{
    // Multi Color Delete
    if(IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
    {
        if(m_EmitterPrtMultiCol.numCol > 0)
        {
            NTL_ARRAY_DELETE(m_EmitterPrtMultiCol.list);
        }
    }

	CNtlResourceComponentSystem::Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourcePostEffectSystem::Load(FILE* pFile)
{
	CNtlResourceComponentSystem::Load(pFile);
	CNtlResourceComponentSystem::Load(pFile, m_strTextureName);
	
	fread(&m_EmitterStandard,				sizeof(SNtlPostEffectEmitterStandard),		1,			pFile);        

    // Color
    if(IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
    {
        fread(&m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor), 1, pFile);        
    }

    // Multi Color
    if(IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
    {
        fread(&m_EmitterPrtMultiCol.offsetTime, sizeof(RwUInt32), 1, pFile);
        fread(&m_EmitterPrtMultiCol.offsetStartCol, sizeof(RwUInt32), 1, pFile);
        fread(&m_EmitterPrtMultiCol.offsetDeltaCol, sizeof(RwUInt32), 1, pFile);
        fread(&m_EmitterPrtMultiCol.numCol, sizeof(RwUInt32), 1, pFile);
        if(m_EmitterPrtMultiCol.numCol > 0)
        {
            m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[m_EmitterPrtMultiCol.numCol];
            fread(m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem), m_EmitterPrtMultiCol.numCol, pFile);
        }        
    }

	return TRUE;
}