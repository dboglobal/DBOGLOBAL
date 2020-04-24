#include "precomp_ntlpresentation.h"
#include "NtlPLShoreLine.h"
#include "ceventhandler.h"
#include "NtlPLEvent.h"
#include "NtlPLShoreLineProp.h"
#include "NtlDNController.h"
#include "NtlWorldDI.h"
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLRenderState.h"
#include "NtlProfiler.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlPLShoreLine::CNtlPLShoreLine(void)
{
	m_pShoreLineProp		= NULL;
	m_VertNum				= 0;
	m_pVBOld				= NULL;
	m_pVBNew				= NULL;
	m_ShoreLineType			= -999;
	m_pShoreLineContainer	= NULL;
	m_Visible				= TRUE;
	//m_PVSCulled				= FALSE;
	memset(m_ScriptKeyName, 0, 32);

	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_ALPHA);
	SetLayer(PLENTITY_LAYER_SHORELINE);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_SHORELINE);
}

CNtlPLShoreLine::~CNtlPLShoreLine(void)
{
}

RwBool CNtlPLShoreLine::SetProperty(const CNtlPLProperty* pProperty)
{
	m_pShoreLineProp = reinterpret_cast<CNtlPLShoreLineProp*>(const_cast<CNtlPLProperty*>(pProperty));

	return TRUE;;
}

RwBool CNtlPLShoreLine::Create(const SPLEntityCreateParam* pParam)
{
	m_SectorMidPos.x = pParam->pPos->x;
	m_SectorMidPos.y = pParam->pPos->y;
	m_SectorMidPos.z = pParam->pPos->z;

	return TRUE;
}

void CNtlPLShoreLine::Destroy() 
{
	NTL_ARRAY_DELETE(m_pVBOld);
	NTL_ARRAY_DELETE(m_pVBNew);
}

void CNtlPLShoreLine::HandleEvents(RWS::CMsg &pMsg)
{
}

// VOID CNtlPLShoreLine::Save(FILE* _pFile)
// {
// 	if(!m_VertNum || !m_pVBNew)
// 	{
// 		return;
// 	}
// 
// 	fwrite(&m_VertNum, sizeof(RwInt32), 1, _pFile);
// 	fwrite(&m_ShoreLineType, sizeof(RwInt32), 1, _pFile);
// 	fwrite(m_ScriptKeyName, sizeof(RwChar) * 32, 1, _pFile);
// 	fwrite(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, _pFile);
// }
// 
// VOID CNtlPLShoreLine::Load(FILE* _pFile)
// {
// 	fread(&m_VertNum, sizeof(RwInt32), 1, _pFile);
// 	fread(&m_ShoreLineType, sizeof(RwInt32), 1, _pFile);
// 	fread(m_ScriptKeyName, sizeof(RwChar) * 32, 1, _pFile);
// 
// 	m_pVBNew = NTL_NEW RwIm3DVertex[m_VertNum];
// 	fread(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, _pFile);        
// 
// 	m_pShoreLineContainer = m_pShoreLineProp->GetContainer(m_ScriptKeyName);
// 	
// 	DBO_ASSERT(m_pShoreLineContainer, "Property find failed." << "(Name:" << m_ScriptKeyName << ")");
// }

RwBool CNtlPLShoreLine::Update(RwReal fElapsed)
{
	// position for each
	RwV3d Datum;

	if (!GetSceneManager()->GetWorld()->GetPlayerPosition(Datum))
	{
		return FALSE;
	}

	// radius cull
	RwV3d	CurPos			= m_SectorMidPos;
	RwReal	Culldist		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize + dGET_WORLD_PARAM()->WorldSectorSize + dGET_WORLD_PARAM()->WorldSectorSize);
	RwReal	SquaredDatum	= Culldist * Culldist;
	RwReal	SquaredDist;

	Datum.y			= 0.0f;
	CurPos.y		= 0.0f;
	SquaredDist		= CNtlMath::MathRwV3dSquaredLength(&(CurPos - Datum));

	if(SquaredDist > SquaredDatum)
	{
		m_Visible = FALSE;
	}
	else
	{
		m_Visible = TRUE;
	}

	// day & night
