#include "precomp_ntlpresentation.h"
#include "NtlPLSkyDragon.h"
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
#include "NtlPLSceneManager.h"


CNtlPLSkyDragon::CNtlPLSkyDragon()
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
	SetLayer(PLENTITY_LAYER_SKY);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_SKY);

	m_pDummySkyLayer		= NULL;
	m_pBaseSkyLayer[0]		= NULL;
	m_pBaseSkyLayer[1]		= NULL;
	m_pBlendedSkyLayer[0]	= NULL;
	m_pBlendedSkyLayer[1]	= NULL;
	m_pBlendedTmpLayer[0]	= NULL;
	m_pBlendedTmpLayer[1]	= NULL;

	m_RestTime4Change[0] = -999.0f;
	m_RestTime4Change[1] = -999.0f;
	m_RestTime4Change[2] = -999.0f;

	m_VisibilityFlag = FALSE;
}

CNtlPLSkyDragon::~CNtlPLSkyDragon()
{
}

RwBool CNtlPLSkyDragon::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLSkyDragon::Create");

	RwTexture* pTexture;

	// Dummy sky atomic
	m_pDummySkyLayer			= NTL_NEW sSKY_LAYER;
	m_pDummySkyLayer->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_dummy.dff", ".\\texture\\ntlwe\\sky\\");
	m_pDummySkyLayer->_pRender	= TRUE;
	DBO_ASSERT(m_pDummySkyLayer->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pDummySkyLayer->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	// Diffuse sky atomic
	m_pBaseSkyLayer[0]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[0]->_pAtom		= NULL;
	m_pBaseSkyLayer[0]->_pRender	= FALSE;

	// Base sky atomic
	m_pBaseSkyLayer[1]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[1]->_pAtom		= API_PL_LoadAtomic("world\\mesh\\sky_base.dff", ".\\texture\\ntlwe\\Sky\\");
	m_pBaseSkyLayer[1]->_pRender	= FALSE;
	DBO_ASSERT(m_pBaseSkyLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBaseSkyLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("sky01.png", ".\\texture\\ntlwe\\sky\\");
	DBO_ASSERT(pTexture, "Texture load failed.");

	API_PL_AtomicSetTexture(m_pBaseSkyLayer[1]->_pAtom, pTexture);
	API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);
	RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
	CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);

	// blending temporary sky atomics
	m_pBlendedSkyLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[0]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_base.dff", ".\\texture\\ntlwe\\Sky\\");
	m_pBlendedSkyLayer[0]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedSkyLayer[0]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedSkyLayer[0]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("cloud-t03.png", ".\\texture\\ntlwe\\sky\\");
	DBO_ASSERT(pTexture, "Texture load failed.");

	API_PL_AtomicSetTexture(m_pBlendedSkyLayer[0]->_pAtom, pTexture);
	API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
	RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
	CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
	
	m_pBlendedSkyLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[1]->_pAtom	= NULL;
	m_pBlendedSkyLayer[1]->_pRender = FALSE;

	m_pBlendedTmpLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[0]->_pAtom	= NULL;
	m_pBlendedTmpLayer[0]->_pRender = FALSE;

	m_pBlendedTmpLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[1]->_pAtom	= NULL;
	m_pBlendedTmpLayer[1]->_pRender = FALSE;

	NTL_RETURN(TRUE);
}

void CNtlPLSkyDragon::Destroy() 
{
	NTL_FUNCTION("CNtlPLSkyDragon::Destroy");

	if(m_pDummySkyLayer)		NTL_DELETE(m_pDummySkyLayer);
	if(m_pBaseSkyLayer[0])		NTL_DELETE(m_pBaseSkyLayer[0]);
	if(m_pBaseSkyLayer[1])		NTL_DELETE(m_pBaseSkyLayer[1]);
	if(m_pBlendedSkyLayer[0])	NTL_DELETE(m_pBlendedSkyLayer[0]);
	if(m_pBlendedSkyLayer[1])	NTL_DELETE(m_pBlendedSkyLayer[1]);
	if(m_pBlendedTmpLayer[0])	NTL_DELETE(m_pBlendedTmpLayer[0]);
	if(m_pBlendedTmpLayer[1])	NTL_DELETE(m_pBlendedTmpLayer[1]);

	NTL_RETURNVOID();
}

void CNtlPLSkyDragon::HandleEvents(RWS::CMsg &pMsg)
{
	return;
}

