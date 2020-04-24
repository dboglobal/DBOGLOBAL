//***********************************************************************************
//	File		:	NtlResourceMeshSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlresourcemeshsystem.h"

#include "NtlDebug.h"

#include "NtlResourceEffect.h"

CNtlResourceMeshSystem::CNtlResourceMeshSystem(const RwChar* strName) : CNtlResourceComponentSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_MESH;

	memset(&m_EmitterPrtSize, 0, sizeof(SNtlPrtStdEmitterPrtSize));
	memset(&m_EmitterPrtCol, 0, sizeof(RpPrtStdEmitterPrtColor));
	memset(&m_EmitterPrtRotate, 0, sizeof(SNtlPrtStdEmitterPrtRotate));

	memset(&m_EmitterPrtMultiCol, 0, sizeof(RpPrtAdvPrtMultiColorEmitter));
	memset(&m_EmitterPrtMultiSize, 0, sizeof(SNtlAdvMultiSizeEmitter));
	memset(&m_EmitterPrtMultiRotate, 0, sizeof(SNtlAdvMultiRotateEmitter));

	m_EmitterPrtSize.prtStartSize.x = 1.f;
	m_EmitterPrtSize.prtStartSize.y = 1.f;
	m_EmitterPrtSize.prtStartSize.z = 1.f;

	m_EmitterPrtSize.prtEndSize.x = 5.f;
	m_EmitterPrtSize.prtEndSize.y = 5.f;
	m_EmitterPrtSize.prtEndSize.z = 5.f;
}

CNtlResourceMeshSystem::~CNtlResourceMeshSystem(void)
{
	Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceMeshSystem::Destroy()
{
	NTL_ARRAY_DELETE(m_EmitterPrtMultiCol.list);
	NTL_ARRAY_DELETE(m_EmitterPrtMultiSize.list);
	NTL_ARRAY_DELETE(m_EmitterPrtMultiRotate.list);
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceMeshSystem::Load(FILE* pFile)
{
	CNtlResourceComponentSystem::Load(pFile);


	CNtlResourceComponentSystem::Load(pFile, m_strMeshFileName);

	CNtlResourceComponentSystem::Load(pFile, m_strAniFileName);

	CNtlResourceComponentSystem::Load(pFile, m_strUvFileName);


	fread(&m_EmitterStandard,				sizeof(SNtlMeshEmitterStandard),		1,			pFile);	

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		fread(&m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		fread(&m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) /*&& CNtlResourceEffect::m_nVersion >= 0x00000018*/)
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
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
	{
		fread(&m_EmitterPrtMultiRotate.numRotate,		sizeof(RwUInt32),				1,				pFile);
		if (m_EmitterPrtMultiRotate.numRotate > 0)
		{
			m_EmitterPrtMultiRotate.list = NTL_NEW SNtlAdvEmtPrtRotateItem[m_EmitterPrtMultiRotate.numRotate];
			NTL_ASSERTE(m_EmitterPrtMultiRotate.list);
			fread(m_EmitterPrtMultiRotate.list,	sizeof(SNtlAdvEmtPrtRotateItem),	m_EmitterPrtMultiRotate.numRotate,			pFile);
		}
	}



    // Finish Action Add (Version 5)
    if(CNtlResourceEffect::m_nVersion > 0x00000004 && IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        fread(&m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish), 1, pFile);
    }    

    // Trail Action Add (Version 6)
    if(CNtlResourceEffect::m_nVersion >= 0x00000006 && IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
		if (CNtlResourceEffect::m_nVersion >= 0x00000067)
		{
			fread(&m_EmitterTrail, sizeof(SNtlPrtStdEmitterTrail), 1, pFile);

			char temp[308];
			fread(&temp, sizeof(temp), 1, pFile);
		}
		else if (CNtlResourceEffect::m_nVersion >= 0x00000066)
		{
			fread(&m_EmitterTrail, sizeof(SNtlPrtStdEmitterTrail), 1, pFile);

			char temp[4];
			fread(&temp, sizeof(temp), 1, pFile);
		}
		else
		{
			fread(&m_EmitterTrail, sizeof(SNtlPrtStdEmitterTrail), 1, pFile);
		}
        
        // Trail의 Max Count가 디폴트가 1000이라서 메모리 부족 버그가 일어났었다. 
        // 디폴트값을 변경하지만 기존의 이펙트를 위해서 변경 코드를 추가한다.
        m_EmitterTrail.nMaxEdgeCount = m_EmitterTrail.nMaxEdgeCount == 1000 ? 100 : m_EmitterTrail.nMaxEdgeCount;
    }


	return TRUE;
}