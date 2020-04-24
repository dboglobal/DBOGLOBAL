//***********************************************************************************
//	File		:	NtlResourceImVertexSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlresourceImVertexsystem.h"
#include "NtlDebug.h"

CNtlResourceImVertexSystem::CNtlResourceImVertexSystem(const RwChar* strName) : CNtlResourceComponentSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_NONE;

	memset(&m_EmitterPrtSize, 0, sizeof(SNtlPrtStdEmitterPrtSize));
	memset(&m_EmitterPrtCol, 0, sizeof(RpPrtStdEmitterPrtColor));
	m_EmitterPrtCol.prtStartCol.red = m_EmitterPrtCol.prtStartCol.green = m_EmitterPrtCol.prtStartCol.blue = m_EmitterPrtCol.prtStartCol.alpha = 255;
	m_EmitterPrtCol.prtEndCol = m_EmitterPrtCol.prtStartCol;

	memset(&m_EmitterPrtMultiCol, 0, sizeof(RpPrtAdvPrtMultiColorEmitter));
	memset(&m_EmitterPrtMultiSize, 0, sizeof(SNtlAdvMultiSizeEmitter));
}

CNtlResourceImVertexSystem::~CNtlResourceImVertexSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceImVertexSystem::Destroy()
{
	NTL_ARRAY_DELETE(m_EmitterPrtMultiCol.list);
	NTL_ARRAY_DELETE(m_EmitterPrtMultiSize.list);
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceImVertexSystem::Load(FILE* pFile)
{
	CNtlResourceComponentSystem::Load(pFile);

	CNtlResourceComponentSystem::Load(pFile, m_strTextureName);


	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		fread(&m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		fread(&m_EmitterPrtSize, sizeof(SNtlPrtStdEmitterPrtSize), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		fread(&m_EmitterPrtMultiCol.numCol,	sizeof(RwUInt32),						1,			pFile);
		if (m_EmitterPrtMultiCol.numCol > 0)
		{
			m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[m_EmitterPrtMultiCol.numCol];
			NTL_ASSERTE(m_EmitterPrtMultiCol.list);
			fread(m_EmitterPrtMultiCol.list,	sizeof(RpPrtAdvEmtPrtColorItem),	m_EmitterPrtMultiCol.numCol,			pFile);
		}
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		fread(&m_EmitterPrtMultiSize.numSize,		sizeof(RwUInt32),				1,				pFile);
		if (m_EmitterPrtMultiSize.numSize > 0)
		{
			m_EmitterPrtMultiSize.list = NTL_NEW SNtlAdvEmtPrtSizeItem[m_EmitterPrtMultiSize.numSize];
			NTL_ASSERTE(m_EmitterPrtMultiSize.list);
			fread(m_EmitterPrtMultiSize.list,	sizeof(SNtlAdvEmtPrtSizeItem),	m_EmitterPrtMultiSize.numSize,			pFile);
		}
	}

    // Finish Action
    if(IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        fread(&m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish), 1, pFile);
    }

	return TRUE;
}