#ifdef dNTL_WORLD_TOOL_MODE
	if(dKEY(DIK_MINUS) || dKEY(DIK_EQUALS))
#else
	if(m_pVBNew && CNtlDNController::GetInstance()->IsDNEffectApplied())
#endif
	{
		RwRGBA Clr = {255, 255, 255, 255};
		Clr.alpha	= 255;
		Clr.red		-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
		Clr.green	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
		Clr.blue	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);

		for(RwInt32 i = 0; i < m_VertNum; ++i)
		{
			RwUInt8 Alpha = (RwUInt8)(m_pVBNew[i].color >> 24) & 0xFF;

			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
		}
	}

	return TRUE;
}

RwBool CNtlPLShoreLine::Render(void)
{
	NTL_SPROFILE("CNtlPLShoreLine::Render")

		if (!IsVisible())
		{
			NTL_RPROFILE(TRUE)
		}

		if(m_pVBNew && m_Visible)
		{
			BeginLightState(FALSE);

			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			if(m_ShoreLineType == 1)
			{
				RwD3D9SetTexture(static_cast<CUVSingleShoreLineContainer*>(m_pShoreLineContainer)->m_pTex, 0);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVSingleShoreLineContainer*>(m_pShoreLineContainer)->m_mTex);

				if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
					RwIm3DEnd();
				}
			}
			else if(m_ShoreLineType == 2)
			{
				RwD3D9SetTexture(static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_pTex[0], 0);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_mTex[0]);

				if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
					RwIm3DEnd();
				}

				RwD3D9SetTexture(static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_pTex[1], 0);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_mTex[1]);

				if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
					RwIm3DEnd();
				}
			}
			else if(m_ShoreLineType == 3)
			{
				RwD3D9SetTexture(static_cast<CSequenceShoreLineContainer*>(m_pShoreLineContainer)->m_ppTexPack[static_cast<CSequenceShoreLineContainer*>(m_pShoreLineContainer)->m_FrameIdx], 0);

				if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
					RwIm3DEnd();
				}
			}

			// restore
			D3DXMATRIXA16 mIdentity;
			D3DXMatrixIdentity(&mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

			EndLightState();
		}

		NTL_RPROFILE(TRUE)
}

VOID CNtlPLShoreLine::Delete(RwInt32 _IdxNum)
{
	if(m_pVBNew)
	{		
		RwInt32			nVNBefore	= 0;
		RwInt32			nVNAfter	= 0;
		RwInt32			nIDBefore	= 0;
		RwInt32			nIDAfter	= 0;

		// VB index
		nIDBefore = 0;

		if(m_VertNum != 6)
		{
			nIDAfter = _IdxNum + 6;
		}

		if(!nIDBefore && !nIDAfter)
		{
			NTL_ARRAY_DELETE(m_pVBNew);

			m_VertNum				= 0;
			m_ShoreLineType			= -999;
			memset(m_ScriptKeyName, 0, 32);

			return;
		}

		// VB size
		nVNBefore	= _IdxNum;
		nVNAfter	= (m_VertNum - nVNBefore - 6);

		// sub VB
		RwIm3DVertex*	pVBBefore	= NULL;
		RwIm3DVertex*	pVBAfter	= NULL;
		if(nVNBefore)	pVBBefore	= NTL_NEW RwIm3DVertex [nVNBefore];
		if(nVNAfter)	pVBAfter	= NTL_NEW RwIm3DVertex [nVNAfter];
		if(nVNBefore)	CopyMemory(pVBBefore, &m_pVBNew[nIDBefore], sizeof(RwIm3DVertex) * nVNBefore);
		if(nVNAfter)	CopyMemory(pVBAfter, &m_pVBNew[nIDAfter], sizeof(RwIm3DVertex) * nVNAfter);

		// main
		NTL_ARRAY_DELETE(m_pVBNew);

		m_pVBNew				= NTL_NEW RwIm3DVertex [nVNBefore + nVNAfter];
		RwIm3DVertex* pVBCur	= m_pVBNew;
		m_VertNum				= nVNBefore + nVNAfter;

		if(nVNBefore)	CopyMemory(pVBCur, pVBBefore, sizeof(RwIm3DVertex) * nVNBefore);	pVBCur += nVNBefore;
		if(nVNAfter)	CopyMemory(pVBCur, pVBAfter, sizeof(RwIm3DVertex) * nVNAfter);

		NTL_ARRAY_DELETE(pVBBefore);
		NTL_ARRAY_DELETE(pVBAfter);
	}
}

