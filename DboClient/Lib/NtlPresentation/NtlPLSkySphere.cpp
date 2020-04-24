#include "precomp_ntlpresentation.h"
#include "NtlPLSkySphere.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlPLResourceManager.h"
#include "ntlworlddi.h"
#include "NtlPLEvent.h"
#include "ntlworldcommon.h"
#include "NtlPLEvent.h"
#include "ceventhandler.h"
#include "ntlworldinterface.h"
#include "NtlPLWorldState.h"

CNtlPLSkySphere::CNtlPLSkySphere()
{
	m_VisibilityFlag = TRUE;
}

CNtlPLSkySphere::~CNtlPLSkySphere()
{
}

RwBool CNtlPLSkySphere::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLSkySphere::Create");

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);
	
	// Dummy sky atomic
	m_pDummySkyLayer			= NTL_NEW sSKY_LAYER;
	m_pDummySkyLayer->_pAtom	= NULL;
	m_pDummySkyLayer->_pRender	= FALSE;

	// Diffuse sky atomic
	m_pBaseSkyLayer[0]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[0]->_pAtom		= NULL;
	m_pBaseSkyLayer[0]->_pRender	= FALSE;

	// Base sky atomic
	m_pBaseSkyLayer[1]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[1]->_pAtom		= API_PL_LoadAtomic("world\\mesh\\Sphere_sky_base.dff", "texture\\ntlwe\\Sky\\");
	m_pBaseSkyLayer[1]->_pRender	= FALSE;
	DBO_ASSERT(m_pBaseSkyLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBaseSkyLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);

	// blending temporary sky atomics
	m_pBlendedTmpLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[0]->_pAtom	= NULL;
	m_pBlendedTmpLayer[0]->_pRender = FALSE;

	m_pBlendedTmpLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[1]->_pAtom	= NULL;
	m_pBlendedTmpLayer[1]->_pRender = FALSE;

	// blending sky atomics
	m_pBlendedSkyLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[0]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\Sphere_sky_blended0.dff", "texture\\ntlwe\\Sky\\");
	m_pBlendedSkyLayer[0]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedSkyLayer[0]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedSkyLayer[0]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);

	m_pBlendedSkyLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[1]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\Sphere_sky_blended1.dff", "texture\\ntlwe\\Sky\\");
	m_pBlendedSkyLayer[1]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedSkyLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedSkyLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, 255);

	NTL_RETURN(TRUE);
}


void CNtlPLSkySphere::Destroy() 
{
	NTL_FUNCTION("CNtlPLSkySphere::Destroy");

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	if(m_pDummySkyLayer)		NTL_DELETE(m_pDummySkyLayer);
	if(m_pBaseSkyLayer[0])		NTL_DELETE(m_pBaseSkyLayer[0]);
	if(m_pBaseSkyLayer[1])		NTL_DELETE(m_pBaseSkyLayer[1]);
	if(m_pBlendedSkyLayer[0])	NTL_DELETE(m_pBlendedSkyLayer[0]);
	if(m_pBlendedSkyLayer[1])	NTL_DELETE(m_pBlendedSkyLayer[1]);
	if(m_pBlendedTmpLayer[0])	NTL_DELETE(m_pBlendedTmpLayer[0]);
	if(m_pBlendedTmpLayer[1])	NTL_DELETE(m_pBlendedTmpLayer[1]);

	NTL_RETURNVOID();
}

RwBool CNtlPLSkySphere::Update(RwReal fElapsed)
{
	if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE)
		return TRUE;

	UpdateFieldVariation(fElapsed);

	m_pBaseSkyLayer[1]->Rotate(m_pBaseSkyLayer[1]->_RotSpeedMPS);
	m_pBlendedSkyLayer[0]->Rotate(m_pBlendedSkyLayer[0]->_RotSpeedMPS);
	m_pBlendedSkyLayer[1]->Rotate(m_pBlendedSkyLayer[1]->_RotSpeedMPS);
	API_PL_RotationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_pBaseSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedSkyLayer[0]->_pAtom, m_pBlendedSkyLayer[0]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedSkyLayer[1]->_pAtom, m_pBlendedSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);

	if(m_pBaseSkyLayer[1]->_pRender)
	{	
		API_PL_TranslationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	if(m_pBlendedSkyLayer[0]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedSkyLayer[0]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	if(m_pBlendedSkyLayer[1]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	return TRUE;
}

void CNtlPLSkySphere::HandleEvents(RWS::CMsg &pMsg)
{
	// 1. only one type of sphere sky must be created
	// 2. there was no need to think the variation; interpolation over
	// 3. the atomics 4 sky has a texture which's got "sky_sp" so careful with this when loading textures

	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		m_InterpolationMode				= 0x00000000;
		m_RestTime4Change[0]			= -999.0f;
		m_RestTime4Change[1]			= -999.0f;
		m_RestTime4Change[2]			= -999.0f;

		// force to change current test textures
		if(strcmp(pNtlFieldProp->_BaseSkyTexName, API_PL_AtomicGetTexture(m_pBaseSkyLayer[1]->_pAtom)->name))
		{
			if(strcmp(pNtlFieldProp->_BaseSkyTexName, "null"))
			{
				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BaseSkyTexName, "texture\\ntlwe\\sky\\");
				DBO_ASSERT(pTexture, "Texture load failed.");

				API_PL_AtomicSetTexture(m_pBaseSkyLayer[1]->_pAtom, pTexture);
				API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);
				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
				CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
				m_pBaseSkyLayer[1]->_pRender = TRUE;
			}
			else
			{
				m_pBaseSkyLayer[1]->_pRender = FALSE;
			}
		}

		if(strcmp(pNtlFieldProp->_BlendedTexName[0], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name))
		{
			if(strcmp(pNtlFieldProp->_BlendedTexName[0], "null"))
			{
				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[0], "texture\\ntlwe\\sky\\");
				DBO_ASSERT(pTexture, "Texture load failed.");

				API_PL_AtomicSetTexture(m_pBlendedSkyLayer[0]->_pAtom, pTexture);
				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
				CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
				m_pBlendedSkyLayer[0]->_pRender = TRUE;
			}
			else
			{
				m_pBlendedSkyLayer[0]->_pRender = FALSE;
			}
		}

		if(strcmp(pNtlFieldProp->_BlendedTexName[1], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name))
		{
			if(strcmp(pNtlFieldProp->_BlendedTexName[1], "null"))
			{
				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[1], "texture\\ntlwe\\sky\\");
				DBO_ASSERT(pTexture, "Texture load failed.");

				API_PL_AtomicSetTexture(m_pBlendedSkyLayer[1]->_pAtom, pTexture);
				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, 255);
				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
				CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
				m_pBlendedSkyLayer[1]->_pRender = TRUE;
			}
			else
			{
				m_pBlendedSkyLayer[1]->_pRender = FALSE;
			}
		}

		// force interpolation of speed
		m_pBaseSkyLayer[1]->_RotSpeedMPS	= pNtlFieldProp->_BaseSkySpeed;
		m_pBlendedSkyLayer[0]->_RotSpeedMPS	= pNtlFieldProp->_BlendedTexSpeed[0];
		m_pBlendedSkyLayer[1]->_RotSpeedMPS	= pNtlFieldProp->_BlendedTexSpeed[1];
	}
}

void CNtlPLSkySphere::UpdateFieldVariation(RwReal fElapsed)
{
	return;
}