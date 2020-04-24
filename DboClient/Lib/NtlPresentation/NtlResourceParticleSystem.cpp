//***********************************************************************************
//	File		:	NtlResourceParticleSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlresourceparticlesystem.h"

#include "NtlResourceEffect.h"

#include "NtlEffectDefine.h"

#include "NtlDebug.h"

CNtlResourceParticleSystem::CNtlResourceParticleSystem(const RwChar* strName) : CNtlResourceComponentSystem(strName)
{
	m_nSystemType = RESOURCE_SYSTEMTYPE_PARTICLE;

	m_nPointEmitterDataFlag	= 0;

	m_nParticleBatchMaxCount = 72;

	memset(&m_EmitterStandard, 0, sizeof(RpPrtStdEmitterStandard));

	memset(&m_PrtEmitterEmitter, 0, sizeof(RpPrtAdvPrtEmitterEmitter));

	memset(&m_EmitterPrtCol, 0, sizeof(RpPrtStdEmitterPrtColor));
	memset(&m_EmitterPrtTexCoords, 0, sizeof(RpPrtStdEmitterPrtTexCoords));
	memset(&m_EmitterPrtRotate, 0, sizeof(RpPrtStdEmitterPrt2DRotate));
	memset(&m_EmitterPrtSize, 0, sizeof(RpPrtStdEmitterPrtSize));
	memset(&m_EmitterPrtMatrix, 0, sizeof(RpPrtStdEmitterPrtMatrix));

	memset(&m_EmitterPrtOrbitPoint, 0, sizeof(SNtlPrtStdEmitterPrtOrbitPoint));
	memset(&m_EmitterPrtHurricane, 0, sizeof(SNtlPrtStdEmitterPrtHurricane));

	memset(&m_EmitterPrtMultiCol, 0, sizeof(RpPrtAdvPrtMultiColorEmitter));
	memset(&m_EmitterPrtMultiSize, 0, sizeof(RpPrtAdvPrtMultiSizeEmitter));
	memset(&m_EmitterPrtMultiTexCoords, 0, sizeof(RpPrtAdvEmtPrtMultiTexCoords));

	memset(&m_EmitterPtList, 0, sizeof(RpPrtAdvPointListEmitter));
	memset(&m_EmitterPtrSphere, 0, sizeof(RpPrtAdvSphereEmitter));
	memset(&m_EmitterPtrCircle, 0, sizeof(RpPrtAdvCircleEmitter));

	m_fDepthBias = 0.0f;
}

CNtlResourceParticleSystem::~CNtlResourceParticleSystem(void)
{
	Destroy();
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlResourceParticleSystem::Destroy()
{
	if (m_EmitterPtList.pointList != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterPtList.pointList);
		m_EmitterPtList.pointList = NULL;
	}
	if (m_EmitterPtList.directionList != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterPtList.directionList);
		m_EmitterPtList.directionList = NULL;
	}

	if (m_EmitterPrtMultiCol.list != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterPrtMultiCol.list);
		m_EmitterPrtMultiCol.list = NULL;
	}
	if (m_EmitterPrtMultiSize.list != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterPrtMultiSize.list);
		m_EmitterPrtMultiSize.list = NULL;
	}
	if (m_EmitterPrtMultiTexCoords.list != NULL)
	{
		NTL_ARRAY_DELETE(m_EmitterPrtMultiTexCoords.list);
		m_EmitterPrtMultiTexCoords.list = NULL;
	}
}