RwBool CNtlPLShoreLine::Insert(RwIm3DVertex* pIm3DVertex, RwInt32 _Size, RwChar* pScriptKeyName)
{
	// create
	if(!m_pVBNew)
	{
		m_pVBNew	= NTL_NEW RwIm3DVertex [_Size];
		m_VertNum	= _Size;

		// script contents
		strcpy_s(m_ScriptKeyName, 32, pScriptKeyName);
		m_ShoreLineType			= static_cast<RwInt32>(m_pShoreLineProp->GetCurType());
		m_pShoreLineContainer	= m_pShoreLineProp->GetContainer(m_ScriptKeyName);

		CopyMemory(m_pVBNew, pIm3DVertex, sizeof(RwIm3DVertex) * _Size);
	}
	// insert
	else
	{
		// compare script contents
		if(strcmp(m_ScriptKeyName, pScriptKeyName))
		{
			DBO_TRACE(FALSE, "different shoreline mesh, script key name.");
			return FALSE;
		}

		if(m_ShoreLineType != static_cast<RwInt32>(m_pShoreLineProp->GetCurType()))
		{
			DBO_TRACE(FALSE, "different shoreline mesh, type.");
			return FALSE;
		}

		// combine mesh
		m_pVBOld = NTL_NEW RwIm3DVertex [m_VertNum];
		CopyMemory(m_pVBOld, m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum);

		NTL_ARRAY_DELETE(m_pVBNew);
		m_pVBNew = NTL_NEW RwIm3DVertex [m_VertNum + _Size];

		RwIm3DVertex* pVBNew = m_pVBNew;
		CopyMemory(pVBNew, m_pVBOld, sizeof(RwIm3DVertex) * m_VertNum);

		pVBNew += m_VertNum;
		CopyMemory(pVBNew, pIm3DVertex, sizeof(RwIm3DVertex) * _Size);

		m_VertNum += _Size;

		NTL_ARRAY_DELETE(m_pVBOld);
	}

	return TRUE;
}

RwBool CNtlPLShoreLine::IsValidShoreline()
{
	if(!m_VertNum || !m_pVBNew)
	{
		return FALSE;
	}
	return TRUE;
}

RwBool CNtlPLShoreLine::LoadFromFile(FILE* pFile)
{
	fread(&m_VertNum, sizeof(RwInt32), 1, pFile);
	fread(&m_ShoreLineType, sizeof(RwInt32), 1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(m_ScriptKeyName, sizeof(RwChar) * uiLength, 1, pFile);
		m_ScriptKeyName[uiLength] = '\0';
	}

	m_pVBNew = NTL_NEW RwIm3DVertex[m_VertNum];
	fread(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, pFile);        

	m_pShoreLineContainer = m_pShoreLineProp->GetContainer(m_ScriptKeyName);

	DBO_ASSERT(m_pShoreLineContainer, "Property find failed." << "(Name:" << m_ScriptKeyName << ")");

	return TRUE;
}

