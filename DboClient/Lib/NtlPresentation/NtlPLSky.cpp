#include "precomp_ntlpresentation.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlPLSky.h"
#include "NtlPLResourceManager.h"
#include "ntlworlddi.h"
#include "NtlPLEvent.h"
#include "ntlworldcommon.h"
#include "NtlPLEvent.h"
#include "ceventhandler.h"
#include "ntlworldinterface.h"
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlProfiler.h"
#include "NtlPLOptionManager.h"
#include "NtlPLWorldState.h"

CNtlPLSky::CNtlPLSky()
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
	m_VisibilityFlag		= TRUE;

	m_RestTime4Change[0]	= -999.0f;
	m_RestTime4Change[1]	= -999.0f;
	m_RestTime4Change[2]	= -999.0f;

	m_clrLightning.alpha	= 0;
	m_clrLightning.red		= 255;
	m_clrLightning.green	= 255;
	m_clrLightning.blue		= 255;

	m_fTimeFadeIn			= 0.0f;
	m_fTimeFadeOut			= 0.0f;
	m_fTimeLightning		= 0.0f;

	m_bPassedSky			= FALSE;
}

CNtlPLSky::~CNtlPLSky()
{
}

RwBool CNtlPLSky::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLSky::Create");

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY, 0);
	// Dummy sky atomic
	m_pDummySkyLayer			= NTL_NEW sSKY_LAYER;
	m_pDummySkyLayer->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_dummy.dff", ".\\texture\\ntlwe\\sky\\");
	m_pDummySkyLayer->_pRender	= TRUE;
	DBO_ASSERT(m_pDummySkyLayer->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pDummySkyLayer->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	// Diffuse sky atomic
	m_pBaseSkyLayer[0]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[0]->_pAtom		= API_PL_LoadAtomic("world\\mesh\\sky_diffuse.dff", ".\\texture\\ntlwe\\Sky\\");
	m_pBaseSkyLayer[0]->_pRender	= TRUE;
	DBO_ASSERT(m_pBaseSkyLayer[0]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBaseSkyLayer[0]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR);

	RwRGBA	RGBATmp[3];
	RGBATmp[0].red		= 255;
	RGBATmp[0].green	= 255;
	RGBATmp[0].blue		= 255;

	RGBATmp[1].red		= 255;
	RGBATmp[1].green	= 255;
	RGBATmp[1].blue		= 255;

	RGBATmp[2].red		= 255;
	RGBATmp[2].green	= 255;
	RGBATmp[2].blue		= 255;
	SetColorOfDiffuseSkyAtomic(RGBATmp);

	// Base sky atomic
	m_pBaseSkyLayer[1]				= NTL_NEW sSKY_LAYER;
	m_pBaseSkyLayer[1]->_pAtom		= API_PL_LoadAtomic("world\\mesh\\sky_base.dff", ".\\texture\\ntlwe\\sky\\");
	m_pBaseSkyLayer[1]->_pRender	= FALSE;
	DBO_ASSERT(m_pBaseSkyLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBaseSkyLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	strcpy_s(API_PL_AtomicGetTexture(m_pBaseSkyLayer[1]->_pAtom)->name, rwTEXTUREBASENAMELENGTH, "null");

	// blending temporary sky atomics
	m_pBlendedTmpLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[0]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_blended0.dff", ".\\texture\\ntlwe\\sky\\");
	m_pBlendedTmpLayer[0]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedTmpLayer[0]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedTmpLayer[0]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	strcpy_s(API_PL_AtomicGetTexture(m_pBlendedTmpLayer[0]->_pAtom)->name, rwTEXTUREBASENAMELENGTH, "null");

	m_pBlendedTmpLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedTmpLayer[1]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_blended1.dff", ".\\texture\\ntlwe\\sky\\");
	m_pBlendedTmpLayer[1]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedTmpLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedTmpLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	strcpy_s(API_PL_AtomicGetTexture(m_pBlendedTmpLayer[1]->_pAtom)->name, rwTEXTUREBASENAMELENGTH, "null");

	// blending sky atomics
	m_pBlendedSkyLayer[0]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[0]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_blended0T.dff", ".\\texture\\ntlwe\\sky\\");
	m_pBlendedSkyLayer[0]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedSkyLayer[0]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedSkyLayer[0]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	strcpy_s(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name, rwTEXTUREBASENAMELENGTH, "null");

	m_pBlendedSkyLayer[1]			= NTL_NEW sSKY_LAYER;
	m_pBlendedSkyLayer[1]->_pAtom	= API_PL_LoadAtomic("world\\mesh\\sky_blended1T.dff", ".\\texture\\ntlwe\\sky\\");
	m_pBlendedSkyLayer[1]->_pRender = FALSE;
	DBO_ASSERT(m_pBlendedSkyLayer[1]->_pAtom, "Resource load failed.");

	API_PL_AtomicSetGeoFlags(m_pBlendedSkyLayer[1]->_pAtom, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);
	strcpy_s(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name, rwTEXTUREBASENAMELENGTH, "null");

	NTL_RETURN(TRUE);
}


void CNtlPLSky::Destroy() 
{
	NTL_FUNCTION("CNtlPLSky::Destroy");
	
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY);

	if(m_pDummySkyLayer)		NTL_DELETE(m_pDummySkyLayer);
	if(m_pBaseSkyLayer[0])		NTL_DELETE(m_pBaseSkyLayer[0]);
	if(m_pBaseSkyLayer[1])		NTL_DELETE(m_pBaseSkyLayer[1]);
	if(m_pBlendedSkyLayer[0])	NTL_DELETE(m_pBlendedSkyLayer[0]);
	if(m_pBlendedSkyLayer[1])	NTL_DELETE(m_pBlendedSkyLayer[1]);
	if(m_pBlendedTmpLayer[0])	NTL_DELETE(m_pBlendedTmpLayer[0]);
	if(m_pBlendedTmpLayer[1])	NTL_DELETE(m_pBlendedTmpLayer[1]);

	NTL_RETURNVOID();
}

eBASE_SKY_MODE CNtlPLSky::GetBaseSkyMode()
{
	if(m_pBaseSkyLayer[0]->_pRender)
	{
		return eBSM_RGB_MODE;
	}
	else if(m_pBaseSkyLayer[1]->_pRender)
	{
		return eBSM_TEX_MODE;
	}

	return eBSM_NOTHING;
}

void CNtlPLSky::SetColorOfDiffuseSkyAtomic(RwRGBA m_DiffuseSkyAtomicRGBA[3])
{
	RpGeometry*		pGeometry		= RpAtomicGetGeometry(m_pBaseSkyLayer[0]->_pAtom);
	RpMaterial*		pMaterial		= RpGeometryGetMaterial(pGeometry, 0);
	RwRGBA*			pPrelights		= RpGeometryGetPreLightColors(pGeometry);
	RwInt32			VertNum			= RpGeometryGetNumVertices(pGeometry);
	RpMorphTarget*	pMorphTarget	= RpGeometryGetMorphTarget(pGeometry, 0);
	RwV3d*			pVList			= RpMorphTargetGetVertices(pMorphTarget);

	RpGeometryLock(pGeometry, rpGEOMETRYLOCKPRELIGHT);

	RwInt32 CurHeight;
	for(int i = 0; i < VertNum; ++i)
	{
		CurHeight = static_cast<RwInt32>(pVList[i].y);

		if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT0)
		{
			pPrelights[i].red	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].red, 0, 255);
			pPrelights[i].green	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].green,	0, 255);
			pPrelights[i].blue	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].blue, 0, 255);
		}
		else if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT1)
		{
			pPrelights[i].red	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].red, 0, 255);
			pPrelights[i].green	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].green, 0,	255);
			pPrelights[i].blue	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].blue,	0, 255);
		}
		else if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT2)
		{
			pPrelights[i].red	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].red, 0, 255) + API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].red, 0, 255)) / 2;
			pPrelights[i].green	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].green, 0,	255) + API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].green, 0, 255)) / 2;
			pPrelights[i].blue	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[0].blue,	0, 255)	+ API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].blue, 0, 255))	/ 2;
		}
		else if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT3)
		{
			pPrelights[i].red	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].red, 0, 255);
			pPrelights[i].green	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].green, 0,	255);
			pPrelights[i].blue	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].blue,	0, 255);
		}
		else if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT4)
		{
			pPrelights[i].red	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].red, 0, 255) + API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].red, 0, 255)) / 2;
			pPrelights[i].green	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].green, 0,	255) + API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].green, 0, 255)) / 2;
			pPrelights[i].blue	= (API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[1].blue,	0, 255)	+ API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].blue, 0, 255))	/ 2;
		}
		else if(CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT5 /*|| CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT6*/)
		{
			pPrelights[i].red	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].red, 0, 255);	
			pPrelights[i].green	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].green, 0, 255);
			pPrelights[i].blue	= API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].blue, 0, 255);
		}
		else if (CurHeight == dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT6)
		{
			pPrelights[i].red = API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].red, 0, 255);
			pPrelights[i].green = API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].green, 0, 255);
			pPrelights[i].blue = API_PL_CLAMP(m_DiffuseSkyAtomicRGBA[2].blue, 0, 255);
		}
	}

	RpGeometryUnlock(pGeometry);
}

RwBool CNtlPLSky::SetProperty(const CNtlPLProperty* pData)
{
	NTL_FUNCTION("CNtlPLSky::SetProperty");

	NTL_RETURN(TRUE);
}

void CNtlPLSky::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		if (!m_bPassedSky)
		{
			RwBool			 NoVariation	= FALSE;
			sNTL_FIELD_PROP* pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
			m_InterpolationMode				= 0x00000000;
			m_RestTime4Change[0]			= -999.0f;
			m_RestTime4Change[1]			= -999.0f;
			m_RestTime4Change[2]			= -999.0f;

			// not variate
			eNTL_WORLD_DIR NWD = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetWorldFieldMoveDir();
			if(NWD == eIDLE || NWD == ePORTAL)
			{
				NoVariation = TRUE;
				if (pNtlFieldProp->_BaseSkyMode == eBSM_NOTHING)
				{
					m_bPassedSky = TRUE;
				}
			}
			RunableVariation(pNtlFieldProp, NoVariation);
		}
	}
	if (CheckEventsWorldEffectChangedWithOutDelay(pMsg))
	{
		RwBool			 NoVariation	= TRUE;
		sNTL_FIELD_PROP* pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		m_InterpolationMode				= 0x00000000;
		m_RestTime4Change[0]			= -999.0f;
		m_RestTime4Change[1]			= -999.0f;
		m_RestTime4Change[2]			= -999.0f;

		if (pNtlFieldProp->_BaseSkyMode == eBSM_NOTHING)
		{
			m_bPassedSky = TRUE;
		}
		else if (m_bPassedSky)
		{
			m_bPassedSky = FALSE;
			RunableVariation(pNtlFieldProp, NoVariation);
		}
	}
}

