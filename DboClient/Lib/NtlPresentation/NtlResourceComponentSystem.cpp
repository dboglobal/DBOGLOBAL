//***********************************************************************************
//	File		:	NtlResourceComponentSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlresourcecomponentsystem.h"

#include "NtlDebug.h"
#include "NtlResourceEffect.h"

#include "NtlEffectDefine.h"

CNtlResourceComponentSystem::CNtlResourceComponentSystem(const RwChar* strName)
{
//#ifdef RWDEBUG	// uncommented by daneos, otherwise the effects wont work
	m_bShow					= TRUE;
//#endif
	m_nSystemType			= RESOURCE_SYSTEMTYPE_NONE;
	m_strName				= strName;

	m_nEmitterDataFlag		= 0;

	m_bSubSystem			= FALSE;

	m_bEternity				= TRUE;
	m_fStartTime			= 0.f;
	m_fLifeTime				= 10.f;
    m_fBoundSphereRadius    = 5.0f;

	m_nPathEmitterDataFlag	= 0;
	ZeroMemory(&m_EmitterPrtSplinePath, sizeof(SNtlPrtEmitterSplinePath));
	ZeroMemory(&m_EmitterPrtFadeOut, sizeof(SNtlPrtStdEmitterPrtFadeOut));
	ZeroMemory(&m_EmitterMultiTextures, sizeof(RpPrtAdvEmtPrtMultiTextures));        

	m_bAlphaBlend	= TRUE;
	m_nSrcBlend		= rwBLENDSRCALPHA;
	m_nDestBlend	= rwBLENDONE;
	
    m_vPosition = ZeroAxis;
    m_vRotate = ZeroAxis;
    

	RwMatrixSetIdentity(&m_matPathWorld);

	RebuildMatrix();
}

CNtlResourceComponentSystem::~CNtlResourceComponentSystem(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceComponentSystem::Destroy()
{
	if (m_EmitterMultiTextures.list != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterMultiTextures.list);
		m_EmitterMultiTextures.list = NULL;
	}
}

//------------------------------------------------------------------
//	FuncName	: RebuildMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceComponentSystem::RebuildMatrix()
{
	RwMatrixRotate(&m_matWorld, &VEC_X, m_vRotate.x, rwCOMBINEREPLACE);
	RwMatrixRotate(&m_matWorld, &VEC_Y, m_vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&m_matWorld, &VEC_Z, m_vRotate.z, rwCOMBINEPOSTCONCAT);

	RwMatrixTranslate(&m_matWorld, &m_vPosition, rwCOMBINEPOSTCONCAT);
}

//------------------------------------------------------------------
//	FuncName	: RebuildMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceComponentSystem::RebuildPathMatrix()
{
	RwMatrixRotate(&m_matPathWorld, &VEC_X, m_EmitterPrtSplinePath.vRotate.x, rwCOMBINEREPLACE);
	RwMatrixRotate(&m_matPathWorld, &VEC_Y, m_EmitterPrtSplinePath.vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&m_matPathWorld, &VEC_Z, m_EmitterPrtSplinePath.vRotate.z, rwCOMBINEPOSTCONCAT);

	RwMatrixTranslate(&m_matPathWorld, &m_EmitterPrtSplinePath.vPosition, rwCOMBINEPOSTCONCAT);
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceComponentSystem::Load(FILE* pFile)
{
	fread(&m_nEmitterDataFlag,			sizeof(RwUInt32),						1,			pFile);

	fread(&m_bSubSystem,				sizeof(RwBool),							1,			pFile);

	fread(&m_bEternity,					sizeof(RwBool),							1,			pFile);
	fread(&m_fStartTime,				sizeof(RwReal),							1,			pFile);
	fread(&m_fLifeTime,					sizeof(RwReal),							1,			pFile);
	
	fread(&m_nPathEmitterDataFlag,		sizeof(RwUInt32),						1,			pFile);

	if (m_nPathEmitterDataFlag == rpPRTADVEMITTERDATAFLAGSPLINEPATH)
	{
		fread(&m_EmitterPrtSplinePath,		sizeof(SNtlPrtEmitterSplinePath),		1,			pFile);
	//	m_EmitterPrtSplinePath.rwUnknown = 1.0f;
	//	DBO_WARNING_MESSAGE("m_EmitterPrtSplinePath.rwUnknown: " << m_EmitterPrtSplinePath.rwUnknown);
	}
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
	{
		fread(&m_EmitterPrtFadeOut,			sizeof(SNtlPrtStdEmitterPrtFadeOut),		1,			pFile);
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		fread(&m_EmitterMultiTextures.time,					sizeof(RwReal),		1,			pFile);

		fread(&m_EmitterMultiTextures.bLoop,				sizeof(RwBool),		1,			pFile);

		fread(&m_EmitterMultiTextures.numTextures,			sizeof(RwUInt32),	1,			pFile);

		if (m_EmitterMultiTextures.numTextures > 0)
		{
			m_EmitterMultiTextures.list = NTL_NEW RpPrtAdvEmtPrtTextureItem[m_EmitterMultiTextures.numTextures];
			for (RwUInt32 i = 0; i < m_EmitterMultiTextures.numTextures; ++ i)
			{
				fread(&m_EmitterMultiTextures.list[i].time,		sizeof(RwReal),						1,			pFile);
				fread(&m_EmitterMultiTextures.list[i].timeBias,	sizeof(RwReal),						1,			pFile);

				CNtlResourceComponentSystem::Load(pFile, m_EmitterMultiTextures.list[i].strTexture);
			}
		}
	}

	fread(&m_bAlphaBlend,					sizeof(RwBool),							1,			pFile); // 1
	if (m_bAlphaBlend == 0)
	{
		m_bAlphaBlend = 1;
	//	DBO_WARNING_MESSAGE("yes");
	}

	fread(&m_nSrcBlend,						sizeof(RwBlendFunction),				1,			pFile); // 5

	fread(&m_nDestBlend,					sizeof(RwBlendFunction),				1,			pFile); // 2

	fread(&m_vPosition,					sizeof(RwV3d),							1,			pFile);
	fread(&m_vRotate,					sizeof(RwV3d),							1,			pFile);
    fread(&m_sGroundFlag,				sizeof(SGroundFlag), 1, pFile);


    if(CNtlResourceEffect::m_nVersion >= 0x00000012)
    {
        fread(&m_fBoundSphereRadius, sizeof(RwReal), 1, pFile);
    }
	
	//by daneos
	if (CNtlResourceEffect::m_nVersion >= 0x00000065)
	{
		int nUnknown;
		fread(&nUnknown, sizeof(int), 1, pFile);
	}

	RebuildMatrix();
	RebuildPathMatrix();
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceComponentSystem::Load(FILE* pFile, std::string& string)
{
	RwInt32 length;
	RwChar strName[NTL_MAX_DIR_PATH];

	fread(&length,							sizeof(RwInt32),						1,			pFile);
	if (length > 0)
	{
		fread(strName,						sizeof(RwChar),							length,		pFile);
		string = strName;
	}
	return TRUE;
}

void CNtlResourceComponentSystem::SetBoundingSphereRadius( RwReal fRadius ) 
{
    m_fBoundSphereRadius = (m_fBoundSphereRadius + fRadius) * 0.5f;
}