RwBool CNtlPLShoreLine::SaveIntoFile(FILE* pFile)
{
	fwrite(&m_VertNum, sizeof(RwInt32), 1, pFile);
	fwrite(&m_ShoreLineType, sizeof(RwInt32), 1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(m_ScriptKeyName, sizeof(RwChar) * 32, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(m_ScriptKeyName);
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(m_ScriptKeyName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	fwrite(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, pFile);

	return TRUE;
}

RwInt32 CNtlPLShoreLine::SkipToFile(FILE* pFile)
{
	RwInt32 VertNum;
	::fread(&VertNum, sizeof(RwInt32), 1, pFile);
	::fseek(pFile, sizeof(RwInt32), SEEK_CUR);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		::fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
	}

	::fseek(pFile, sizeof(RwIm3DVertex) * VertNum, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlPLShoreLine::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwInt32 VertNum;

	CopyMemory(&VertNum, pFileMem, sizeof(RwInt32));
	fwrite(&VertNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// script type
	fwrite(pFileMem, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// script name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(pFileMem, (sizeof(RwChar) * 32), 1, pFile);
		pFileMem += (sizeof(RwChar) * 32);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength);
	}

	for(RwInt32 i = 0; i < VertNum; ++i)
	{
		RwIm3DVertex im3DVertex;
		CopyMemory(&im3DVertex, pFileMem, sizeof(RwIm3DVertex));
		pFileMem += sizeof(RwIm3DVertex);
		if (pvOffset)
		{
			im3DVertex.objVertex.x += pvOffset->x;
			im3DVertex.objVertex.y += pvOffset->y;
			im3DVertex.objVertex.z += pvOffset->z;
		}
		::fwrite(&im3DVertex, sizeof(RwIm3DVertex), 1, pFile);
	}
	return pFileMem;
}

BYTE* CNtlPLShoreLine::SkipToFileMem(BYTE* pFileMem)
{
	RwInt32 VertNum;

	CopyMemory(&VertNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	// script type
	pFileMem += sizeof(RwInt32);

	// script name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += (sizeof(RwChar) * 32);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		pFileMem += (sizeof(RwChar) * uiLength);
	}

	for(RwInt32 i = 0; i < VertNum; ++i)
	{
		pFileMem += sizeof(RwIm3DVertex);
	}
	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlPLShoreLine::CNtlPLShoreLine(void)
{
	m_pShoreLineProp		= NULL;
	m_VertNum				= 0;
	m_pVBOld				= NULL;
	m_pVBNew				= NULL;
	m_ShoreLineType			= -999;
	m_pShoreLineContainer	= NULL;
	m_Visible				= TRUE;
	//m_PVSCulled				= FALSE;
	memset(m_ScriptKeyName, 0, 32);

	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_ALPHA);
	SetLayer(PLENTITY_LAYER_SHORELINE);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_SHORELINE);
}

CNtlPLShoreLine::~CNtlPLShoreLine(void)
{
}

RwBool CNtlPLShoreLine::SetProperty(const CNtlPLProperty* pProperty)
{
	m_pShoreLineProp = reinterpret_cast<CNtlPLShoreLineProp*>(const_cast<CNtlPLProperty*>(pProperty));

	return TRUE;;
}

RwBool CNtlPLShoreLine::Create(const SPLEntityCreateParam* pParam)
{
	m_SectorMidPos.x = pParam->pPos->x;
	m_SectorMidPos.y = pParam->pPos->y;
	m_SectorMidPos.z = pParam->pPos->z;

	return TRUE;
}

void CNtlPLShoreLine::Destroy() 
{
	NTL_ARRAY_DELETE(m_pVBOld);
	NTL_ARRAY_DELETE(m_pVBNew);
}

void CNtlPLShoreLine::HandleEvents(RWS::CMsg &pMsg)
{
}

// VOID CNtlPLShoreLine::Save(FILE* _pFile)
// {
// 	if(!m_VertNum || !m_pVBNew)
// 	{
// 		return;
// 	}
// 
// 	fwrite(&m_VertNum, sizeof(RwInt32), 1, _pFile);
// 	fwrite(&m_ShoreLineType, sizeof(RwInt32), 1, _pFile);
// 	fwrite(m_ScriptKeyName, sizeof(RwChar) * 32, 1, _pFile);
// 	fwrite(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, _pFile);
// }
// 
// VOID CNtlPLShoreLine::Load(FILE* _pFile)
// {
// 	fread(&m_VertNum, sizeof(RwInt32), 1, _pFile);
// 	fread(&m_ShoreLineType, sizeof(RwInt32), 1, _pFile);
// 	fread(m_ScriptKeyName, sizeof(RwChar) * 32, 1, _pFile);
// 
// 	m_pVBNew = NTL_NEW RwIm3DVertex[m_VertNum];
// 	fread(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, _pFile);        
// 
// 	m_pShoreLineContainer = m_pShoreLineProp->GetContainer(m_ScriptKeyName);
// 	
// 	DBO_ASSERT(m_pShoreLineContainer, "Property find failed." << "(Name:" << m_ScriptKeyName << ")");
// }

RwBool CNtlPLShoreLine::Update(RwReal fElapsed)
{
	// position for each
	RwV3d Datum;

	if (!GetSceneManager()->GetWorld()->GetPlayerPosition(Datum))
	{
		return FALSE;
	}

	// radius cull
	RwV3d	CurPos			= m_SectorMidPos;
	RwReal	Culldist		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize + dGET_WORLD_PARAM()->WorldSectorSize + dGET_WORLD_PARAM()->WorldSectorSize);
	RwReal	SquaredDatum	= Culldist * Culldist;
	RwReal	SquaredDist;

	Datum.y			= 0.0f;
	CurPos.y		= 0.0f;
	SquaredDist		= CNtlMath::MathRwV3dSquaredLength(&(CurPos - Datum));

	if(SquaredDist > SquaredDatum)
	{
		m_Visible = FALSE;
	}
	else
	{
		m_Visible = TRUE;
	}

	// day & night
#ifdef dNTL_WORLD_TOOL_MODE
	if(dKEY(DIK_MINUS) || dKEY(DIK_EQUALS))
#else
	if(m_pVBNew && CNtlDNController::GetInstance()->IsDNEffectApplied())
#endif
	{
		RwRGBA Clr = {255, 255, 255, 255};
		Clr.alpha	= 255;
		Clr.red		-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
		Clr.green	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
		Clr.blue	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);

		for(RwInt32 i = 0; i < m_VertNum; ++i)
		{
			RwUInt8 Alpha = (RwUInt8)(m_pVBNew[i].color >> 24) & 0xFF;

			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
			RwIm3DVertexSetRGBA(&m_pVBNew[i], Clr.red, Clr.green, Clr.blue, Alpha);
		}
	}

	return TRUE;
}