void CNtlPLSky::RunableVariation(sNTL_FIELD_PROP* pNtlFieldProp, RwBool NoVariation)
{
	//DBO_WARNING_MESSAGE("_BaseSkyMode: " << pNtlFieldProp->_BaseSkyMode << " _NewSkyValue: " << pNtlFieldProp->_NewSkyValue << " _BaseSkyTexName: " << pNtlFieldProp->_BaseSkyTexName);
	// decide interpolation of RGB color; dSKY_RGB_EFFECT_SWITCHING_TIME
	if(pNtlFieldProp->_BaseSkyMode == eBSM_TEX_MODE && GetBaseSkyMode() == eBSM_TEX_MODE)
	{
		if(	strcmp(pNtlFieldProp->_BaseSkyTexName, API_PL_AtomicGetTexture(m_pBaseSkyLayer[1]->_pAtom)->name) ||
			strcmp(pNtlFieldProp->_BlendedTexName[0], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name) ||
			strcmp(pNtlFieldProp->_BlendedTexName[1], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name))
		{
			m_InterpolationMode |= dSKY_INTERPOLATION_TEX_2_TEX;
			m_pBaseSkyLayer[0]->_pRender = FALSE;
			m_pBaseSkyLayer[1]->_pRender = TRUE;

			RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BaseSkyTexName, "texture\\ntlwe\\sky\\");
			DBO_ASSERT(pTexture, "Texture load failed.");

			API_PL_AtomicSetTexture(m_pBaseSkyLayer[1]->_pAtom, pTexture);
			API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);
			RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
			CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
		}
	}

	if(pNtlFieldProp->_BaseSkyMode == eBSM_TEX_MODE && GetBaseSkyMode() == eBSM_RGB_MODE)
	{
		m_InterpolationMode |= dSKY_INTERPOLATION_TEX_2_RGB;
		m_pBaseSkyLayer[0]->_pRender = FALSE;
		m_pBaseSkyLayer[1]->_pRender = TRUE;

		RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BaseSkyTexName, "texture\\ntlwe\\sky\\");
		DBO_ASSERT(pTexture, "Texture load failed.");

		API_PL_AtomicSetTexture(m_pBaseSkyLayer[1]->_pAtom, pTexture);
		API_PL_AtomicSetMaterialSetAlpha(m_pBaseSkyLayer[1]->_pAtom, 255);
		RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);
	}

	if(pNtlFieldProp->_BaseSkyMode == eBSM_RGB_MODE && GetBaseSkyMode() == eBSM_TEX_MODE)
	{
		m_InterpolationMode |= dSKY_INTERPOLATION_RGB_2_TEX;
		m_pBaseSkyLayer[0]->_pRender = TRUE;
		m_pBaseSkyLayer[1]->_pRender = FALSE;

		SetColorOfDiffuseSkyAtomic(pNtlFieldProp->_RGBSkyColor);
	}

	if(pNtlFieldProp->_BaseSkyMode == eBSM_RGB_MODE && GetBaseSkyMode() == eBSM_RGB_MODE)
	{
		if(	pNtlFieldProp->_RGBSkyColor[0].red != m_pBaseSkyLayer[0]->_RGBA[0].red ||
			pNtlFieldProp->_RGBSkyColor[0].green != m_pBaseSkyLayer[0]->_RGBA[0].green ||
			pNtlFieldProp->_RGBSkyColor[0].blue != m_pBaseSkyLayer[0]->_RGBA[0].blue ||
			pNtlFieldProp->_RGBSkyColor[1].red != m_pBaseSkyLayer[0]->_RGBA[1].red ||
			pNtlFieldProp->_RGBSkyColor[1].green != m_pBaseSkyLayer[0]->_RGBA[1].green ||
			pNtlFieldProp->_RGBSkyColor[1].blue != m_pBaseSkyLayer[0]->_RGBA[1].blue ||
			pNtlFieldProp->_RGBSkyColor[2].red != m_pBaseSkyLayer[0]->_RGBA[2].red ||
			pNtlFieldProp->_RGBSkyColor[2].green != m_pBaseSkyLayer[0]->_RGBA[2].green ||
			pNtlFieldProp->_RGBSkyColor[2].blue != m_pBaseSkyLayer[0]->_RGBA[2].blue)
		{
			m_InterpolationMode	   |= dSKY_INTERPOLATION_RGB_2_RGB;

			if(NoVariation)
			{
				m_RestTime4Change[0] = RwRealMINVAL;
			}
			else
			{
				m_RestTime4Change[0] = dSKY_RGB_EFFECT_SWITCHING_TIME;
			}

			memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[0], &m_pBaseSkyLayer[0]->_RGBA[0], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[1], &m_pBaseSkyLayer[0]->_RGBA[1], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[2], &m_pBaseSkyLayer[0]->_RGBA[2], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBATo[0], &pNtlFieldProp->_RGBSkyColor[0], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBATo[1], &pNtlFieldProp->_RGBSkyColor[1], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBATo[2], &pNtlFieldProp->_RGBSkyColor[2], sizeof(RwRGBA));
		}
	}

	// decide interpolation of opacity; dSKY_OPA_EFFECT_SWITCHING_TIME
	if(!strcmp(pNtlFieldProp->_BlendedTexName[0], "null"))
	{
		m_pBlendedTmpLayer[0]->_pRender = FALSE;

		if(m_pBlendedSkyLayer[0]->_pRender)
		{
			if(strcmp(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name, pNtlFieldProp->_BlendedTexName[0]))
			{
				m_InterpolationMode |= dSKY_INTERPOLATION_OPA_PASS1;

				if(NoVariation)
				{
					m_RestTime4Change[1] = RwRealMINVAL;
				}
				else
				{
					m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
				}

				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
				m_pBlendedSkyLayer[0]->_AutoDelete = TRUE;
			}
		}
	}
	else
	{
		// compared to the current one
		bool SkipTest = FALSE;
		if(m_pBlendedSkyLayer[0]->_pRender)
		{
			if(!strcmp(pNtlFieldProp->_BlendedTexName[0], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name))
			{
				SkipTest = TRUE;
			}
		}

		if(!SkipTest)
		{
			m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS1;

			if(NoVariation)
			{
				m_RestTime4Change[1] = RwRealMINVAL;
			}
			else
			{
				m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
			}

			m_pBlendedTmpLayer[0]->_pRender = TRUE;

			RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[0], "texture\\ntlwe\\sky\\");
			DBO_ASSERT(pTexture, "Texture load failed.");

			API_PL_AtomicSetTexture(m_pBlendedTmpLayer[0]->_pAtom, pTexture);
			API_PL_AtomicSetMaterialSetAlpha(m_pBlendedTmpLayer[0]->_pAtom, 0);
			RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
			CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);

			if(m_pBlendedSkyLayer[0]->_pRender)
			{
				if(strcmp(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[0]->_pAtom)->name, pNtlFieldProp->_BlendedTexName[0]))
				{
					m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS1;

					if(NoVariation)
					{
						m_RestTime4Change[1] = RwRealMINVAL;
					}
					else
					{
						m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
					}

					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
					m_pBlendedSkyLayer[0]->_AutoDelete	= TRUE;
					m_pBlendedSkyLayer[0]->_pRender		= TRUE;
				}
			}
		}
	}

	if(!strcmp(pNtlFieldProp->_BlendedTexName[1], "null"))
	{
		m_pBlendedTmpLayer[1]->_pRender = FALSE;

		if(m_pBlendedSkyLayer[1]->_pRender)
		{
			if(strcmp(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name, pNtlFieldProp->_BlendedTexName[1]))
			{
				m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;
				m_RestTime4Change[1]	= dSKY_OPA_EFFECT_SWITCHING_TIME;

				if(NoVariation)
				{
					m_RestTime4Change[1] = RwRealMINVAL;
				}
				else
				{
					m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
				}

				API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
				m_pBlendedSkyLayer[1]->_AutoDelete = TRUE;
			}
		}
	}
	else
	{
		// compared to the current one
		bool SkipTest = FALSE;
		if(m_pBlendedSkyLayer[1]->_pRender)
		{
			if(!strcmp(pNtlFieldProp->_BlendedTexName[1], API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name))
			{
				SkipTest = TRUE;
			}
		}

		if(!SkipTest)
		{
			m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;

			if(NoVariation)
			{
				m_RestTime4Change[1] = RwRealMINVAL;
			}
			else
			{
				m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
			}

			m_pBlendedTmpLayer[1]->_pRender = TRUE;

			RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[1], "texture\\ntlwe\\sky\\");
			DBO_ASSERT(pTexture, "Texture load failed.");

			API_PL_AtomicSetTexture(m_pBlendedTmpLayer[1]->_pAtom, pTexture);
			API_PL_AtomicSetMaterialSetAlpha(m_pBlendedTmpLayer[1]->_pAtom, 0);
			RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
			CNtlPLResourceManager::GetInstance()->UnLoadTexture(pTexture);

			if(m_pBlendedSkyLayer[1]->_pRender)
			{
				if(strcmp(API_PL_AtomicGetTexture(m_pBlendedSkyLayer[1]->_pAtom)->name, pNtlFieldProp->_BlendedTexName[1]))
				{
					m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;

					if(NoVariation)
					{
						m_RestTime4Change[1] = RwRealMINVAL;
					}
					else
					{
						m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
					}

					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
					m_pBlendedSkyLayer[1]->_AutoDelete	= TRUE;
					m_pBlendedSkyLayer[1]->_pRender		= TRUE;
				}
			}
		}
	}

	// decide interpolation of speed; dSKY_ROT_EFFECT_SWITCHING_TIME
	if(m_pBaseSkyLayer[1]->_RotSpeed != pNtlFieldProp->_BaseSkySpeed)
	{
		m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS0;

		if(NoVariation)
		{
			m_RestTime4Change[2] = RwRealMINVAL;
		}
		else
		{
			m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
		}

		m_pBaseSkyLayer[1]->_RotSpeedFrom	= m_pBaseSkyLayer[1]->_RotSpeedMPS;	
		m_pBaseSkyLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BaseSkySpeed;
	}

	if(m_pBlendedSkyLayer[0]->_RotSpeed != pNtlFieldProp->_BlendedTexSpeed[0])
	{
		m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS1;

		if(NoVariation)
		{
			m_RestTime4Change[2] = RwRealMINVAL;
		}
		else
		{
			m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
		}

		m_pBlendedSkyLayer[0]->_RotSpeedFrom	= m_pBlendedSkyLayer[0]->_RotSpeedMPS;
		m_pBlendedSkyLayer[0]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[0];
		m_pBlendedTmpLayer[0]->_RotSpeedFrom	= m_pBlendedSkyLayer[0]->_RotSpeedMPS;
		m_pBlendedTmpLayer[0]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[0];	
	}

	if(m_pBlendedSkyLayer[1]->_RotSpeed != pNtlFieldProp->_BlendedTexSpeed[1])
	{
		m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS2;

		if(NoVariation)
		{
			m_RestTime4Change[2] = RwRealMINVAL;
		}
		else
		{
			m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
		}

		m_pBlendedSkyLayer[1]->_RotSpeedFrom	= m_pBlendedSkyLayer[1]->_RotSpeedMPS;
		m_pBlendedSkyLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[1];
		m_pBlendedTmpLayer[1]->_RotSpeedFrom	= m_pBlendedSkyLayer[1]->_RotSpeedMPS;
		m_pBlendedTmpLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[1];	
	}
}