//------------------------------------------------------------------
//	FuncName	: GetEmitterDataFlag
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwInt32 CNtlResourceParticleSystem::GetEmitterDataFlag()
{
/*
#define rpPRTSTDEMITTERDATAFLAGSTANDARD                     0x00000001
#define rpPRTSTDEMITTERDATAFLAGPRTCOLOR                     0x00000002
#define rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS                 0x00000004
#define rpPRTSTDEMITTERDATAFLAGPRT2DROTATE                  0x00000008
#define rpPRTSTDEMITTERDATAFLAGPRTSIZE                      0x00000010
#define rpPRTSTDEMITTERDATAFLAGPTANK                        0x00000020
#define rpPRTSTDEMITTERDATAFLAGPRTMATRIX                    0x00000040

#define rpPRTADVEMITTERDATAFLAGPRTEMITTER                 0x00000100
#define rpPRTADVEMITTERDATAFLAGMULTICOLOR                 0x00000200
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS             0x00000400
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDSSTEP         0x00000800
#define rpPRTADVEMITTERDATAFLAGMULTISIZE                  0x00001000

#define rpPRTADVEMITTERDATAFLAGPOINTLIST                 0x00010000
#define rpPRTADVEMITTERDATAFLAGCIRCLE                    0x00020000
#define rpPRTADVEMITTERDATAFLAGSPHERE                    0x00030000

*/
	RwInt32 nEmitterDataFlag = rpPRTSTDEMITTERDATAFLAGSTANDARD | rpPRTSTDEMITTERDATAFLAGPTANK;
	nEmitterDataFlag |= (m_nEmitterDataFlag | m_nPointEmitterDataFlag);

	if (m_strSubSystem.empty())
	{
		nEmitterDataFlag &= ~rpPRTADVEMITTERDATAFLAGPRTEMITTER;
	}

	return nEmitterDataFlag;
}

//------------------------------------------------------------------
//	FuncName	: GetParticleDataFlag
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwInt32 CNtlResourceParticleSystem::GetParticleDataFlag()
{
	RwInt32 nParticleDataFlag = rpPRTSTDPARTICLEDATAFLAGSTANDARD | rpPRTSTDPARTICLEDATAFLAGVELOCITY;

	if (m_nEmitterDataFlag & rpPRTSTDEMITTERDATAFLAGPRTCOLOR)				nParticleDataFlag |= rpPRTSTDPARTICLEDATAFLAGCOLOR;
	if (m_nEmitterDataFlag & rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS)			nParticleDataFlag |= rpPRTSTDPARTICLEDATAFLAGTEXCOORDS;
	if (m_nEmitterDataFlag & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE)			nParticleDataFlag |= rpPRTSTDPARTICLEDATAFLAG2DROTATE;
	if (m_nEmitterDataFlag & rpPRTSTDEMITTERDATAFLAGPRTSIZE)				nParticleDataFlag |= rpPRTSTDPARTICLEDATAFLAGSIZE;
	if (m_nEmitterDataFlag & rpPRTSTDEMITTERDATAFLAGPRTMATRIX)				nParticleDataFlag |= rpPRTSTDPARTICLEDATAFLAGMATRIX;

	if (m_nEmitterDataFlag & rpPRTADVEMITTERDATAFLAGPRTEMITTER && !m_strSubSystem.empty())	nParticleDataFlag |= rpPRTADVPARTICLEDATAFLAGPRTEMITTER;
	if (m_nEmitterDataFlag & rpPRTADVEMITTERDATAFLAGMULTICOLOR)				nParticleDataFlag |= rpPRTADVPARTICLEDATAFLAGMULTICOLOR;
	if (m_nEmitterDataFlag & rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS)			nParticleDataFlag |= rpPRTADVPARTICLEDATAFLAGMULTITEXCOORDS;
	if (m_nEmitterDataFlag & rpPRTADVEMITTERDATAFLAGMULTITEXCOORDSSTEP)		nParticleDataFlag |= rpPRTADVPARTICLEDATAFLAGMULTITEXCOORDSSTEP;
	if (m_nEmitterDataFlag & rpPRTADVEMITTERDATAFLAGMULTISIZE)				nParticleDataFlag |= rpPRTADVPARTICLEDATAFLAGMULTISIZE;

	return nParticleDataFlag;
}