RwBool CNtlPLShoreLine::Render(void)
{
	NTL_SPROFILE("CNtlPLShoreLine::Render")

	if (!IsVisible())
	{
		NTL_RPROFILE(TRUE)
	}

	if(m_pVBNew && m_Visible)
	{
		BeginLightState(FALSE);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

		RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

		if(m_ShoreLineType == 1)
		{
			RwD3D9SetTexture(static_cast<CUVSingleShoreLineContainer*>(m_pShoreLineContainer)->m_pTex, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVSingleShoreLineContainer*>(m_pShoreLineContainer)->m_mTex);

			if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}
		}
		else if(m_ShoreLineType == 2)
		{
			RwD3D9SetTexture(static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_pTex[0], 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_mTex[0]);
			
			if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}

			RwD3D9SetTexture(static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_pTex[1], 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &static_cast<CUVDualShoreLineContainer*>(m_pShoreLineContainer)->m_mTex[1]);

			if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}
		}
		else if(m_ShoreLineType == 3)
		{
			RwD3D9SetTexture(static_cast<CSequenceShoreLineContainer*>(m_pShoreLineContainer)->m_ppTexPack[static_cast<CSequenceShoreLineContainer*>(m_pShoreLineContainer)->m_FrameIdx], 0);

			if(RwIm3DTransform(m_pVBNew, m_VertNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}
		}

		// restore
		D3DXMATRIXA16 mIdentity;
		D3DXMatrixIdentity(&mIdentity);
		RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		EndLightState();
	}

	NTL_RPROFILE(TRUE)
}