void CNtlPLSky::UpdateFieldVariation(RwReal fElapsed)
{
	if(m_RestTime4Change[0] >= 0.0f)
	{
		m_RestTime4Change[0] -= g_GetElapsedTime();
		if(m_RestTime4Change[0] < 0)
		{
			m_RestTime4Change[0]	= -999.0f;
			memcpy(&m_pBaseSkyLayer[0]->_RGBA[0], &m_pBaseSkyLayer[0]->_RGBATo[0], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBA[1], &m_pBaseSkyLayer[0]->_RGBATo[1], sizeof(RwRGBA));
			memcpy(&m_pBaseSkyLayer[0]->_RGBA[2], &m_pBaseSkyLayer[0]->_RGBATo[2], sizeof(RwRGBA));

			SetColorOfDiffuseSkyAtomic(m_pBaseSkyLayer[0]->_RGBA);
		}
		else
		{
			for(int i = 0; i < 3; ++i)
			{
				m_pBaseSkyLayer[0]->_RGBA[i].red	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].red, m_pBaseSkyLayer[0]->_RGBATo[i].red, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
				m_pBaseSkyLayer[0]->_RGBA[i].green	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].green, m_pBaseSkyLayer[0]->_RGBATo[i].green, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
				m_pBaseSkyLayer[0]->_RGBA[i].blue	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].blue, m_pBaseSkyLayer[0]->_RGBATo[i].blue, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
			}

			SetColorOfDiffuseSkyAtomic(m_pBaseSkyLayer[0]->_RGBA);
		}
	}

	if(m_RestTime4Change[1] >= 0.0f)
	{
		m_RestTime4Change[1] -= g_GetElapsedTime();

		if(m_RestTime4Change[1] < 0)
		{
			m_RestTime4Change[1] = -999.0f;

			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS1)
			{
				if(m_pBlendedSkyLayer[0]->_pRender)
				{
					if(m_pBlendedSkyLayer[0]->_AutoDelete)
					{
						m_pBlendedSkyLayer[0]->_pRender = FALSE;
					}
					else
					{
						API_PL_AtomicSetTexture(m_pBlendedSkyLayer[0]->_pAtom, API_PL_AtomicGetTexture(m_pBlendedTmpLayer[0]->_pAtom));
						m_pBlendedSkyLayer[0]->_pRender = TRUE;
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
					}		
				}

				if(m_pBlendedTmpLayer[0]->_pRender)
				{
					API_PL_AtomicSetTexture(m_pBlendedSkyLayer[0]->_pAtom, API_PL_AtomicGetTexture(m_pBlendedTmpLayer[0]->_pAtom));
					m_pBlendedSkyLayer[0]->_pRender = TRUE;
					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, 255);
					API_PL_AtomicSetTexture(m_pBlendedTmpLayer[0]->_pAtom, NULL);
					m_pBlendedTmpLayer[0]->_pRender = FALSE;
				}
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS2)
			{
				if(m_pBlendedSkyLayer[1]->_pRender)
				{
					if(m_pBlendedSkyLayer[1]->_AutoDelete)
					{
						m_pBlendedSkyLayer[1]->_pRender = FALSE;
					}
					else
					{	
						m_pBlendedSkyLayer[1]->_pRender = TRUE;
						API_PL_AtomicSetTexture(m_pBlendedSkyLayer[1]->_pAtom, API_PL_AtomicGetTexture(m_pBlendedTmpLayer[1]->_pAtom));
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, 255);
					}		
				}

				if(m_pBlendedTmpLayer[1]->_pRender)
				{
					API_PL_AtomicSetTexture(m_pBlendedSkyLayer[1]->_pAtom, API_PL_AtomicGetTexture(m_pBlendedTmpLayer[1]->_pAtom));
					m_pBlendedSkyLayer[1]->_pRender = TRUE;
					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, 255);

					API_PL_AtomicSetTexture(m_pBlendedTmpLayer[1]->_pAtom, NULL);
					m_pBlendedTmpLayer[1]->_pRender = FALSE;
				}
			}
		}
		else
		{
			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS1)
			{
				if(m_pBlendedSkyLayer[0]->_pRender)
				{
					if(m_pBlendedSkyLayer[0]->_AutoDelete)
					{
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, RpNtlWorldUInt8PropLerp(255, 0, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
					}
					else
					{
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[0]->_pAtom, RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
					}
				}

				if(m_pBlendedTmpLayer[0]->_pRender)
				{
					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedTmpLayer[0]->_pAtom, RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
				}
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS2)
			{
				if(m_pBlendedSkyLayer[1]->_pRender)
				{
					if(m_pBlendedSkyLayer[1]->_AutoDelete)
					{
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, RpNtlWorldUInt8PropLerp(255, 0, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
					}
					else
					{
						API_PL_AtomicSetMaterialSetAlpha(m_pBlendedSkyLayer[1]->_pAtom, RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
					}
				}

				if(m_pBlendedTmpLayer[1]->_pRender)
				{
					API_PL_AtomicSetMaterialSetAlpha(m_pBlendedTmpLayer[1]->_pAtom, RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
				}
			}
		}
	}

	if(m_RestTime4Change[2] >= 0.0f)
	{
		m_RestTime4Change[2] -= g_GetElapsedTime();

		if(m_RestTime4Change[2] < 0)
		{
			m_RestTime4Change[2] = -999.0f;

			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS0)
			{
				m_pBaseSkyLayer[1]->_RotSpeedMPS = m_pBaseSkyLayer[1]->_RotSpeedTo;
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS1)
			{
				m_pBlendedSkyLayer[0]->_RotSpeedMPS = m_pBlendedSkyLayer[0]->_RotSpeedTo;
				m_pBlendedTmpLayer[0]->_RotSpeedMPS = m_pBlendedTmpLayer[0]->_RotSpeedTo;
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS2)
			{
				m_pBlendedSkyLayer[1]->_RotSpeedMPS = m_pBlendedSkyLayer[1]->_RotSpeedTo;
				m_pBlendedTmpLayer[1]->_RotSpeedMPS = m_pBlendedTmpLayer[1]->_RotSpeedTo;
			}
		}
		else
		{
			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS0)
			{
				m_pBaseSkyLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBaseSkyLayer[1]->_RotSpeedFrom, m_pBaseSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS1)
			{
				m_pBlendedSkyLayer[0]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[0]->_RotSpeedFrom, m_pBlendedSkyLayer[0]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
				m_pBlendedTmpLayer[0]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[0]->_RotSpeedFrom, m_pBlendedSkyLayer[0]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
			}

			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS2)
			{
				m_pBlendedSkyLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[1]->_RotSpeedFrom, m_pBlendedSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
				m_pBlendedTmpLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[1]->_RotSpeedFrom, m_pBlendedSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
			}
		}
	}
}

RpAtomic* CNtlPLSky::GetAtomic()
{ 
	// _pRender flag isn't always updated
	//m_pBlendedSkyLayer[1]->Rotate(m_pBlendedSkyLayer[1]->_RotSpeedMPS);

	API_PL_TranslationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEREPLACE);

	return m_pBaseSkyLayer[1]->_pAtom;
}