//------------------------------------------------------------------
//	FuncName	: CreateParticleClass
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceParticleSystem::CreateParticleClass(RpPrtStdEmitterClass*& pEmitterClass, RpPrtStdParticleClass*& pParticleClass)
{
	RpPrtAdvParamBlock ParamBlock;
	if (IsPointEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPOINTLIST))
	{
		ParamBlock.pointList.haveDirection = m_EmitterPtList.useDirection;
		ParamBlock.pointList.numPoint = m_EmitterPtList.numPoint;
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		ParamBlock.colorFrames.numFrames = m_EmitterPrtMultiCol.numCol;
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		ParamBlock.sizeFrames.numFrames = m_EmitterPrtMultiSize.numSize;
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
	{
		ParamBlock.texFrames.numFrames = m_EmitterPrtMultiTexCoords.numTex;
	}

	pEmitterClass = RpPrtAdvEClassStdCreate(GetEmitterDataFlag(), &ParamBlock);
	if(NULL == pEmitterClass)
	{
		return FALSE;
	}

	pParticleClass = RpPrtAdvPClassStdCreate(GetParticleDataFlag(), &ParamBlock);
	if(NULL == pParticleClass)
	{
		return FALSE;
	}
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: ResourceDataCopy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceParticleSystem::ResourceDataCopy(RpPrtStdEmitterClass* pEmitterClass, RpPrtStdEmitter* pEmitter)
{
/*
#define rpPRTSTDPROPERTYCODEEMITTER                                 0
#define rpPRTSTDPROPERTYCODEEMITTERSTANDARD                         1
#define rpPRTSTDPROPERTYCODEEMITTERPRTCOLOR                         2
#define rpPRTSTDPROPERTYCODEEMITTERPRTTEXCOORDS                     3
#define rpPRTSTDPROPERTYCODEEMITTERPRT2DROTATE                      4
#define rpPRTSTDPROPERTYCODEEMITTERPRTSIZE                          5
#define rpPRTSTDPROPERTYCODEEMITTERPTANK                            6
#define rpPRTSTDPROPERTYCODEEMITTERPRTVELOCITY                      7
#define rpPRTSTDPROPERTYCODEEMITTERPRTMATRIX                        8
#define rpPRTSTDEMITTERPROPERTYCOUNT                                9

#define RPPRTADVPROPERTYCODE(_code)                   ((0x01 << 24) | (_code))
#define rpPRTADVPROPERTYCODEEMITTERPRTCHAIN              RPPRTADVPROPERTYCODE(0)
#define rpPRTADVPROPERTYCODEEMITTERPRTEMITTER            RPPRTADVPROPERTYCODE(1)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTICOLOR         RPPRTADVPROPERTYCODE(2)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTITEXCOORDS     RPPRTADVPROPERTYCODE(3)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTITEXCOORDSSTEP RPPRTADVPROPERTYCODE(4)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTISIZE          RPPRTADVPROPERTYCODE(5)
#define rpPRTADVPROPERTYCODEEMITTERPOINTLIST             RPPRTADVPROPERTYCODE(6)
#define rpPRTADVPROPERTYCODEEMITTERCIRCLE                RPPRTADVPROPERTYCODE(7)
#define rpPRTADVPROPERTYCODEEMITTERSPHERE                RPPRTADVPROPERTYCODE(8)
*/

	RwInt32 nOffset;
	// Color
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTCOLOR);
		RpPrtStdEmitterPrtColor* emtPrtCol = (RpPrtStdEmitterPrtColor *) (((RwChar *)pEmitter) + nOffset);
		memcpy(emtPrtCol, &m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
	}
	// TexCoords
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTTEXCOORDS);
		RpPrtStdEmitterPrtTexCoords* emtPrtTexCoords = (RpPrtStdEmitterPrtTexCoords *) (((RwChar *)pEmitter) + nOffset);
		memcpy(emtPrtTexCoords, &m_EmitterPrtTexCoords, sizeof(RpPrtStdEmitterPrtTexCoords));
	}
	// 2DRotate
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRT2DROTATE);
		RpPrtStdEmitterPrt2DRotate* emtPrt2DRotate = (RpPrtStdEmitterPrt2DRotate *) (((RwChar *)pEmitter) + nOffset);
		memcpy(emtPrt2DRotate, &m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate));
	}
	// Size
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTSIZE);
	    RpPrtStdEmitterPrtSize* emtPtrSize = (RpPrtStdEmitterPrtSize *) (((RwChar *)pEmitter) + nOffset);
		memcpy(emtPtrSize, &m_EmitterPrtSize, sizeof(RpPrtStdEmitterPrtSize));
	}
	// Matrix
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTMATRIX);
	    RpPrtStdEmitterPrtMatrix* emtPtrMatrix = (RpPrtStdEmitterPrtMatrix *) (((RwChar *)pEmitter) + nOffset);
		memcpy(emtPtrMatrix, &m_EmitterPrtMatrix, sizeof(RpPrtStdEmitterPrtMatrix));
	}

	// Emitter Emitter
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER) && !m_strSubSystem.empty())
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPRTEMITTER);
		RpPrtAdvPrtEmitterEmitter* subEmitterControl = (RpPrtAdvPrtEmitterEmitter *) (((RwChar *)pEmitter) + nOffset);   
		memcpy(subEmitterControl, &m_PrtEmitterEmitter, sizeof(RpPrtAdvPrtEmitterEmitter));
	}
	// Multi Color
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPRTMULTICOLOR);
		RpPrtAdvPrtMultiColorEmitter* emtPrtMultiCol = (RpPrtAdvPrtMultiColorEmitter*) (((RwChar *)pEmitter) + nOffset);
		emtPrtMultiCol->numCol = m_EmitterPrtMultiCol.numCol;
		memcpy(emtPrtMultiCol->list, m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem) * emtPrtMultiCol->numCol);
	}
	// Multi TexCoords
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPRTMULTITEXCOORDS);
		RpPrtAdvEmtPrtMultiTexCoords* emtPrtMultiTexCoords = (RpPrtAdvEmtPrtMultiTexCoords*) (((RwChar *)pEmitter) + nOffset);
		emtPrtMultiTexCoords->numTex = m_EmitterPrtMultiTexCoords.numTex;
		memcpy(emtPrtMultiTexCoords->list, m_EmitterPrtMultiTexCoords.list, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * emtPrtMultiTexCoords->numTex);
	}
	// Multi Size
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPRTMULTISIZE);
		RpPrtAdvPrtMultiSizeEmitter* emtPrtMultiSize = (RpPrtAdvPrtMultiSizeEmitter*) (((RwChar *)pEmitter) + nOffset);
		emtPrtMultiSize->numSize = m_EmitterPrtMultiSize.numSize;
		memcpy(emtPrtMultiSize->list, m_EmitterPrtMultiSize.list, sizeof(RpPrtAdvEmtPrtSizeItem) * emtPrtMultiSize->numSize);
	}
	switch(m_nPointEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		{
			nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPOINTLIST);
			RpPrtAdvPointListEmitter* emtPtList = (RpPrtAdvPointListEmitter*) (((RwChar *)pEmitter) + nOffset);
			emtPtList->numPoint = m_EmitterPtList.numPoint;
			emtPtList->random = m_EmitterPtList.random;
			emtPtList->useDirection = m_EmitterPtList.useDirection;
			memcpy(emtPtList->pointList, m_EmitterPtList.pointList, sizeof(RwV3d) * emtPtList->numPoint);
			if (emtPtList->useDirection)
			{
				memcpy(emtPtList->directionList, m_EmitterPtList.directionList, sizeof(RwV3d) * emtPtList->numPoint);
			}
		}
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		{
			nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERCIRCLE);
			RpPrtAdvCircleEmitter* emtPtrCircle = (RpPrtAdvCircleEmitter *) (((RwChar *)pEmitter) + nOffset);
			memcpy(emtPtrCircle, &m_EmitterPtrCircle, sizeof(RpPrtAdvCircleEmitter));
		}
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		{
			nOffset = RpPrtStdPropTabGetPropOffset(pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERSPHERE);
			RpPrtAdvSphereEmitter* emtPtrSphere = (RpPrtAdvSphereEmitter*) (((RwChar *)pEmitter) + nOffset);
			memcpy(emtPtrSphere, &m_EmitterPtrSphere, sizeof(RpPrtAdvSphereEmitter));
		}
		break;
	}
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlResourceParticleSystem::Load(FILE* pFile)
{
	CNtlResourceComponentSystem::Load(pFile);

	fread(&m_nPointEmitterDataFlag,			sizeof(RwUInt32),						1,			pFile);

	fread(&m_nParticleBatchMaxCount,		sizeof(RwInt32),						1,			pFile);

	CNtlResourceComponentSystem::Load(pFile, m_strSubSystem);

	CNtlResourceComponentSystem::Load(pFile, m_strTextureName);

	fread(&m_EmitterStandard,				sizeof(RpPrtStdEmitterStandard),		1,			pFile);

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		fread(&m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))
	{
		fread(&m_EmitterPrtTexCoords, sizeof(RpPrtStdEmitterPrtTexCoords), 1, pFile);
	}
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		fread(&m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate), 1, pFile);
	}
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		fread(&m_EmitterPrtSize, sizeof(RpPrtStdEmitterPrtSize), 1, pFile);
	}
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
		fread(&m_EmitterPrtMatrix, sizeof(RpPrtStdEmitterPrtMatrix), 1, pFile);
	}
	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE)) 
	{
		fread(&m_EmitterPrtHurricane, sizeof(SNtlPrtStdEmitterPrtHurricane), 1, pFile);
	}

	if (IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
	{
		fread(&m_EmitterPrtOrbitPoint, sizeof(SNtlPrtStdEmitterPrtOrbitPoint), 1, pFile);
	}


	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER))
	{
		fread(&m_PrtEmitterEmitter,			sizeof(RpPrtAdvPrtEmitterEmitter),		1,			pFile);
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
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
	{
		fread(&m_EmitterPrtMultiTexCoords.numTex,	sizeof(RwUInt32),				1,			pFile);
		if (m_EmitterPrtMultiTexCoords.numTex > 0)
		{
			m_EmitterPrtMultiTexCoords.list = NTL_NEW RpPrtAdvEmtPrtTexCoordsItem[m_EmitterPrtMultiTexCoords.numTex];
			NTL_ASSERTE(m_EmitterPrtMultiTexCoords.list);
			fread(m_EmitterPrtMultiTexCoords.list,	sizeof(RpPrtAdvEmtPrtTexCoordsItem),	m_EmitterPrtMultiTexCoords.numTex,			pFile);
		}
	}
	if (IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		fread(&m_EmitterPrtMultiSize.numSize,		sizeof(RwUInt32),				1,				pFile);
		if (m_EmitterPrtMultiSize.numSize > 0)
		{
			m_EmitterPrtMultiSize.list = NTL_NEW RpPrtAdvEmtPrtSizeItem[m_EmitterPrtMultiSize.numSize];
			NTL_ASSERTE(m_EmitterPrtMultiSize.list);
			fread(m_EmitterPrtMultiSize.list,	sizeof(RpPrtAdvEmtPrtSizeItem),	m_EmitterPrtMultiSize.numSize,			pFile);
		}
	}

	switch(m_nPointEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		{
			fread(&m_EmitterPtList.useDirection,		sizeof(RwBool),			1,			pFile);
			fread(&m_EmitterPtList.numPoint,			sizeof(RwUInt32),		1,			pFile);

			if (m_EmitterPtList.numPoint > 0)
			{
				m_EmitterPtList.pointList = NTL_NEW RwV3d[m_EmitterPtList.numPoint];
				NTL_ASSERTE(m_EmitterPtList.pointList);

				m_EmitterPtList.directionList = NTL_NEW RwV3d[m_EmitterPtList.numPoint];
				NTL_ASSERTE(m_EmitterPtList.directionList);

				fread(m_EmitterPtList.pointList,		sizeof(RwV3d),			m_EmitterPtList.numPoint,			pFile);
				fread(m_EmitterPtList.directionList,	sizeof(RwV3d),			m_EmitterPtList.numPoint,			pFile);
			}
		}
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		fread(&m_EmitterPtrCircle,			sizeof(RpPrtAdvCircleEmitter),			1,			pFile);
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		fread(&m_EmitterPtrSphere,			sizeof(RpPrtAdvSphereEmitter),			1,			pFile);
		break;
	}

    // Trail Action 추가 (버젼 6)
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

		// Trail's Max Count had a default of 1000 which caused an out-of-memory bug.
		// Change the default value, but add the change code for an existing effect.
		m_EmitterTrail.nMaxEdgeCount = m_EmitterTrail.nMaxEdgeCount == 1000 ? 100 : m_EmitterTrail.nMaxEdgeCount;
    }

	// Depth Bias Addition (Version 7)
	if(CNtlResourceEffect::m_nVersion >= 0x00000007 && IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE))
	{
		fread(&m_fDepthBias, sizeof(RwReal), 1, pFile);
	}

	return TRUE;
}

// 파티클이 생성위치를 나타다닐지 Flag를 설정한다.
// 이 Flag의 영향은 InstanceParticle 클래스안에서 설정된다(Update시)
// 이 Flag는 m_nPointEmitterDataFlag 필드에 설정된다.
void CNtlResourceParticleSystem::SetFollowFlag(BOOL bFollow)
{
    if(bFollow)
    {
        m_nEmitterDataFlag |= NTLrpPRTADVEMITTERDATAFLAGFOLLOW;
        
    }
    else if(m_nEmitterDataFlag & NTLrpPRTADVEMITTERDATAFLAGFOLLOW)
    {
        m_nEmitterDataFlag ^= NTLrpPRTADVEMITTERDATAFLAGFOLLOW;
    }
}