VOID CNtlPLShoreLine::Delete(RwInt32 _IdxNum)
{
	if(m_pVBNew)
	{		
		RwInt32			nVNBefore	= 0;
		RwInt32			nVNAfter	= 0;
		RwInt32			nIDBefore	= 0;
		RwInt32			nIDAfter	= 0;

		// VB index
		nIDBefore = 0;

		if(m_VertNum != 6)
		{
			nIDAfter = _IdxNum + 6;
		}

		if(!nIDBefore && !nIDAfter)
		{
			NTL_ARRAY_DELETE(m_pVBNew);

			m_VertNum				= 0;
			m_ShoreLineType			= -999;
			memset(m_ScriptKeyName, 0, 32);

			return;
		}

		// VB size
		nVNBefore	= _IdxNum;
		nVNAfter	= (m_VertNum - nVNBefore - 6);

		// sub VB
        RwIm3DVertex*	pVBBefore	= NULL;
        RwIm3DVertex*	pVBAfter	= NULL;
		if(nVNBefore)	pVBBefore	= NTL_NEW RwIm3DVertex [nVNBefore];
		if(nVNAfter)	pVBAfter	= NTL_NEW RwIm3DVertex [nVNAfter];
		if(nVNBefore)	CopyMemory(pVBBefore, &m_pVBNew[nIDBefore], sizeof(RwIm3DVertex) * nVNBefore);
		if(nVNAfter)	CopyMemory(pVBAfter, &m_pVBNew[nIDAfter], sizeof(RwIm3DVertex) * nVNAfter);
		
		// main
		NTL_ARRAY_DELETE(m_pVBNew);

		m_pVBNew				= NTL_NEW RwIm3DVertex [nVNBefore + nVNAfter];
		RwIm3DVertex* pVBCur	= m_pVBNew;
		m_VertNum				= nVNBefore + nVNAfter;

		if(nVNBefore)	CopyMemory(pVBCur, pVBBefore, sizeof(RwIm3DVertex) * nVNBefore);	pVBCur += nVNBefore;
		if(nVNAfter)	CopyMemory(pVBCur, pVBAfter, sizeof(RwIm3DVertex) * nVNAfter);

		NTL_ARRAY_DELETE(pVBBefore);
		NTL_ARRAY_DELETE(pVBAfter);
	}
}

RwBool CNtlPLShoreLine::Insert(RwIm3DVertex* pIm3DVertex, RwInt32 _Size, RwChar* pScriptKeyName)
{
	// create
	if(!m_pVBNew)
	{
		m_pVBNew	= NTL_NEW RwIm3DVertex [_Size];
		m_VertNum	= _Size;

		// script contents
		strcpy_s(m_ScriptKeyName, 32, pScriptKeyName);
		m_ShoreLineType			= static_cast<RwInt32>(m_pShoreLineProp->GetCurType());
		m_pShoreLineContainer	= m_pShoreLineProp->GetContainer(m_ScriptKeyName);

		CopyMemory(m_pVBNew, pIm3DVertex, sizeof(RwIm3DVertex) * _Size);
	}
	// insert
	else
	{
		// compare script contents
		if(strcmp(m_ScriptKeyName, pScriptKeyName))
		{
			DBO_TRACE(FALSE, "different shoreline mesh, script key name.");
			return FALSE;
		}

		if(m_ShoreLineType != static_cast<RwInt32>(m_pShoreLineProp->GetCurType()))
		{
			DBO_TRACE(FALSE, "different shoreline mesh, type.");
			return FALSE;
		}

		// combine mesh
		m_pVBOld = NTL_NEW RwIm3DVertex [m_VertNum];
		CopyMemory(m_pVBOld, m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum);

		NTL_ARRAY_DELETE(m_pVBNew);
		m_pVBNew = NTL_NEW RwIm3DVertex [m_VertNum + _Size];

		RwIm3DVertex* pVBNew = m_pVBNew;
		CopyMemory(pVBNew, m_pVBOld, sizeof(RwIm3DVertex) * m_VertNum);

		pVBNew += m_VertNum;
		CopyMemory(pVBNew, pIm3DVertex, sizeof(RwIm3DVertex) * _Size);

		m_VertNum += _Size;

		NTL_ARRAY_DELETE(m_pVBOld);
	}

	return TRUE;
}