VOID CNtlPLSkyDragon::VisibilitySwitch(RwBool _Switch)
{
	m_VisibilityFlag = _Switch;
	m_pBaseSkyLayer[1]->_pRender	= _Switch;
	m_pBlendedSkyLayer[0]->_pRender	= _Switch;
}

VOID CNtlPLSkyDragon::FadeLerp(eCUR_FADE_STATUS _CFS, RpAtomic* _pAtom, RwReal _STime, RwReal _ETime)
{
	if(_CFS == eCFS_NOTHING)
	{
		API_PL_AtomicSetMaterialSetAlpha(_pAtom, 0);
	}
	else if(_CFS == eCFS_IN)
	{
		API_PL_AtomicSetMaterialSetAlpha(_pAtom, RpNtlWorldUInt8PropLerp(0, 255, _STime, dSKY_DRAGON_FADE_VARIATION_TIME));
	}
	else if(_CFS == eCFS_OUT)
	{
		API_PL_AtomicSetMaterialSetAlpha(_pAtom, RpNtlWorldUInt8PropLerp(255, 0, _STime, dSKY_DRAGON_FADE_VARIATION_TIME));
	}
}

VOID CNtlPLSkyDragon::VisibilitySwitchOfMainSky(RwBool _Switch)
{
	CNtlPLSky* pBaseSky			= static_cast<CNtlPLSky*>(GetSceneManager()->GetSky());
	pBaseSky->m_VisibilityFlag	= _Switch;
}

void CNtlPLSkyDragon::UpdateFieldVariation(RwReal fElapsed)
{
	if(m_RestTime4Change[0] >= 0.0f)
	{
		m_RestTime4Change[0] -= g_GetElapsedTime();

		FadeLerp(m_CFSIdx, m_pBaseSkyLayer[1]->_pAtom, m_RestTime4Change[0], dSKY_DRAGON_FADE_VARIATION_TIME);
		FadeLerp(m_CFSIdx, m_pBlendedSkyLayer[0]->_pAtom, m_RestTime4Change[0], dSKY_DRAGON_FADE_VARIATION_TIME);

		if(m_RestTime4Change[0] < 0)
		{
			m_RestTime4Change[0] = -999.0f;

			if(m_CFSIdx == eCFS_IN)
			{
				API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);
				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);

				VisibilitySwitch(TRUE);
				VisibilitySwitchOfMainSky(FALSE);

				m_CFSIdx = eCFS_NOTHING;
			}
			else if(m_CFSIdx == eCFS_OUT)
			{
				API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 0);
				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 0);

				VisibilitySwitch(FALSE);
				VisibilitySwitchOfMainSky(TRUE);

				m_CFSIdx = eCFS_NOTHING;
			}
		}
	}
}

RwBool CNtlPLSkyDragon::Update(RwReal fElapsed)
{
	if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE)
		return TRUE;

	// update fade variation
	UpdateFieldVariation(fElapsed);

	// update trasform variation
	API_PL_TranslationAtomic(m_pDummySkyLayer->_pAtom, m_WorldDatumPos, rwCOMBINEREPLACE);

	m_pBaseSkyLayer[1]->Rotate(dSKY_DRAGON_LAYER0_ROTATION_SPEED);
	m_pBlendedSkyLayer[0]->Rotate(dSKY_DRAGON_LAYER1_ROTATION_SPEED);
	API_PL_RotationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_pBaseSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedSkyLayer[0]->_pAtom, m_pBlendedSkyLayer[0]->_RotSpeed, rwCOMBINEREPLACE);

	if(m_pBaseSkyLayer[0]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBaseSkyLayer[0]->_pAtom, m_WorldDatumPos, rwCOMBINEREPLACE);
	}

	if(m_pBaseSkyLayer[1]->_pRender)
	{	
		API_PL_TranslationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	if(m_pBlendedSkyLayer[0]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedSkyLayer[0]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	return TRUE;
}

VOID CNtlPLSkyDragon::FadeIn()
{
	m_CFSIdx				= eCFS_IN;
	m_RestTime4Change[0]	= dSKY_DRAGON_FADE_VARIATION_TIME;

	VisibilitySwitch(TRUE);
	VisibilitySwitchOfMainSky(TRUE);
}

VOID CNtlPLSkyDragon::FadeOut()
{
	m_CFSIdx				= eCFS_OUT;
	m_RestTime4Change[0]	= dSKY_DRAGON_FADE_VARIATION_TIME;

	VisibilitySwitch(TRUE);
	VisibilitySwitchOfMainSky(TRUE);
}