RwBool CNtlPLSky::Update(RwReal fElapsed)
{
	if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE || m_bPassedSky)
		return TRUE;

	UpdateFieldVariation(fElapsed);

	//API_PL_TranslationAtomic(m_pDummySkyLayer->_pAtom, m_WorldDatumPos, rwCOMBINEREPLACE);

	m_pBaseSkyLayer[1]->Rotate(m_pBaseSkyLayer[1]->_RotSpeedMPS);
	m_pBlendedSkyLayer[0]->Rotate(m_pBlendedSkyLayer[0]->_RotSpeedMPS);
	m_pBlendedSkyLayer[1]->Rotate(m_pBlendedSkyLayer[1]->_RotSpeedMPS);
	m_pBlendedTmpLayer[0]->Rotate(m_pBlendedTmpLayer[0]->_RotSpeedMPS);
	m_pBlendedTmpLayer[1]->Rotate(m_pBlendedTmpLayer[1]->_RotSpeedMPS);
	API_PL_RotationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_pBaseSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedSkyLayer[0]->_pAtom, m_pBlendedSkyLayer[0]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedSkyLayer[1]->_pAtom, m_pBlendedSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedTmpLayer[0]->_pAtom, m_pBlendedTmpLayer[0]->_RotSpeed, rwCOMBINEREPLACE);
	API_PL_RotationAtomic(m_pBlendedTmpLayer[1]->_pAtom, m_pBlendedTmpLayer[1]->_RotSpeed, rwCOMBINEREPLACE);

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

	if(m_pBlendedSkyLayer[1]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	if(m_pBlendedTmpLayer[0]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedTmpLayer[0]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	if(m_pBlendedTmpLayer[1]->_pRender)
	{
		API_PL_TranslationAtomic(m_pBlendedTmpLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
	}

	return TRUE;
}

void CNtlPLSky::SkyLayerRender(sSKY_LAYER* pSkyLayer, RwInt32 iTFactor)
{
	if (pSkyLayer->_pRender)
 	{
 		RpMaterial*	pMaterial	= API_PL_AtomicGetMaterial(pSkyLayer->_pAtom);
 		RwRGBA		RGBATmp, RGBABk;
 
 		memcpy(&RGBABk, RpMaterialGetColor(pMaterial), sizeof(RwRGBA));
 		memcpy(&RGBATmp, RpMaterialGetColor(pMaterial), sizeof(RwRGBA));
 
 		::API_PL_8BColorClamp(&RGBATmp.red, iTFactor);
 		::API_PL_8BColorClamp(&RGBATmp.green, iTFactor);
 		::API_PL_8BColorClamp(&RGBATmp.blue, iTFactor);
 
 		RpMaterialSetColor(pMaterial, &RGBATmp);
 		RpAtomicRender(pSkyLayer->_pAtom);
 		RpMaterialSetColor(pMaterial, &RGBABk);
 	}
}

RwBool CNtlPLSky::Render(void)
{
	NTL_SPROFILE("CNtlPLSky::Render")

	RpNtlActiveCameraSetFarDistance(GetNtlPLOptionManager()->GetInstance()->GetTerrainFar());

	if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE || m_bPassedSky)
	{
		NTL_RPROFILE(TRUE)
	}

	if(!m_VisibilityFlag)
	{
		NTL_RPROFILE(TRUE)
	}

	RwInt32 TFactor = -static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff);

	BegBaseSkyRenderState();
	{
		SkyLayerRender(m_pBaseSkyLayer[0], TFactor);

		SkyLayerRender(m_pBaseSkyLayer[1], TFactor);

		// 구름과 SkyBox 사이에 해/달을 렌더링한다. (2007.08.06 by agebreak)
		CNtlPLVisualManager* pVisualManager = (CNtlPLVisualManager*)GetSceneManager();
		CNtlPLRenderGroup* pSunGroup = pVisualManager->FindInstanceGroup(PLENTITY_SUN);
		if(pSunGroup)
		{
			CNtlPLRenderGroup::MapEntity::iterator the;
			CNtlPLRenderGroup::MapEntity *pmapEntity = pSunGroup->GetEntities();
			for(the = pmapEntity->begin(); the != pmapEntity->end(); ++the)
			{
				CNtlPLSun* pSun = static_cast<CNtlPLSun*>((*the).second);
				if(pSun && pSun->GetVisibility())
					pSun->Render();
			}
		}

		SkyLayerRender(m_pBlendedSkyLayer[0], TFactor);
		SkyLayerRender(m_pBlendedSkyLayer[1], TFactor);

		SkyLayerRender(m_pBlendedTmpLayer[0], TFactor);
		SkyLayerRender(m_pBlendedTmpLayer[1], TFactor);
	}
	EndBaseSkyRenderState();

	BegDummySkyRenderState();
	{
		SkyLayerRender(m_pDummySkyLayer, TFactor);
	}
	EndDummySkyRenderState();

	// LIGHTNING
	if (m_fTimeLightning > m_fTimeFadeOut)
	{
		m_fTimeLightning -= g_GetElapsedTime();
		if (m_fTimeLightning >= 0.0f)
		{
			 m_clrLightning.alpha = static_cast<RwUInt8>((1.0f - (m_fTimeLightning / m_fTimeFadeIn)) * 255.0f);
		}
		else if (m_fTimeLightning <= m_fTimeFadeOut)
		{
			m_clrLightning.alpha = 0;
		}
		else
		{
			m_clrLightning.alpha = static_cast<RwUInt8>((1.0f - (m_fTimeLightning / m_fTimeFadeOut)) * 255.0f);
		}
	}

	if (m_clrLightning.alpha)
	{
		RwIm2DVertex	vIm2D[4];
		RwReal			fWidth	= static_cast<RwReal>(CNtlPLGlobal::m_RwCamera->frameBuffer->width);
		RwReal			fHeight	= static_cast<RwReal>(CNtlPLGlobal::m_RwCamera->frameBuffer->height);

 		RwIm2DVertexSetScreenX(&vIm2D[0], 0.0f);
 		RwIm2DVertexSetScreenY(&vIm2D[0], 0.0f);
 		RwIm2DVertexSetScreenX(&vIm2D[1], 0.0f);
 		RwIm2DVertexSetScreenY(&vIm2D[1], fHeight);
 		RwIm2DVertexSetScreenX(&vIm2D[2], fWidth);
 		RwIm2DVertexSetScreenY(&vIm2D[2], 0.0f);
 		RwIm2DVertexSetScreenX(&vIm2D[3], fWidth);
 		RwIm2DVertexSetScreenY(&vIm2D[3], fHeight);

		RwIm2DVertexSetIntRGBA(&vIm2D[0], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
		RwIm2DVertexSetIntRGBA(&vIm2D[1], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
		RwIm2DVertexSetIntRGBA(&vIm2D[2], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
		RwIm2DVertexSetIntRGBA(&vIm2D[3], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);

		BeginSkyLightningRenderState();

		RwD3D9SetTexture(NULL, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vIm2D, 4);

		EndSkyLightningRenderState();
	}

	NTL_RPROFILE(TRUE)
}

void CNtlPLSky::RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut)
{
	m_fTimeFadeIn	= fTimeFadeIn;
	m_fTimeFadeOut	= -fTimeFadeOut;
	m_fTimeLightning = m_fTimeFadeIn * (1.0f - (static_cast<RwReal>(m_clrLightning.alpha) / 255.0f));
}

// #include "precomp_ntlpresentation.h"
// #include "NtlPLEntityFreeList.h"
// #include "NtlPLGlobal.h"
// #include "NtlPLRenderState.h"
// #include "NtlPLSky.h"
// #include "NtlPLResourceManager.h"
// #include "ntlworlddi.h"
// #include "NtlPLEvent.h"
// #include "ntlworldcommon.h"
// #include "NtlPLEvent.h"
// #include "ceventhandler.h"
// #include "ntlworldinterface.h"
// #include "NtlPLVisualManager.h"
// #include "NtlPLWorldEntity.h"
// #include "NtlWorldFieldManager.h"
// #include "NtlProfiler.h"
// 
// 
// CNtlPLSky::CNtlPLSky()
// {
// 	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
// 	SetLayer(PLENTITY_LAYER_SKY);
// 	SetClassType(PLENTITY_SKY);
// 
// 	m_pDummySkyLayer		= NULL;
// 	m_pBaseSkyLayer[0]		= NULL;
// 	m_pBaseSkyLayer[1]		= NULL;
// 	m_pBlendedSkyLayer[0]	= NULL;
// 	m_pBlendedSkyLayer[1]	= NULL;
// 	m_pBlendedTmpLayer[0]	= NULL;
// 	m_pBlendedTmpLayer[1]	= NULL;
// 	m_VisibilityFlag		= TRUE;
// 
// 	m_RestTime4Change[0]	= -999.0f;
// 	m_RestTime4Change[1]	= -999.0f;
// 	m_RestTime4Change[2]	= -999.0f;
// 
// 	m_clrLightning.alpha	= 0;
// 	m_clrLightning.red		= 255;
// 	m_clrLightning.green	= 255;
// 	m_clrLightning.blue		= 255;
// 
// 	m_fTimeFadeIn			= 0.0f;
// 	m_fTimeFadeOut			= 0.0f;
// 	m_fTimeLightning		= 0.0f;
// 
// 	for (int i = 0; i < 30; ++i)
// 	{
// 		bRenderSky[i] = TRUE;
// 	}
// }
// 
// CNtlPLSky::~CNtlPLSky()
// {
// }
// 
// RwBool CNtlPLSky::Create(const SPLEntityCreateParam* pParam)
// {
// 	NTL_FUNCTION("CNtlPLSky::Create");
// 
// 	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
// 
// 	// Dummy sky
// 	CreateSkyLayer(&m_pDummySkyLayer, "world\\test\\sky_dummy.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, TRUE, FALSE);
// 
// 	// Diffuse sky atomic
// 	CreateSkyLayer(&m_pBaseSkyLayer[0], "world\\test\\sky_base0.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR, TRUE, FALSE);
// 
// 	// Base sky atomic
// 	CreateSkyLayer(&m_pBaseSkyLayer[1], "world\\test\\sky_base1.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, FALSE, TRUE);
// 
// 	// blending temporary sky atomics
// 	CreateSkyLayer(&m_pBlendedTmpLayer[0], "world\\test\\sky_temp1.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, FALSE, TRUE);
// 	CreateSkyLayer(&m_pBlendedTmpLayer[1], "world\\test\\sky_temp1.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, FALSE, TRUE);
// 
// 	// blending sky atomics
// 	CreateSkyLayer(&m_pBlendedSkyLayer[0], "world\\test\\sky_Blend0.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, FALSE, TRUE);
// 	CreateSkyLayer(&m_pBlendedSkyLayer[1], "world\\test\\sky_Blend1.dff", "texture/ntlwe/Sky/", rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED, FALSE, TRUE);
// 
// 	RwRGBA	RGBATmp[3];
// 	RGBATmp[0].red		= 255;
// 	RGBATmp[0].green	= 255;
// 	RGBATmp[0].blue		= 255;
// 	RGBATmp[1].red		= 255;
// 	RGBATmp[1].green	= 255;
// 	RGBATmp[1].blue		= 255;
// 	RGBATmp[2].red		= 255;
// 	RGBATmp[2].green	= 255;
// 	RGBATmp[2].blue		= 255;
// 
// 	SetSkyLayerDiffuseColor(m_pBaseSkyLayer[0], RGBATmp);
// 	SetSkyLayerAlpha(m_pBaseSkyLayer[0], 255);
// 
// 	NTL_RETURN(TRUE);
// }
// 
// void CNtlPLSky::CreateSkyLayer(sSKY_LAYER** ppSkyLayer, const char *pStrName, const char * pResourcePath, RwUInt32 eGeoFlag, RwBool bRender, RwBool bClearTextrue)
// {
// 	if (*ppSkyLayer)
// 	{
// 		DestroySkyLayer(ppSkyLayer);
// 	}
// 
// 	sSKY_LAYER* pSkyLayer	= NTL_NEW sSKY_LAYER;
// 
// 	pSkyLayer->_pClumpRes	= CNtlPLResourceManager::GetInstance()->LoadClump(pStrName, pResourcePath);
// 	pSkyLayer->_pRender		= bRender;
// 
// 	Helper_SetClumpAllAtomics(pSkyLayer->_pClumpRes->GetClump(), &pSkyLayer->_vecAtomic);	
// 
// 	for (int i = 0; i < (int)pSkyLayer->_vecAtomic.size(); ++i)
// 	{
// 		API_PL_AtomicSetGeoFlags(pSkyLayer->_vecAtomic[i]->pData, eGeoFlag);
// 	}
// 
// 	if (bClearTextrue)
// 	{
// 		//SetSkyLayerTextrue(pSkyLayer, NULL);
// 	}
// 
// 	*ppSkyLayer = pSkyLayer;
// }
// 
// void CNtlPLSky::DestroySkyLayer(sSKY_LAYER** ppSkyLayer)
// {
// 	if (*ppSkyLayer)
// 	{
// 		NTL_DELETE(*ppSkyLayer);
// 		*ppSkyLayer = NULL;
// 	}
// }
// 
// void CNtlPLSky::Destroy() 
// {
// 	NTL_FUNCTION("CNtlPLSky::Destroy");
// 
// 	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
// 
// 	DestroySkyLayer(&m_pDummySkyLayer);
// 	DestroySkyLayer(&m_pBaseSkyLayer[0]);
// 	DestroySkyLayer(&m_pBaseSkyLayer[1]);
// 	DestroySkyLayer(&m_pBlendedSkyLayer[0]);
// 	DestroySkyLayer(&m_pBlendedSkyLayer[1]);
// 	DestroySkyLayer(&m_pBlendedTmpLayer[0]);
// 	DestroySkyLayer(&m_pBlendedTmpLayer[1]);
// 
// 	NTL_RETURNVOID();
// }
// 
// eBASE_SKY_MODE CNtlPLSky::GetBaseSkyMode()
// {
// 	if(m_pBaseSkyLayer[0]->_pRender)
// 	{
// 		return eBSM_RGB_MODE;
// 	}
// 	else if(m_pBaseSkyLayer[1]->_pRender)
// 	{
// 		return eBSM_TEX_MODE;
// 	}
// 
// 	return eBSM_NOTHING;
// }
// 
// void CNtlPLSky::SetSkyLayerDiffuseColor(sSKY_LAYER* pSkyLayer, RwRGBA rgba[3])
// {
// 	for (int j = 0; j < (int)pSkyLayer->_vecAtomic.size(); ++j)
// 	{
// 		RpAtomic*		pAtomic			= pSkyLayer->_vecAtomic[j]->pData;
// 		RpGeometry*		pGeometry		= RpAtomicGetGeometry(pAtomic);
// 		RpMaterial*		pMaterial		= RpGeometryGetMaterial(pGeometry, 0);
// 		RwRGBA*			pPrelights		= RpGeometryGetPreLightColors(pGeometry);
// 		RwInt32			iVertNum		= RpGeometryGetNumVertices(pGeometry);
// 		RpMorphTarget*	pMorphTarget	= RpGeometryGetMorphTarget(pGeometry, 0);
// 		RwV3d*			pVList			= RpMorphTargetGetVertices(pMorphTarget);
// 
// 		RpGeometryLock(pGeometry, rpGEOMETRYLOCKPRELIGHT);
// 
// 		RwInt32 iCurHeight;
// 		for(int i = 0; i < iVertNum; ++i)
// 		{
// 			iCurHeight = static_cast<RwInt32>(pVList[i].y);
// 			if(iCurHeight == 300)
// 			{
// 				pPrelights[i].red	= API_PL_CLAMP(rgba[0].red, 0, 255);
// 				pPrelights[i].green	= API_PL_CLAMP(rgba[0].green,	0, 255);
// 				pPrelights[i].blue	= API_PL_CLAMP(rgba[0].blue, 0, 255);
// 			}
// 			else if(iCurHeight == 245)
// 			{
// 				pPrelights[i].red	= API_PL_CLAMP(rgba[0].red, 0, 255);
// 				pPrelights[i].green	= API_PL_CLAMP(rgba[0].green, 0,	255);
// 				pPrelights[i].blue	= API_PL_CLAMP(rgba[0].blue,	0, 255);
// 			}
// 			else if(iCurHeight == 180)
// 			{
// 				pPrelights[i].red	= (API_PL_CLAMP(rgba[0].red, 0, 255) + API_PL_CLAMP(rgba[1].red, 0, 255)) / 2;
// 				pPrelights[i].green	= (API_PL_CLAMP(rgba[0].green, 0,	255) + API_PL_CLAMP(rgba[1].green, 0, 255)) / 2;
// 				pPrelights[i].blue	= (API_PL_CLAMP(rgba[0].blue,	0, 255)	+ API_PL_CLAMP(rgba[1].blue, 0, 255))	/ 2;
// 			}
// 			else if(iCurHeight == 100)
// 			{
// 				pPrelights[i].red	= API_PL_CLAMP(rgba[1].red, 0, 255);
// 				pPrelights[i].green	= API_PL_CLAMP(rgba[1].green, 0,	255);
// 				pPrelights[i].blue	= API_PL_CLAMP(rgba[1].blue,	0, 255);
// 			}
// 			else if(iCurHeight == 40)
// 			{
// 				pPrelights[i].red	= (API_PL_CLAMP(rgba[1].red, 0, 255) + API_PL_CLAMP(rgba[2].red, 0, 255)) / 2;
// 				pPrelights[i].green	= (API_PL_CLAMP(rgba[1].green, 0,	255) + API_PL_CLAMP(rgba[2].green, 0, 255)) / 2;
// 				pPrelights[i].blue	= (API_PL_CLAMP(rgba[1].blue,	0, 255)	+ API_PL_CLAMP(rgba[2].blue, 0, 255))	/ 2;
// 			}
// 			else if(iCurHeight == 0)
// 			{
// 				pPrelights[i].red	= API_PL_CLAMP(rgba[2].red, 0, 255);	
// 				pPrelights[i].green	= API_PL_CLAMP(rgba[2].green, 0, 255);
// 				pPrelights[i].blue	= API_PL_CLAMP(rgba[2].blue, 0, 255);
// 			}
// 		}
// 
// 		RpGeometryUnlock(pGeometry);
// 	}
// }
// 
// void CNtlPLSky::SetSkyLayerTextrue(sSKY_LAYER* pSkyLayer, RwTexture* pTexture)
// {
// 	for (int j = 0; j < (int)pSkyLayer->_vecAtomic.size(); ++j)
// 	{
// 		API_PL_AtomicSetTexture(pSkyLayer->_vecAtomic[j]->pData, pTexture);
// 	}
// 	if (pSkyLayer->_pTexture)
// 	{
// 		//CNtlPLResourceManager::GetInstance()->UnLoadTexture(pSkyLayer->_pTexture);
// 	}
// 	pSkyLayer->_pTexture = pTexture;
// }
// 
// void CNtlPLSky::SetSkyLayerAlpha(sSKY_LAYER* pSkyLayer, RwUInt8 uiAlpha)
// {
// 	for (int j = 0; j < (int)pSkyLayer->_vecAtomic.size(); ++j)
// 	{
// 		API_PL_AtomicSetMaterialSetAlpha(pSkyLayer->_vecAtomic[j]->pData, uiAlpha);
// 	}
// }
// 
// void CNtlPLSky::SkyLayerTranslation(sSKY_LAYER* pSkyLayer, RwV3d* pvPos, RwOpCombineType rwOpCombineType)
// {	
// 	RwFrame* pFrame = RpClumpGetFrame(pSkyLayer->_pClumpRes->GetClump());
// 	pFrame = RwFrameTranslate(pFrame, pvPos, rwOpCombineType);
// 	RwFrameUpdateObjects(pFrame);
// }
// 
// void CNtlPLSky::SkyLayerRotation(sSKY_LAYER* pSkyLayer, RwReal fRot, RwOpCombineType rwOpCombineType)
// {
// 	RwFrame* pFrame = RpClumpGetFrame(pSkyLayer->_pClumpRes->GetClump());
// 	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, fRot, rwOpCombineType);
// 	RwFrameUpdateObjects(pFrame);
// }
// 
// RwBool CNtlPLSky::SetProperty(const CNtlPLProperty* pData)
// {
// 	NTL_FUNCTION("CNtlPLSky::SetProperty");
// 
// 	NTL_RETURN(TRUE);
// }
// 
// void CNtlPLSky::HandleEvents(RWS::CMsg &pMsg)
// {
// 	if(pMsg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED)
// 	{
// 		RwBool			 NoVariation	= FALSE;
// 		sNTL_FIELD_PROP* pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
// 		m_InterpolationMode				= 0x00000000;
// 		m_RestTime4Change[0]			= -999.0f;
// 		m_RestTime4Change[1]			= -999.0f;
// 		m_RestTime4Change[2]			= -999.0f;
// 
// 		// not variate
// 		eNTL_WORLD_DIR NWD = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetWorldFieldMoveDir();
// 		if(NWD == eIDLE || NWD == ePORTAL)
// 		{
// 			NoVariation = TRUE;
// 		}
// 
// 		// decide interpolation of RGB color; dSKY_RGB_EFFECT_SWITCHING_TIME
// 		if(pNtlFieldProp->_BaseSkyMode == eBSM_TEX_MODE && GetBaseSkyMode() == eBSM_TEX_MODE)
// 		{
// 			if(	strcmp(pNtlFieldProp->_BaseSkyTexName, m_pBaseSkyLayer[1]->_pTexture->name) ||
// 				strcmp(pNtlFieldProp->_BlendedTexName[0], m_pBlendedSkyLayer[0]->_pTexture->name) ||
// 				strcmp(pNtlFieldProp->_BlendedTexName[1], m_pBlendedSkyLayer[1]->_pTexture->name))
// 			{
// 				m_InterpolationMode |= dSKY_INTERPOLATION_TEX_2_TEX;
// 				m_pBaseSkyLayer[0]->_pRender = FALSE;
// 				m_pBaseSkyLayer[1]->_pRender = TRUE;
// 
// 				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BaseSkyTexName, "texture/ntlwe/sky/");
// 
// 				SetSkyLayerTextrue(m_pBaseSkyLayer[1], pTexture);
// 				SetSkyLayerAlpha(m_pBaseSkyLayer[1], 255);
// 
// 				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
// 			}
// 		}
// 
// 		if(pNtlFieldProp->_BaseSkyMode == eBSM_TEX_MODE && GetBaseSkyMode() == eBSM_RGB_MODE)
// 		{
// 			m_InterpolationMode |= dSKY_INTERPOLATION_TEX_2_RGB;
// 			m_pBaseSkyLayer[0]->_pRender = FALSE;
// 			m_pBaseSkyLayer[1]->_pRender = TRUE;
// 
// 			RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BaseSkyTexName, "texture/ntlwe/sky/");
// 
// 			SetSkyLayerTextrue(m_pBaseSkyLayer[1], pTexture);
// 			SetSkyLayerAlpha(m_pBaseSkyLayer[1], 255);
// 
// 			RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
// 		}
// 
// 		if(pNtlFieldProp->_BaseSkyMode == eBSM_RGB_MODE && GetBaseSkyMode() == eBSM_TEX_MODE)
// 		{
// 			m_InterpolationMode |= dSKY_INTERPOLATION_RGB_2_TEX;
// 			m_pBaseSkyLayer[0]->_pRender = TRUE;
// 			m_pBaseSkyLayer[1]->_pRender = FALSE;
// 
// 			SetSkyLayerDiffuseColor(m_pBaseSkyLayer[0], pNtlFieldProp->_RGBSkyColor);
// 			SetSkyLayerAlpha(m_pBaseSkyLayer[0], 255);
// 		}
// 
// 		if(pNtlFieldProp->_BaseSkyMode == eBSM_RGB_MODE && GetBaseSkyMode() == eBSM_RGB_MODE)
// 		{
// 			if(	pNtlFieldProp->_RGBSkyColor[0].red != m_pBaseSkyLayer[0]->_RGBA[0].red ||
// 				pNtlFieldProp->_RGBSkyColor[0].green != m_pBaseSkyLayer[0]->_RGBA[0].green ||
// 				pNtlFieldProp->_RGBSkyColor[0].blue != m_pBaseSkyLayer[0]->_RGBA[0].blue ||
// 				pNtlFieldProp->_RGBSkyColor[1].red != m_pBaseSkyLayer[0]->_RGBA[1].red ||
// 				pNtlFieldProp->_RGBSkyColor[1].green != m_pBaseSkyLayer[0]->_RGBA[1].green ||
// 				pNtlFieldProp->_RGBSkyColor[1].blue != m_pBaseSkyLayer[0]->_RGBA[1].blue ||
// 				pNtlFieldProp->_RGBSkyColor[2].red != m_pBaseSkyLayer[0]->_RGBA[2].red ||
// 				pNtlFieldProp->_RGBSkyColor[2].green != m_pBaseSkyLayer[0]->_RGBA[2].green ||
// 				pNtlFieldProp->_RGBSkyColor[2].blue != m_pBaseSkyLayer[0]->_RGBA[2].blue)
// 			{
// 				m_InterpolationMode	   |= dSKY_INTERPOLATION_RGB_2_RGB;
// 
// 				if(NoVariation)
// 				{
// 					m_RestTime4Change[0] = RwRealMINVAL;
// 				}
// 				else
// 				{
// 					m_RestTime4Change[0] = dSKY_RGB_EFFECT_SWITCHING_TIME;
// 				}
// 
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[0], &m_pBaseSkyLayer[0]->_RGBA[0], sizeof(RwRGBA));
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[1], &m_pBaseSkyLayer[0]->_RGBA[1], sizeof(RwRGBA));
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBAFrom[2], &m_pBaseSkyLayer[0]->_RGBA[2], sizeof(RwRGBA));
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBATo[0], &pNtlFieldProp->_RGBSkyColor[0], sizeof(RwRGBA));
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBATo[1], &pNtlFieldProp->_RGBSkyColor[1], sizeof(RwRGBA));
// 				memcpy(&m_pBaseSkyLayer[0]->_RGBATo[2], &pNtlFieldProp->_RGBSkyColor[2], sizeof(RwRGBA));
// 			}
// 		}
// 
// 		// decide interpolation of opacity; dSKY_OPA_EFFECT_SWITCHING_TIME
// 		if(!strcmp(pNtlFieldProp->_BlendedTexName[0], "null"))
// 		{
// 			m_pBlendedTmpLayer[0]->_pRender = FALSE;
// 
// 			if(m_pBlendedSkyLayer[0]->_pRender)
// 			{
// 				if(strcmp(m_pBlendedSkyLayer[0]->_pTexture->name, pNtlFieldProp->_BlendedTexName[0]))
// 				{
// 					m_InterpolationMode |= dSKY_INTERPOLATION_OPA_PASS1;
// 
// 					if(NoVariation)
// 					{
// 						m_RestTime4Change[1] = RwRealMINVAL;
// 					}
// 					else
// 					{
// 						m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 					}
// 
// 					SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 					m_pBlendedSkyLayer[0]->_AutoDelete = TRUE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			// compared to the current one
// 			bool SkipTest = FALSE;
// 			if(m_pBlendedSkyLayer[0]->_pRender)
// 			{
// 				if(!strcmp(pNtlFieldProp->_BlendedTexName[0], m_pBlendedSkyLayer[0]->_pTexture->name))
// 				{
// 					SkipTest = TRUE;
// 				}
// 			}
// 
// 			if(!SkipTest)
// 			{
// 				m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS1;
// 
// 				if(NoVariation)
// 				{
// 					m_RestTime4Change[1] = RwRealMINVAL;
// 				}
// 				else
// 				{
// 					m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 				}
// 
// 				m_pBlendedTmpLayer[0]->_pRender = TRUE;
// 
// 				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[0], "texture/ntlwe/sky/");
// 
// 				SetSkyLayerTextrue(m_pBlendedTmpLayer[0], pTexture);
// 				SetSkyLayerAlpha(m_pBlendedTmpLayer[0], 255);
// 
// 				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
// 
// 				if(m_pBlendedSkyLayer[0]->_pRender)
// 				{
// 					if(strcmp(m_pBlendedSkyLayer[0]->_pTexture->name, pNtlFieldProp->_BlendedTexName[0]))
// 					{
// 						m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS1;
// 
// 						if(NoVariation)
// 						{
// 							m_RestTime4Change[1] = RwRealMINVAL;
// 						}
// 						else
// 						{
// 							m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 						}
// 
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 						m_pBlendedSkyLayer[0]->_AutoDelete	= TRUE;
// 						m_pBlendedSkyLayer[0]->_pRender		= TRUE;
// 					}
// 				}
// 			}
// 		}
// 
// 		if(!strcmp(pNtlFieldProp->_BlendedTexName[1], "null"))
// 		{
// 			m_pBlendedTmpLayer[1]->_pRender = FALSE;
// 
// 			if(m_pBlendedSkyLayer[1]->_pRender)
// 			{
// 				if(strcmp(m_pBlendedSkyLayer[1]->_pTexture->name, pNtlFieldProp->_BlendedTexName[1]))
// 				{
// 					m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;
// 					m_RestTime4Change[1]	= dSKY_OPA_EFFECT_SWITCHING_TIME;
// 
// 					if(NoVariation)
// 					{
// 						m_RestTime4Change[1] = RwRealMINVAL;
// 					}
// 					else
// 					{
// 						m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 					}
// 
// 					SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 					m_pBlendedSkyLayer[1]->_AutoDelete = TRUE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			// compared to the current one
// 			bool SkipTest = FALSE;
// 			if(m_pBlendedSkyLayer[1]->_pRender)
// 			{
// 				if(!strcmp(pNtlFieldProp->_BlendedTexName[1], m_pBlendedSkyLayer[1]->_pTexture->name))
// 				{
// 					SkipTest = TRUE;
// 				}
// 			}
// 
// 			if(!SkipTest)
// 			{
// 				m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;
// 
// 				if(NoVariation)
// 				{
// 					m_RestTime4Change[1] = RwRealMINVAL;
// 				}
// 				else
// 				{
// 					m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 				}
// 
// 				m_pBlendedTmpLayer[1]->_pRender = TRUE;
// 
// 				RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlFieldProp->_BlendedTexName[1], "texture/ntlwe/sky/");
// 
// 				SetSkyLayerTextrue(m_pBlendedTmpLayer[1], pTexture);
// 				SetSkyLayerAlpha(m_pBlendedTmpLayer[1], 255);
// 
// 				RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
// 
// 				if(m_pBlendedSkyLayer[1]->_pRender)
// 				{
// 					if(strcmp(m_pBlendedSkyLayer[1]->_pTexture->name, pNtlFieldProp->_BlendedTexName[1]))
// 					{
// 						m_InterpolationMode	   |= dSKY_INTERPOLATION_OPA_PASS2;
// 
// 						if(NoVariation)
// 						{
// 							m_RestTime4Change[1] = RwRealMINVAL;
// 						}
// 						else
// 						{
// 							m_RestTime4Change[1] = dSKY_OPA_EFFECT_SWITCHING_TIME;
// 						}
// 
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 						m_pBlendedSkyLayer[1]->_AutoDelete	= TRUE;
// 						m_pBlendedSkyLayer[1]->_pRender		= TRUE;
// 					}
// 				}
// 			}
// 		}
// 
// 		// decide interpolation of speed; dSKY_ROT_EFFECT_SWITCHING_TIME
// 		if(m_pBaseSkyLayer[1]->_RotSpeed != pNtlFieldProp->_BaseSkySpeed)
// 		{
// 			m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS0;
// 
// 			if(NoVariation)
// 			{
// 				m_RestTime4Change[2] = RwRealMINVAL;
// 			}
// 			else
// 			{
// 				m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
// 			}
// 
// 			m_pBaseSkyLayer[1]->_RotSpeedFrom	= m_pBaseSkyLayer[1]->_RotSpeedMPS;	
// 			m_pBaseSkyLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BaseSkySpeed;
// 		}
// 
// 		if(m_pBlendedSkyLayer[0]->_RotSpeed != pNtlFieldProp->_BlendedTexSpeed[0])
// 		{
// 			m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS1;
// 
// 			if(NoVariation)
// 			{
// 				m_RestTime4Change[2] = RwRealMINVAL;
// 			}
// 			else
// 			{
// 				m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
// 			}
// 
// 			m_pBlendedSkyLayer[0]->_RotSpeedFrom	= m_pBlendedSkyLayer[0]->_RotSpeedMPS;
// 			m_pBlendedSkyLayer[0]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[0];
// 			m_pBlendedTmpLayer[0]->_RotSpeedFrom	= m_pBlendedSkyLayer[0]->_RotSpeedMPS;
// 			m_pBlendedTmpLayer[0]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[0];	
// 		}
// 
// 		if(m_pBlendedSkyLayer[1]->_RotSpeed != pNtlFieldProp->_BlendedTexSpeed[1])
// 		{
// 			m_InterpolationMode	   |= dSKY_INTERPOLATION_ANG_PASS2;
// 
// 			if(NoVariation)
// 			{
// 				m_RestTime4Change[2] = RwRealMINVAL;
// 			}
// 			else
// 			{
// 				m_RestTime4Change[2] = dSKY_ROT_EFFECT_SWITCHING_TIME;
// 			}
// 
// 			m_pBlendedSkyLayer[1]->_RotSpeedFrom	= m_pBlendedSkyLayer[1]->_RotSpeedMPS;
// 			m_pBlendedSkyLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[1];
// 			m_pBlendedTmpLayer[1]->_RotSpeedFrom	= m_pBlendedSkyLayer[1]->_RotSpeedMPS;
// 			m_pBlendedTmpLayer[1]->_RotSpeedTo		= pNtlFieldProp->_BlendedTexSpeed[1];	
// 		}
// 	}
// }
// 
// void CNtlPLSky::UpdateFieldVariation(RwReal fElapsed)
// {
// 	if(m_RestTime4Change[0] >= 0.0f)
// 	{
// 		m_RestTime4Change[0] -= g_GetElapsedTime();
// 		if(m_RestTime4Change[0] < 0)
// 		{
// 			m_RestTime4Change[0]	= -999.0f;
// 			memcpy(&m_pBaseSkyLayer[0]->_RGBA[0], &m_pBaseSkyLayer[0]->_RGBATo[0], sizeof(RwRGBA));
// 			memcpy(&m_pBaseSkyLayer[0]->_RGBA[1], &m_pBaseSkyLayer[0]->_RGBATo[1], sizeof(RwRGBA));
// 			memcpy(&m_pBaseSkyLayer[0]->_RGBA[2], &m_pBaseSkyLayer[0]->_RGBATo[2], sizeof(RwRGBA));
// 
// 			SetSkyLayerDiffuseColor(m_pBaseSkyLayer[0], m_pBaseSkyLayer[0]->_RGBA);
// 			SetSkyLayerAlpha(m_pBaseSkyLayer[0], 255);
// 		}
// 		else
// 		{
// 			for(int i = 0; i < 3; ++i)
// 			{
// 				m_pBaseSkyLayer[0]->_RGBA[i].red	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].red, m_pBaseSkyLayer[0]->_RGBATo[i].red, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
// 				m_pBaseSkyLayer[0]->_RGBA[i].green	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].green, m_pBaseSkyLayer[0]->_RGBATo[i].green, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
// 				m_pBaseSkyLayer[0]->_RGBA[i].blue	= RpNtlWorldUInt8PropLerp(m_pBaseSkyLayer[0]->_RGBAFrom[i].blue, m_pBaseSkyLayer[0]->_RGBATo[i].blue, m_RestTime4Change[0], dSKY_RGB_EFFECT_SWITCHING_TIME);
// 			}
// 
// 			SetSkyLayerDiffuseColor(m_pBaseSkyLayer[0], m_pBaseSkyLayer[0]->_RGBA);
// 			SetSkyLayerAlpha(m_pBaseSkyLayer[0], 255);
// 		}
// 	}
// 
// 	if(m_RestTime4Change[1] >= 0.0f)
// 	{
// 		m_RestTime4Change[1] -= g_GetElapsedTime();
// 
// 		if(m_RestTime4Change[1] < 0)
// 		{
// 			m_RestTime4Change[1] = -999.0f;
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS1)
// 			{
// 				if(m_pBlendedSkyLayer[0]->_pRender)
// 				{
// 					if(m_pBlendedSkyLayer[0]->_AutoDelete)
// 					{
// 						m_pBlendedSkyLayer[0]->_pRender = FALSE;
// 					}
// 					else
// 					{
// 						SetSkyLayerTextrue(m_pBlendedSkyLayer[0], m_pBlendedTmpLayer[0]->_pTexture);
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 						m_pBlendedSkyLayer[0]->_pRender = TRUE;
// 					}		
// 				}
// 
// 				if(m_pBlendedTmpLayer[0]->_pRender)
// 				{
// 					SetSkyLayerTextrue(m_pBlendedSkyLayer[0], m_pBlendedTmpLayer[0]->_pTexture);
// 					SetSkyLayerAlpha(m_pBlendedSkyLayer[0], 255);
// 					m_pBlendedSkyLayer[0]->_pRender = TRUE;
// 
// 					SetSkyLayerTextrue(m_pBlendedTmpLayer[0], NULL);
// 					m_pBlendedTmpLayer[0]->_pRender = FALSE;
// 				}
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS2)
// 			{
// 				if(m_pBlendedSkyLayer[1]->_pRender)
// 				{
// 					if(m_pBlendedSkyLayer[1]->_AutoDelete)
// 					{
// 						m_pBlendedSkyLayer[1]->_pRender = FALSE;
// 					}
// 					else
// 					{
// 						SetSkyLayerTextrue(m_pBlendedSkyLayer[1], m_pBlendedTmpLayer[1]->_pTexture);
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[1], 255);
// 						m_pBlendedSkyLayer[1]->_pRender = TRUE;
// 					}		
// 				}
// 
// 				if(m_pBlendedTmpLayer[1]->_pRender)
// 				{
// 					SetSkyLayerTextrue(m_pBlendedSkyLayer[1], m_pBlendedTmpLayer[1]->_pTexture);
// 					SetSkyLayerAlpha(m_pBlendedSkyLayer[1], 255);
// 					m_pBlendedSkyLayer[1]->_pRender = TRUE;
// 
// 					SetSkyLayerTextrue(m_pBlendedTmpLayer[1], NULL);
// 					m_pBlendedTmpLayer[1]->_pRender = FALSE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS1)
// 			{
// 				if(m_pBlendedSkyLayer[0]->_pRender)
// 				{
// 					if(m_pBlendedSkyLayer[0]->_AutoDelete)
// 					{
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[0], RpNtlWorldUInt8PropLerp(255, 0, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 					}
// 					else
// 					{
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[0], RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 					}
// 				}
// 
// 				if(m_pBlendedTmpLayer[0]->_pRender)
// 				{
// 					SetSkyLayerAlpha(m_pBlendedTmpLayer[0], RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 				}
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_OPA_PASS2)
// 			{
// 				if(m_pBlendedSkyLayer[1]->_pRender)
// 				{
// 					if(m_pBlendedSkyLayer[1]->_AutoDelete)
// 					{
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[1], RpNtlWorldUInt8PropLerp(255, 0, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 					}
// 					else
// 					{
// 						SetSkyLayerAlpha(m_pBlendedSkyLayer[1], RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 					}
// 				}
// 
// 				if(m_pBlendedTmpLayer[1]->_pRender)
// 				{
// 					SetSkyLayerAlpha(m_pBlendedTmpLayer[1], RpNtlWorldUInt8PropLerp(0, 255, m_RestTime4Change[1], dSKY_OPA_EFFECT_SWITCHING_TIME));
// 				}
// 			}
// 		}
// 	}
// 
// 	if(m_RestTime4Change[2] >= 0.0f)
// 	{
// 		m_RestTime4Change[2] -= g_GetElapsedTime();
// 
// 		if(m_RestTime4Change[2] < 0)
// 		{
// 			m_RestTime4Change[2] = -999.0f;
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS0)
// 			{
// 				m_pBaseSkyLayer[1]->_RotSpeedMPS = m_pBaseSkyLayer[1]->_RotSpeedTo;
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS1)
// 			{
// 				m_pBlendedSkyLayer[0]->_RotSpeedMPS = m_pBlendedSkyLayer[0]->_RotSpeedTo;
// 				m_pBlendedTmpLayer[0]->_RotSpeedMPS = m_pBlendedTmpLayer[0]->_RotSpeedTo;
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS2)
// 			{
// 				m_pBlendedSkyLayer[1]->_RotSpeedMPS = m_pBlendedSkyLayer[1]->_RotSpeedTo;
// 				m_pBlendedTmpLayer[1]->_RotSpeedMPS = m_pBlendedTmpLayer[1]->_RotSpeedTo;
// 			}
// 		}
// 		else
// 		{
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS0)
// 			{
// 				m_pBaseSkyLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBaseSkyLayer[1]->_RotSpeedFrom, m_pBaseSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS1)
// 			{
// 				m_pBlendedSkyLayer[0]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[0]->_RotSpeedFrom, m_pBlendedSkyLayer[0]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
// 				m_pBlendedTmpLayer[0]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[0]->_RotSpeedFrom, m_pBlendedSkyLayer[0]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
// 			}
// 
// 			if(m_InterpolationMode & dSKY_INTERPOLATION_ANG_PASS2)
// 			{
// 				m_pBlendedSkyLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[1]->_RotSpeedFrom, m_pBlendedSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
// 				m_pBlendedTmpLayer[1]->_RotSpeedMPS = RpNtlWorldRealPropLerp(m_pBlendedSkyLayer[1]->_RotSpeedFrom, m_pBlendedSkyLayer[1]->_RotSpeedTo, m_RestTime4Change[2], dSKY_ROT_EFFECT_SWITCHING_TIME);
// 			}
// 		}
// 	}
// }
// 
// /*RpAtomic* CNtlPLSky::GetAtomic()
// { 
// // _pRender flag isn't always updated
// //m_pBlendedSkyLayer[1]->Rotate(m_pBlendedSkyLayer[1]->_RotSpeedMPS);
// 
// API_PL_TranslationAtomic(m_pBaseSkyLayer[1]->_pAtom, m_WorldDatumPos, rwCOMBINEREPLACE);
// 
// return m_pBaseSkyLayer[1]->_pAtom;
// }*/
// 
// ENTITY_ATOMIC_VEC CNtlPLSky::GetAtomicList()
// {
// 	return m_pBaseSkyLayer[1]->_vecAtomic;
// }
// 
// RwBool CNtlPLSky::Update(RwReal fElapsed)
// {
// 	if (dKEY(DIK_LSHIFT))
// 	{
// 		if (dOKEY(DIK_INSERT))
// 		{
// 			bRenderSky[6] = !bRenderSky[6];
// 		}
// 		else if (dOKEY(DIK_HOME))
// 		{
// 			bRenderSky[7] = !bRenderSky[7];
// 		}
// 		else if (dOKEY(DIK_PGUP))
// 		{
// 			bRenderSky[8] = !bRenderSky[8];
// 		}
// 		else if (dOKEY(DIK_DELETE))
// 		{
// 			bRenderSky[9] = !bRenderSky[9];
// 		}
// 		else if (dOKEY(DIK_END))
// 		{
// 			bRenderSky[10] = !bRenderSky[10];
// 		}
// 		else if (dOKEY(DIK_PGDN))
// 		{
// 			bRenderSky[11] = !bRenderSky[11];
// 		}
// 	}
// 	else if (dKEY(DIK_RSHIFT))
// 	{
// 		if (dOKEY(DIK_INSERT))
// 		{
// 			bRenderSky[12] = !bRenderSky[12];
// 		}
// 		else if (dOKEY(DIK_HOME))
// 		{
// 			bRenderSky[13] = !bRenderSky[13];
// 		}
// 		else if (dOKEY(DIK_PGUP))
// 		{
// 			bRenderSky[14] = !bRenderSky[14];
// 		}
// 		else if (dOKEY(DIK_DELETE))
// 		{
// 			bRenderSky[15] = !bRenderSky[15];
// 		}
// 		else if (dOKEY(DIK_END))
// 		{
// 			for (int i = 0; i <= 15; ++i)
// 			{
// 				bRenderSky[i] = FALSE;
// 			}			
// 		}
// 		else if (dOKEY(DIK_PGDN))
// 		{
// 			for (int i = 0; i <= 15; ++i)
// 			{
// 				bRenderSky[i] = TRUE;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (dOKEY(DIK_INSERT))
// 		{
// 			bRenderSky[0] = !bRenderSky[0];
// 		}
// 		else if (dOKEY(DIK_HOME))
// 		{
// 			bRenderSky[1] = !bRenderSky[1];
// 		}
// 		else if (dOKEY(DIK_PGUP))
// 		{
// 			bRenderSky[2] = !bRenderSky[2];
// 		}
// 		else if (dOKEY(DIK_DELETE))
// 		{
// 			bRenderSky[3] = !bRenderSky[3];
// 		}
// 		else if (dOKEY(DIK_END))
// 		{
// 			bRenderSky[4] = !bRenderSky[4];
// 		}
// 		else if (dOKEY(DIK_PGDN))
// 		{
// 			bRenderSky[5] = !bRenderSky[5];
// 		}
// 	}	
// 
// 	if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE)
// 		return TRUE;
// 
// 	UpdateFieldVariation(fElapsed);
// 
// 	m_pBaseSkyLayer[1]->Rotate(m_pBaseSkyLayer[1]->_RotSpeedMPS);
// 	m_pBlendedSkyLayer[0]->Rotate(m_pBlendedSkyLayer[0]->_RotSpeedMPS);
// 	m_pBlendedSkyLayer[1]->Rotate(m_pBlendedSkyLayer[1]->_RotSpeedMPS);
// 	m_pBlendedTmpLayer[0]->Rotate(m_pBlendedTmpLayer[0]->_RotSpeedMPS);
// 	m_pBlendedTmpLayer[1]->Rotate(m_pBlendedTmpLayer[1]->_RotSpeedMPS);
// 
// 	SkyLayerRotation(m_pBaseSkyLayer[1], m_pBaseSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
// 	SkyLayerRotation(m_pBlendedSkyLayer[0], m_pBlendedSkyLayer[0]->_RotSpeed, rwCOMBINEREPLACE);
// 	SkyLayerRotation(m_pBlendedSkyLayer[1], m_pBlendedSkyLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
// 	SkyLayerRotation(m_pBlendedTmpLayer[0], m_pBlendedTmpLayer[0]->_RotSpeed, rwCOMBINEREPLACE);
// 	SkyLayerRotation(m_pBlendedTmpLayer[1], m_pBlendedTmpLayer[1]->_RotSpeed, rwCOMBINEREPLACE);
// 
// 	SkyLayerTranslation(m_pDummySkyLayer, &m_WorldDatumPos, rwCOMBINEREPLACE);
// 
// 	if(m_pBaseSkyLayer[0]->_pRender)
// 	{
// 		SkyLayerTranslation(m_pBaseSkyLayer[0], &m_WorldDatumPos, rwCOMBINEREPLACE);
// 	}
// 
// 	if(m_pBaseSkyLayer[1]->_pRender)
// 	{	
// 		SkyLayerTranslation(m_pBaseSkyLayer[1], &m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
// 	}
// 
// 	if(m_pBlendedSkyLayer[0]->_pRender)
// 	{
// 		SkyLayerTranslation(m_pBlendedSkyLayer[0], &m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
// 	}
// 
// 	if(m_pBlendedSkyLayer[1]->_pRender)
// 	{
// 		SkyLayerTranslation(m_pBlendedSkyLayer[1], &m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
// 	}
// 
// 	if(m_pBlendedTmpLayer[0]->_pRender)
// 	{
// 		SkyLayerTranslation(m_pBlendedTmpLayer[0], &m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
// 	}
// 
// 	if(m_pBlendedTmpLayer[1]->_pRender)
// 	{
// 		SkyLayerTranslation(m_pBlendedTmpLayer[1], &m_WorldDatumPos, rwCOMBINEPOSTCONCAT);
// 	}
// 
// 	return TRUE;
// }
// 
// #include "NtlProfiler.h"
// 
// void CNtlPLSky::SkyLayerRender(sSKY_LAYER* pSkyLayer, RwInt32 iTFactor)
// {
// 	if (pSkyLayer->_pRender)
// 	{
// 		NTL_SPROFILE("CNtlPLSky::SkyLayerRender");
// 		for (int i = 0; i < (int)pSkyLayer->_vecAtomic.size(); ++i)
// 		{
// 			if (!bRenderSky[i])
// 			{
// 				continue;
// 			}
// 			RpAtomic*	pAtomic		= pSkyLayer->_vecAtomic[i]->pData;
// 			RpMaterial*	pMaterial	= API_PL_AtomicGetMaterial(pAtomic);
// 			RwRGBA		RGBATmp, RGBABk;
// 
// 			memcpy(&RGBABk, RpMaterialGetColor(pMaterial), sizeof(RwRGBA));
// 			memcpy(&RGBATmp, RpMaterialGetColor(pMaterial), sizeof(RwRGBA));
// 
// 			::API_PL_8BColorClamp(&RGBATmp.red, iTFactor);
// 			::API_PL_8BColorClamp(&RGBATmp.green, iTFactor);
// 			::API_PL_8BColorClamp(&RGBATmp.blue, iTFactor);
// 
// 			RpMaterialSetColor(pMaterial, &RGBATmp);
// 			RpAtomicRender(pAtomic);
// 			RpMaterialSetColor(pMaterial, &RGBABk);
// 		}
// 		NTL_EPROFILE();
// 	}
// }
// 
// RwBool CNtlPLSky::Render(void)
// {
// 	NTL_SPROFILE("CNtlPLSky::Render")
// 
// 		if(CNtlPLGlobal::m_bWorldSkyVisible == FALSE)
// 		{
// 			NTL_RPROFILE(TRUE)
// 		}
// 
// 		if(!m_VisibilityFlag)
// 		{
// 			NTL_RPROFILE(TRUE)
// 		}
// 
// 		RwInt32 TFactor = -static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff);
// 
// 		BegBaseSkyRenderState();
// 		{
// 			SkyLayerRender(m_pBaseSkyLayer[0], TFactor);
// 
// 			SkyLayerRender(m_pBaseSkyLayer[1], TFactor);
// 
// 			// 구름과 SkyBox 사이에 해/달을 렌더링한다. (2007.08.06 by agebreak)
// 			CNtlPLVisualManager* pVisualManager = (CNtlPLVisualManager*)GetSceneManager();
// 			CNtlPLRenderGroup* pSunGroup = pVisualManager->FindInstanceGroup(PLENTITY_SUN);
// 			if(pSunGroup)
// 			{
// 				CNtlPLRenderGroup::MapEntity::iterator the;
// 				CNtlPLRenderGroup::MapEntity *pmapEntity = pSunGroup->GetEntities();
// 				for(the = pmapEntity->begin(); the != pmapEntity->end(); ++the)
// 				{
// 					CNtlPLSun* pSun = static_cast<CNtlPLSun*>((*the).second);
// 					if(pSun && pSun->GetVisibility())
// 						pSun->Render();
// 				}
// 			}
// 
// 			SkyLayerRender(m_pBlendedSkyLayer[0], TFactor);
// 
// 			SkyLayerRender(m_pBlendedSkyLayer[1], TFactor);
// 
// 			SkyLayerRender(m_pBlendedTmpLayer[0], TFactor);
// 
// 			SkyLayerRender(m_pBlendedTmpLayer[1], TFactor);
// 		}
// 		EndBaseSkyRenderState();
// 
// 		BegDummySkyRenderState();
// 		{
// 			SkyLayerRender(m_pDummySkyLayer, TFactor);
// 		}
// 		EndDummySkyRenderState();
// 
// 		// LIGHTNING
// 		if (m_fTimeLightning > m_fTimeFadeOut)
// 		{
// 			m_fTimeLightning -= g_GetElapsedTime();
// 			if (m_fTimeLightning >= 0.0f)
// 			{
// 				m_clrLightning.alpha = static_cast<RwUInt8>((1.0f - (m_fTimeLightning / m_fTimeFadeIn)) * 255.0f);
// 			}
// 			else if (m_fTimeLightning <= m_fTimeFadeOut)
// 			{
// 				m_clrLightning.alpha = 0;
// 			}
// 			else
// 			{
// 				m_clrLightning.alpha = static_cast<RwUInt8>((1.0f - (m_fTimeLightning / m_fTimeFadeOut)) * 255.0f);
// 			}
// 		}
// 
// 		if (m_clrLightning.alpha)
// 		{
// 			RwIm2DVertex	vIm2D[4];
// 			RwReal			fWidth	= static_cast<RwReal>(CNtlPLGlobal::m_RwCamera->frameBuffer->width);
// 			RwReal			fHeight	= static_cast<RwReal>(CNtlPLGlobal::m_RwCamera->frameBuffer->height);
// 
// 			RwIm2DVertexSetScreenX(&vIm2D[0], 0.0f);
// 			RwIm2DVertexSetScreenY(&vIm2D[0], 0.0f);
// 			RwIm2DVertexSetScreenX(&vIm2D[1], 0.0f);
// 			RwIm2DVertexSetScreenY(&vIm2D[1], fHeight);
// 			RwIm2DVertexSetScreenX(&vIm2D[2], fWidth);
// 			RwIm2DVertexSetScreenY(&vIm2D[2], 0.0f);
// 			RwIm2DVertexSetScreenX(&vIm2D[3], fWidth);
// 			RwIm2DVertexSetScreenY(&vIm2D[3], fHeight);
// 
// 			RwIm2DVertexSetIntRGBA(&vIm2D[0], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
// 			RwIm2DVertexSetIntRGBA(&vIm2D[1], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
// 			RwIm2DVertexSetIntRGBA(&vIm2D[2], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
// 			RwIm2DVertexSetIntRGBA(&vIm2D[3], m_clrLightning.red, m_clrLightning.green, m_clrLightning.blue, m_clrLightning.alpha);
// 
// 			BeginSkyLightningRenderState();
// 
// 			RwD3D9SetTexture(NULL, 0);
// 			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vIm2D, 4);
// 
// 			RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
// 
// 			EndSkyLightningRenderState();
// 		}
// 
// 		NTL_RPROFILE(TRUE)
// }
// 
// void CNtlPLSky::RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut)
// {
// 	m_fTimeFadeIn	= fTimeFadeIn;
// 	m_fTimeFadeOut	= -fTimeFadeOut;
// 	m_fTimeLightning = m_fTimeFadeIn * (1.0f - (static_cast<RwReal>(m_clrLightning.alpha) / 255.0f));
// }