RwBool CNtlPLShoreLine::IsValidShoreline()
{
	if(!m_VertNum || !m_pVBNew)
	{
		return FALSE;
	}
	return TRUE;
}

RwBool CNtlPLShoreLine::LoadFromFile(FILE* pFile)
{
	fread(&m_VertNum, sizeof(RwInt32), 1, pFile);
	fread(&m_ShoreLineType, sizeof(RwInt32), 1, pFile);
	fread(m_ScriptKeyName, sizeof(RwChar) * 32, 1, pFile);

	m_pVBNew = NTL_NEW RwIm3DVertex[m_VertNum];
	fread(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, pFile);        

	m_pShoreLineContainer = m_pShoreLineProp->GetContainer(m_ScriptKeyName);

	DBO_ASSERT(m_pShoreLineContainer, "Property find failed." << "(Name:" << m_ScriptKeyName << ")");

	return TRUE;
}

RwBool CNtlPLShoreLine::SaveIntoFile(FILE* pFile)
{
	fwrite(&m_VertNum, sizeof(RwInt32), 1, pFile);
	fwrite(&m_ShoreLineType, sizeof(RwInt32), 1, pFile);
	fwrite(m_ScriptKeyName, sizeof(RwChar) * 32, 1, pFile);
	fwrite(m_pVBNew, sizeof(RwIm3DVertex) * m_VertNum, 1, pFile);

	return TRUE;
}

RwInt32 CNtlPLShoreLine::SkipToFile(FILE* pFile)
{
	RwInt32 VertNum;
	::fread(&VertNum, sizeof(RwInt32), 1, pFile);
	::fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	::fseek(pFile, sizeof(RwChar) * 32, SEEK_CUR);
	::fseek(pFile, sizeof(RwIm3DVertex) * VertNum, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlPLShoreLine::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwInt32 VertNum;

	CopyMemory(&VertNum, pFileMem, sizeof(RwInt32));
	fwrite(&VertNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// script type
	fwrite(pFileMem, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// script name
	fwrite(pFileMem, (sizeof(RwChar) * 32), 1, pFile);
	pFileMem += (sizeof(RwChar) * 32);

	for(RwInt32 i = 0; i < VertNum; ++i)
	{
		RwIm3DVertex im3DVertex;
		CopyMemory(&im3DVertex, pFileMem, sizeof(RwIm3DVertex));
		pFileMem += sizeof(RwIm3DVertex);
		if (pvOffset)
		{
			im3DVertex.objVertex.x += pvOffset->x;
			im3DVertex.objVertex.y += pvOffset->y;
			im3DVertex.objVertex.z += pvOffset->z;
		}
		::fwrite(&im3DVertex, sizeof(RwIm3DVertex), 1, pFile);
	}
	return pFileMem;
}

BYTE* CNtlPLShoreLine::SkipToFileMem(BYTE* pFileMem)
{
	RwInt32 VertNum;

	CopyMemory(&VertNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	// script type
	pFileMem += sizeof(RwInt32);

	// script name
	pFileMem += (sizeof(RwChar) * 32);

	for(RwInt32 i = 0; i < VertNum; ++i)
	{
		pFileMem += sizeof(RwIm3DVertex);
	}
	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif