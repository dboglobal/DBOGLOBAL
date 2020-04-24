#include "StdAfx.h"
#include "ntlwe.h"
#include "NtlPLWorldEntity.h"
#include "MergeUIManager.h"
#include "NtlPLResourceManager.h"
#include "NtlPLRenderState.h"
#include "NtlWorldMouse.h"
#include "NtlWorldDI.h"
#include "NtlWorldFieldManager.h"
#include "FieldUIManager.h"
#include "NtlText.h"
#include "NtlLog.h"
#include "NtlWorldMergeManager.h"
#include "NtlWEFrm.h"
#include "PaletteFieldUI.h"
#include "PaletteTerrain.h"
#include "ProgressWnd.h"
#include "MiniMapManager.h"
#include "UpdateEvent.h"
#include "NtlWEApplication.h"
#include "ntlworldinterface.h"

#include "SpawnManager.h"

CMergeUIManager::CMergeUIManager(void)
{
	m_pTextureGrid = NULL;
	::ZeroMemory(&m_MergeUIMap, sizeof(sMERGE_UIMAP));
	::ZeroMemory(&m_MergeDrag, sizeof(sDRAG_INFO));
	::ZeroMemory(&m_CamInfo, sizeof(sCAM_INFO));
	::ZeroMemory(&m_FieldInfo, sizeof(sFIELD_INFO));

	m_MergedInfo._pMergedTex = NULL;
	m_MergedInfo._vecMergedField.clear();

	m_State = eFC_IDLE_MAP;
	m_Working = FALSE;
	m_Update = FALSE;
}

CMergeUIManager::~CMergeUIManager(void)
{
	Delete();
}

void CMergeUIManager::Delete()
{
	if(m_pTextureGrid)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pTextureGrid);
		//RwTextureDestroy(m_pTextureGrid);
		m_pTextureGrid = NULL;
	}

	if(m_MergeUIMap._pTextureField)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_MergeUIMap._pTextureField);
		//RwTextureDestroy(m_MergeUIMap._pTextureField);
		m_MergeUIMap._pTextureField = NULL;
	}

	if(m_MergeDrag._pTextureField)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_MergeDrag._pTextureField);
		//RwTextureDestroy(m_MergeDrag._pTextureField);
		m_MergeDrag._pTextureField = NULL;
	}

	if(m_CamInfo._pTextureCamera)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_CamInfo._pTextureCamera);
		//RwTextureDestroy(m_CamInfo._pTextureCamera);
		m_CamInfo._pTextureCamera = NULL;
	}

	if(m_MergedInfo._pMergedTex)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_MergedInfo._pMergedTex);
		//RwTextureDestroy(m_MergedInfo._pMergedTex);
		m_MergedInfo._pMergedTex = NULL;
	}
	m_MergedInfo._vecMergedField.clear();
}

VOID CMergeUIManager::UpdateMergeHistory(RwChar *fmt, ... )
{
	// read only
	DWORD dwAttrs = ::GetFileAttributes("SABER.mrg");
	if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
	{ 
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes("SABER.mrg", dwAttrs); 
	} 

	// access file
	FILE *fp = NULL;
	if(fopen_s(&fp, "SABER.mrg", "a+") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << "SABER.mrg" << ")");
		return;
	}	

	CNtlLog cLog;
	//RwChar *pHeaderLog = cLog.GetDay();
	RwChar *pHeaderLog = cLog.GetCalendar();
	if(pHeaderLog)
	{
		fprintf(fp, pHeaderLog);

		RwChar	ComputerName[64] = {0,};
		DWORD	Size = 64;
		GetComputerName(ComputerName, &Size);

		fprintf(fp, "[%s] user merged ", ComputerName);
	}
	
	va_list args;
	va_start(args, fmt);
	vfprintf_s(fp, fmt, args);
	va_end(args);
	fprintf(fp, "\n");

	fclose(fp);
}


void CMergeUIManager::Create()
{
	if(dGET_WORLD_PARAM()->WorldFieldNum == 8)
	{
		m_FieldGridNum	= 8;
		m_FieldGridSize	= 64;
	}
	else
	{
		m_FieldGridNum	= 16;
		m_FieldGridSize	= 32;
	}
	
	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewDir[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	::strcpy(szNewDir, "Tool\\SABER\\Texture\\UI\\");

	m_CamInfo._pTextureCamera = CNtlPLResourceManager::GetInstance()->LoadTexture("Camerafield", szNewDir);
	DBO_ASSERT(m_CamInfo._pTextureCamera, "Texture load failed.");

	if(dGET_WORLD_PARAM()->WorldFieldNum == 8)
	{
		m_pTextureGrid = CNtlPLResourceManager::GetInstance()->LoadTexture("Grid8x8", szNewDir);
	}
	else
	{
		m_pTextureGrid = CNtlPLResourceManager::GetInstance()->LoadTexture("Grid16x16", szNewDir);
	}
	DBO_ASSERT(m_pTextureGrid, "Texture load failed.");

	:: SetCurrentDirectory(szOldDir);

	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32 nStartX = 512;
	RwInt32 nStartY = 0;
	RwInt32 width	= m_FieldGridNum * m_FieldGridSize;
	RwInt32 height	= m_FieldGridNum * m_FieldGridSize;

	// GridPanel
	RwIm2DVertexSetScreenX(&m_GridPanel[0], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_GridPanel[0], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_GridPanel[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[0], recipCameraZ);
	RwIm2DVertexSetU(&m_GridPanel[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_GridPanel[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_GridPanel[0], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_GridPanel[1], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_GridPanel[1], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_GridPanel[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[1], recipCameraZ);
	RwIm2DVertexSetU(&m_GridPanel[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_GridPanel[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_GridPanel[1], 255, 255, 255, 100);

	RwIm2DVertexSetScreenX(&m_GridPanel[2], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_GridPanel[2], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_GridPanel[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[2], recipCameraZ);
	RwIm2DVertexSetU(&m_GridPanel[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_GridPanel[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_GridPanel[2], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_GridPanel[3], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_GridPanel[3], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_GridPanel[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[3], recipCameraZ);
	RwIm2DVertexSetU(&m_GridPanel[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_GridPanel[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_GridPanel[3], 255, 255, 255, 100);

	// FieldUIMap
	RwInt32 BlankFieldNum		= static_cast<RwInt32>((dGET_WORLD_PARAM()->WorldFieldNum - m_FieldGridNum) / 2);
	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
	RwInt32 VisibleFieldNum		= m_FieldGridNum;
	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);

	RwIm2DVertexSetScreenX(&m_MergeUIMap._Panel[0], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_MergeUIMap._Panel[0], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_MergeUIMap._Panel[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_MergeUIMap._Panel[0], recipCameraZ);
	RwIm2DVertexSetU(&m_MergeUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_MergeUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_MergeUIMap._Panel[0], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_MergeUIMap._Panel[1], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_MergeUIMap._Panel[1], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_MergeUIMap._Panel[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_MergeUIMap._Panel[1], recipCameraZ);
	RwIm2DVertexSetU(&m_MergeUIMap._Panel[1], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_MergeUIMap._Panel[1], 1.0f - BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_MergeUIMap._Panel[1], 255, 255, 255, 100);

	RwIm2DVertexSetScreenX(&m_MergeUIMap._Panel[2], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_MergeUIMap._Panel[2], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_MergeUIMap._Panel[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_MergeUIMap._Panel[2], recipCameraZ);
	RwIm2DVertexSetU(&m_MergeUIMap._Panel[2], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_MergeUIMap._Panel[2], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_MergeUIMap._Panel[2], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_MergeUIMap._Panel[3], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_MergeUIMap._Panel[3], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_MergeUIMap._Panel[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_MergeUIMap._Panel[3], recipCameraZ);
	RwIm2DVertexSetU(&m_MergeUIMap._Panel[3], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_MergeUIMap._Panel[3], 1.0f - BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_MergeUIMap._Panel[3], 255, 255, 255, 100);

	m_MergeUIMap._Collision.left	= 512;
	m_MergeUIMap._Collision.top		= 0;
	m_MergeUIMap._Collision.right	= 512 + m_FieldGridNum * m_FieldGridSize;
	m_MergeUIMap._Collision.bottom	= m_FieldGridNum * m_FieldGridSize;

	m_MergeUIMap._FieldUVTabSize = 1.0f / dGET_WORLD_PARAM()->WorldFieldNum;

	// merge info. display
	m_MergedInfo._pMergedTex	= CNtlPLResourceManager::GetInstance()->LoadTexture("MergeRect", "Tool\\SABER\\Texture\\UI\\");
	DBO_ASSERT(m_MergedInfo._pMergedTex, "Texture load failed.");

	m_MergedInfo._vecMergedField.reserve(16 * 16);
}

void CMergeUIManager::Restore()
{
}

void CMergeUIManager::LoadFieldTexture(const char* pName)
{
	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewDir[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	::SetCurrentDirectory(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::strcpy(szNewDir, "Tool/SABER/Texture/Grid Map/");

	if(m_MergeUIMap._pTextureField)
	{
		RwTextureDestroy(m_MergeUIMap._pTextureField);
	}

	m_MergeUIMap._pTextureField = CNtlPLResourceManager::GetInstance()->LoadTexture(pName, szNewDir);
	DBO_ASSERT(m_MergeUIMap._pTextureField, "Texture load failed.");

	:: SetCurrentDirectory(szOldDir);
}

RwBool CMergeUIManager::IsCollidedInCurFieldArea(RECT& rt)
{
	RECT CurFieldArea = GetCurFieldArea2World();

	if(	rt.left >= CurFieldArea.left &&
		rt.top >= CurFieldArea.top &&
		rt.right <= CurFieldArea.right &&
		rt.bottom <= CurFieldArea.bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

RwBool CMergeUIManager::IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY)
{
	RECT CurFieldArea = GetCurFieldArea2World();

	if(	nX <= CurFieldArea.left && 
		nX >= CurFieldArea.right &&
		nY <= CurFieldArea.top &&
		nY >= CurFieldArea.bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

RwBool CMergeUIManager::IsCollided(RwInt32 nX, RwInt32 nY)
{
	if(	nX >= m_MergeUIMap._Collision.left && 
		nX <= m_MergeUIMap._Collision.right &&
		nY >= m_MergeUIMap._Collision.top &&
		nY <= m_MergeUIMap._Collision.bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void CMergeUIManager::MoveFieldUIMap(eMERGEUI_DIR Dir)
{
	// 0(0, 0) 2(1, 0)
	// 1(0, 1) 3(1, 1)

	RwInt32 BlankFieldNum		= static_cast<RwInt32>((dGET_WORLD_PARAM()->WorldFieldNum - m_FieldGridNum) / 2);
	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
	RwInt32 VisibleFieldNum		= m_FieldGridNum;
	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
	RwReal	Temp[4];

	switch(Dir)
	{
	case eFD_N:
		{
			Temp[0] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[0]) - m_MergeUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[1]) - m_MergeUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[2]) - m_MergeUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[3]) - m_MergeUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetV(&m_MergeUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	case eFD_E:
		{
			Temp[0] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[0]) + m_MergeUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[1]) + m_MergeUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[2]) + m_MergeUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[3]) + m_MergeUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetU(&m_MergeUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	case eFD_S:
		{
			Temp[0] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[0]) + m_MergeUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[1]) + m_MergeUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[2]) + m_MergeUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetV(&m_MergeUIMap._Panel[3]) + m_MergeUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetV(&m_MergeUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[3], Temp[3], recipCameraZ);
		}
	    break;
	case eFD_W:
		{
			Temp[0] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[0]) - m_MergeUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[1]) - m_MergeUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[2]) - m_MergeUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetU(&m_MergeUIMap._Panel[3]) - m_MergeUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);
							
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[3], Temp[3], recipCameraZ);
		}
	    break;
	}
}

RwV3d CMergeUIManager::GetPortalPT()
{
	RwV3d RetPT;

	RetPT.x = m_MergeDrag._Rect.left + (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.z = m_MergeDrag._Rect.top	+ (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.y = 0.0f;

	return RetPT;
}

RwV3d CMergeUIManager::GetWorldPT(RwV3d& FieldUIMapPT)
{
	RwV3d RetPT;

	RwReal fXMove = FieldUIMapPT.x / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
	RwReal fYMove = FieldUIMapPT.z / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
	fXMove *= (dGET_WORLD_PARAM()->WorldFieldSize * m_FieldGridNum);
	fYMove *= (dGET_WORLD_PARAM()->WorldFieldSize * m_FieldGridNum);

	RECT CurWorldFieldAreaRT = GetCurFieldArea2World();
	RetPT.x = CurWorldFieldAreaRT.left - fXMove;
	RetPT.z = CurWorldFieldAreaRT.top - fYMove;
	RetPT.y = FieldUIMapPT.y;
	
	return RetPT;
}

RECT CMergeUIManager::GetCurFieldArea2World()
{
	// 0(0, 0) 2(1, 0)
	// 1(0, 1) 3(1, 1)

	RECT rt;
	rt.left		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetU(&m_MergeUIMap._Panel[0]) / m_MergeUIMap._FieldUVTabSize * dGET_WORLD_PARAM()->WorldFieldSize));
	rt.top		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetV(&m_MergeUIMap._Panel[0]) / m_MergeUIMap._FieldUVTabSize * dGET_WORLD_PARAM()->WorldFieldSize));
	rt.right	= rt.left - m_FieldGridNum * dGET_WORLD_PARAM()->WorldFieldSize;
	rt.bottom	= rt.top - m_FieldGridNum * dGET_WORLD_PARAM()->WorldFieldSize;

	return rt;
}

// This function will be called when the param. must be existed in fielduimap area; see IsCollidedInCurFieldArea
POINT CMergeUIManager::GetFieldSPosInFieldUIMap(RwReal x, RwReal z)
{
	POINT	pt;
	RECT	CurFieldArea = GetCurFieldArea2World();

	CurFieldArea.left	+= dGET_WORLD_PARAM()->WorldSizeHalf;
	CurFieldArea.top	+= dGET_WORLD_PARAM()->WorldSizeHalf;
	x					+= dGET_WORLD_PARAM()->WorldSizeHalf;
	z					+= dGET_WORLD_PARAM()->WorldSizeHalf;
	pt.x				= static_cast<LONG>(CurFieldArea.left - x);
	pt.y				= static_cast<LONG>(CurFieldArea.top - z);
	pt.x				/= dGET_WORLD_PARAM()->WorldFieldSize;
	pt.y				/= dGET_WORLD_PARAM()->WorldFieldSize;
	pt.x				*= m_FieldGridSize;
	pt.y				*= m_FieldGridSize;
	pt.x				+= 512;

	return pt;
}

void CMergeUIManager::UpdateRenderInfo()
{
	// The field where the current camera's being
	RwV3d CamPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	if(IsCollidedInCurFieldArea(static_cast<RwInt32>(CamPos.x), static_cast<RwInt32>(CamPos.z)))
	{
		POINT SPos = GetFieldSPosInFieldUIMap(CamPos.x, CamPos.z);

		RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

		RwIm2DVertexSetScreenX(&m_CamInfo._Line[0], SPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_CamInfo._Line[0], SPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[0], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[0], recipCameraZ);
		RwIm2DVertexSetU(&m_CamInfo._Line[0], 0, recipCameraZ);
		RwIm2DVertexSetV(&m_CamInfo._Line[0], 0, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[0], 255, 255, 0, 100);

		RwIm2DVertexSetScreenX(&m_CamInfo._Line[1], SPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_CamInfo._Line[1], SPos.y + m_FieldGridSize - 0.5f);
		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[1], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[1], recipCameraZ);
		RwIm2DVertexSetU(&m_CamInfo._Line[1], 0, recipCameraZ);
		RwIm2DVertexSetV(&m_CamInfo._Line[1], 1, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[1], 255, 255, 0, 100);

		RwIm2DVertexSetScreenX(&m_CamInfo._Line[2], SPos.x + m_FieldGridSize - 0.5f);
		RwIm2DVertexSetScreenY(&m_CamInfo._Line[2], SPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[2], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[2], recipCameraZ);
		RwIm2DVertexSetU(&m_CamInfo._Line[2], 1, recipCameraZ);
		RwIm2DVertexSetV(&m_CamInfo._Line[2], 0, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[2], 255, 255, 0, 100);

		RwIm2DVertexSetScreenX(&m_CamInfo._Line[3], SPos.x + m_FieldGridSize - 0.5f);
		RwIm2DVertexSetScreenY(&m_CamInfo._Line[3], SPos.y + m_FieldGridSize - 0.5f);
		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[3], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[3], recipCameraZ);
		RwIm2DVertexSetU(&m_CamInfo._Line[3], 1, recipCameraZ);
		RwIm2DVertexSetV(&m_CamInfo._Line[3], 1, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[3], 255, 255, 0, 100);

		m_CamInfo._bRender = TRUE;
	}
	else
	{
		m_CamInfo._bRender = FALSE;
	}

	// The fields loaded currently
	CNtlWorldFieldManager*	pFMgr = dGETMGR();
	RwInt32					FieldIndex[2] = {-999, -999};

	for(int i = 35; i >= 0; --i)
	{
		if(pFMgr->m_Fields6x6[1][i] != -1)
		{
			FieldIndex[0] = pFMgr->m_Fields6x6[1][i];
			break;
		}
/*
		if(i == 35)
		{
			// full fields were loaded
			FieldIndex[0] = pFMgr->m_Fields6x6[1][35];
			FieldIndex[1] = pFMgr->m_Fields6x6[1][0];
		}*/
	}

	if(FieldIndex[1] == -999)
	{
		for(int i = 0; i < 36; ++i)
		{
			if(pFMgr->m_Fields6x6[1][i] != -1)
			{
				FieldIndex[1] = pFMgr->m_Fields6x6[1][i];
				break;
			}
		}
	}

	CNtlWorldField* pFields = const_cast<CNtlWorldField*>(pFMgr->GetFields());
	RwV3d SFieldWorldSPos	= pFields[FieldIndex[0]].GetSPos();
	RwV3d EFieldWorldSPos	= pFields[FieldIndex[1]].GetSPos();

	// add WorldFieldSize 'cos of the drawing point
	SFieldWorldSPos.x		+= dGET_WORLD_PARAM()->WorldFieldSize;
	SFieldWorldSPos.z		+= dGET_WORLD_PARAM()->WorldFieldSize;

	SFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
	SFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;
	EFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
	EFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;

	RwV3d SFieldWorldSPosFinal;
	RwV3d EFieldWorldSPosFinal;

	// Find the start and the end point of a drawing area
	RwBool Collided[2] = {FALSE, FALSE};

	for(int j = static_cast<int>(EFieldWorldSPos.z); j <= static_cast<int>(SFieldWorldSPos.z); j += dGET_WORLD_PARAM()->WorldFieldSize)
	{
		for(int i = static_cast<int>(EFieldWorldSPos.x); i <= static_cast<int>(SFieldWorldSPos.x); i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			if(IsCollidedInCurFieldArea(i - dGET_WORLD_PARAM()->WorldSizeHalf, j - dGET_WORLD_PARAM()->WorldSizeHalf))
			{
				EFieldWorldSPosFinal.x = static_cast<RwReal>(i);
				EFieldWorldSPosFinal.z = static_cast<RwReal>(j);
				EFieldWorldSPosFinal.x -= dGET_WORLD_PARAM()->WorldSizeHalf;
				EFieldWorldSPosFinal.z -= dGET_WORLD_PARAM()->WorldSizeHalf;

				Collided[0] = TRUE;
				break;
			}
		}

		if(Collided[0])
			break;
	}

	if(Collided[0])
	{
		for(int j = static_cast<int>(SFieldWorldSPos.z); j >= static_cast<int>(EFieldWorldSPos.z); j -= dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(int i = static_cast<int>(SFieldWorldSPos.x); i >= static_cast<int>(EFieldWorldSPos.x); i -= dGET_WORLD_PARAM()->WorldFieldSize)
			{
				if(IsCollidedInCurFieldArea(i - dGET_WORLD_PARAM()->WorldSizeHalf, j - dGET_WORLD_PARAM()->WorldSizeHalf))
				{
					SFieldWorldSPosFinal.x = static_cast<RwReal>(i);
					SFieldWorldSPosFinal.z = static_cast<RwReal>(j);
					SFieldWorldSPosFinal.x -= dGET_WORLD_PARAM()->WorldSizeHalf;
					SFieldWorldSPosFinal.z -= dGET_WORLD_PARAM()->WorldSizeHalf;

					Collided[1] = TRUE;
					break;
				}
			}

			if(Collided[1])
				break;
		}
	}

	if(Collided[0] && Collided[1])
	{
		POINT SPos = GetFieldSPosInFieldUIMap(SFieldWorldSPosFinal.x, SFieldWorldSPosFinal.z);
		POINT EPos = GetFieldSPosInFieldUIMap(EFieldWorldSPosFinal.x, EFieldWorldSPosFinal.z);

		RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[0], SPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[0], SPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[0], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[0], recipCameraZ);
		RwIm2DVertexSetU(&m_FieldInfo._Line[0], 0, recipCameraZ);
		RwIm2DVertexSetV(&m_FieldInfo._Line[0], 0, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[0], 0, 0, 255, 100);

		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[1], SPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[1], EPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[1], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[1], recipCameraZ);
		RwIm2DVertexSetU(&m_FieldInfo._Line[1], 0, recipCameraZ);
		RwIm2DVertexSetV(&m_FieldInfo._Line[1], 1, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[1], 0, 0, 255, 100);

		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[2], EPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[2], SPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[2], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[2], recipCameraZ);
		RwIm2DVertexSetU(&m_FieldInfo._Line[2], 1, recipCameraZ);
		RwIm2DVertexSetV(&m_FieldInfo._Line[2], 0, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[2], 0, 0, 255, 100);

		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[3], EPos.x - 0.5f);
		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[3], EPos.y - 0.5f);
		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[3], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[3], recipCameraZ);
		RwIm2DVertexSetU(&m_FieldInfo._Line[3], 1, recipCameraZ);
		RwIm2DVertexSetV(&m_FieldInfo._Line[3], 1, recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[3], 0, 0, 255, 100);

		m_FieldInfo._bRender = TRUE;
	}
	else
	{
		m_FieldInfo._bRender = FALSE;
	}
}

RwBool CMergeUIManager::IsSelected()
{
	if(!m_MergeDrag._bRender)
		return FALSE;

	return TRUE;
}

RwBool CMergeUIManager::IsMergeEnable()
{
	if(!m_MergeDrag._bRender)
		return FALSE;

	return TRUE;
}

VOID CMergeUIManager::InsertMergedArea(RwV3d& FieldPos)
{
	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	RwInt32					MergedFieldIdx	= pMgr->GetFieldIdx(FieldPos);
	
	// exceptions
	if(MergedFieldIdx == -1)
	{
		DBO_TRACE(FALSE, "CMergeUIManager::InsertMergedArea, invalid field index.");
		return;
	}

	// same index already exists
	std::vector<RwInt32>::iterator iter;
	iter = find(m_MergedInfo._vecMergedField.begin(), m_MergedInfo._vecMergedField.end(), MergedFieldIdx);
	if(iter != m_MergedInfo._vecMergedField.end())
	{
		return;
	}

	m_MergedInfo._vecMergedField.push_back(MergedFieldIdx);
}

// New Merge
RwBool CMergeUIManager::UpdateMerge(RwV3d& SFieldDatumPos, RwV3d& EFieldDatumPos, RwV3d& SFieldDatumPos4CurWorld, RwV3d& EFieldDatumPos4CurWorld, RwBool Singular /* = TRUE */)
{
	CNtlWorldFieldManager*	pMgr = dGETMGR();
	RwInt32					MergedFieldIdx[2];
	RwInt32					MergedFieldIdx4World[2];
	string					ProjectName;
	
	// current merge menu used index
	RwInt32 IdxMergeMenu[7];
	IdxMergeMenu[0] = dGETFRM()->m_pFieldUI->m_M0.GetCheck();
	IdxMergeMenu[1] = dGETFRM()->m_pFieldUI->m_M1.GetCheck();
	IdxMergeMenu[2] = dGETFRM()->m_pFieldUI->m_M2.GetCheck();
	IdxMergeMenu[3] = dGETFRM()->m_pFieldUI->m_M3.GetCheck();
	IdxMergeMenu[4] = dGETFRM()->m_pFieldUI->m_M4.GetCheck();
	IdxMergeMenu[5] = dGETFRM()->m_pFieldUI->m_M5.GetCheck();
	IdxMergeMenu[6] = dGETFRM()->m_pFieldUI->m_M6.GetCheck();

	// get the folder names
	string ProjectNameTmp = dGET_WORLD_PARAM()->WorldProjectFolderName;
	CFileController::ExtractFolderNameWithLastBSRemoved(ProjectNameTmp, ProjectName);

	// counts of fields
	RwInt32 WidthGridNum		= abs(m_MergeDrag._Rect.left - m_MergeDrag._Rect.right) / m_FieldGridSize;
	RwInt32 HeightColGridNum	= abs(m_MergeDrag._Rect.top - m_MergeDrag._Rect.bottom) / m_FieldGridSize;
	RwInt32 NumCurFields2Merge	= m_MergedInfo._vecMergedField.size();

	if(Singular)
	{
		MergedFieldIdx[0] = pMgr->GetFieldIdx(SFieldDatumPos);
		MergedFieldIdx[1] = -1;
		MergedFieldIdx4World[0] = pMgr->GetFieldIdx(SFieldDatumPos4CurWorld);
		MergedFieldIdx4World[1] = -1;
		UpdateMergeHistory("[%s] at the field[%d] using merge menu[%d,%d,%d,%d,%d,%d,%d]", ProjectName.c_str(), MergedFieldIdx[0], IdxMergeMenu[0], IdxMergeMenu[1], IdxMergeMenu[2], IdxMergeMenu[3], IdxMergeMenu[4], IdxMergeMenu[5], IdxMergeMenu[6]);
	}
	else
	{
		MergedFieldIdx[0] = pMgr->GetFieldIdx(SFieldDatumPos);
		MergedFieldIdx[1] = pMgr->GetFieldIdx(EFieldDatumPos);
		MergedFieldIdx4World[0] = pMgr->GetFieldIdx(SFieldDatumPos4CurWorld);
		MergedFieldIdx4World[1] = pMgr->GetFieldIdx(EFieldDatumPos4CurWorld);
		UpdateMergeHistory("[%s] at the fields from [%d] to [%d] = (%d, %d) using merge menu[%d,%d,%d,%d,%d,%d,%d].", ProjectName.c_str(), MergedFieldIdx[0], MergedFieldIdx[1], WidthGridNum, HeightColGridNum, IdxMergeMenu[0], IdxMergeMenu[1], IdxMergeMenu[2], IdxMergeMenu[3], IdxMergeMenu[4], IdxMergeMenu[5], IdxMergeMenu[6]);
	}

	sMERGE_PARAM sMergeParam;
	if(dGETFRM()->m_pFieldUI->m_M0.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bMaterial			= TRUE;
		sMergeParam.bHeightField		= TRUE;
		sMergeParam.bDiffuse			= TRUE;		
		sMergeParam.bWater				= TRUE;		
		sMergeParam.bSectorCull			= TRUE;
		sMergeParam.bTileTransparency	= TRUE;
		sMergeParam.bShoreline			= TRUE;		
		sMergeParam.bPlant				= TRUE;
		sMergeParam.bWorldLight			= TRUE;		

		sMergeParam.bWorldMaterial		= TRUE;
		sMergeParam.bFieldColor			= TRUE;
	}
	if(dGETFRM()->m_pFieldUI->m_M1.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bDoodads			= TRUE;
		sMergeParam.bShadow				= TRUE;
		sMergeParam.bEffects			= TRUE;
		// CZ-SS
		//sMergeParam.bSS					= TRUE;
		sMergeParam.bDecal				= TRUE;
		sMergeParam.bOccluder			= TRUE;
		sMergeParam.bLightObject		= TRUE;
		sMergeParam.bDojo				= TRUE;

		sMergeParam.bRealLight			= TRUE;		
	}
	if(dGETFRM()->m_pFieldUI->m_M2.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bBitProp			= TRUE;
	}
	if(dGETFRM()->m_pFieldUI->m_M3.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bHeatHazeObject		= TRUE;

		sMergeParam.bFog				= TRUE;
		sMergeParam.bSky				= TRUE;
		sMergeParam.bPlanet				= TRUE;
		sMergeParam.bSpecular			= TRUE;
		sMergeParam.bWeather			= TRUE;
		sMergeParam.bFieldHeatHaze		= TRUE;
	}
	if(dGETFRM()->m_pFieldUI->m_M4.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bSE					= TRUE;

		sMergeParam.bBGM				= TRUE;
	}
	if(dGETFRM()->m_pFieldUI->m_M5.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bName				= TRUE;
	}
	if(dGETFRM()->m_pFieldUI->m_M6.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		sMergeParam.bBloom				= TRUE;
	}

	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY))
	{
		dGETHDL()->SetAnimatibleDoodadsDummyVision(FALSE);
	}

	//////////////////////////////////////////////////////////////////////////
	// Merge
	//////////////////////////////////////////////////////////////////////////
	// verify field indices
	if (CNtlWorldMergeManager::GetInstance()->IsFieldValid(MergedFieldIdx[0], MergedFieldIdx[1]))
	{
		RwInt32				NumFieldsOfWorldWidth = dGET_WORLD_PARAM()->WorldFieldNum;
		vector<RwInt32>		vecIdxFieldSrc, vecIdxFieldDst;

		// fill field index buffer for each
		for(RwInt32 i = 0; i < HeightColGridNum; ++i)
		{
			for(RwInt32 j = 0; j < WidthGridNum; ++j)
			{
				vecIdxFieldSrc.push_back(NumFieldsOfWorldWidth * i + MergedFieldIdx[0] + j);
			}
		}

		for(RwInt32 i = 0; i < HeightColGridNum; ++i)
		{
			for(RwInt32 j = 0; j < WidthGridNum; ++j)
			{
				vecIdxFieldDst.push_back(NumFieldsOfWorldWidth * i + MergedFieldIdx4World[0] + j);
			}
		}

		// verification
		if(vecIdxFieldSrc.size() == vecIdxFieldDst.size())
		{
			CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Merge");
			WndProgress.GoModal();
			WndProgress.SetRange(0, vecIdxFieldSrc.size());	

			for(RwUInt32 i = 0; i < vecIdxFieldSrc.size(); ++i)
			{
				RwChar cText[64];
				::sprintf_s(cText, 64, "%d / %d fields Merge", i, vecIdxFieldSrc.size());
				WndProgress.SetText(cText);
				CNtlWorldMergeManager::GetInstance()->Merge(sMergeParam, vecIdxFieldSrc[i], vecIdxFieldDst[i]);

				WndProgress.StepIt();
				WndProgress.PeekAndPump();
			}
			
// 			if (dGETFRM()->m_pFieldUI->m_MergeSpawn.GetCheck())
// 			{
// 				RwInt32 iIdxMin, iIdxMax;
// 				RwBBox bboxSrc, bboxDst;
// 
// 				iIdxMin = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; iIdxMax = 0;
// 				for (int i = 0; i < (int)vecIdxFieldSrc.size(); ++i)
// 				{
// 					if (iIdxMin > vecIdxFieldSrc.at(i))
// 					{
// 						iIdxMin = vecIdxFieldSrc.at(i);
// 					}
// 					if (iIdxMax < vecIdxFieldSrc.at(i))
// 					{
// 						iIdxMax = vecIdxFieldSrc.at(i);
// 					}
// 				}
// 
// 				bboxSrc.inf.x = static_cast<RwReal>(iIdxMin % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxSrc.inf.z = static_cast<RwReal>(iIdxMin / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxSrc.sup.x = static_cast<RwReal>(iIdxMax % dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxSrc.sup.z = static_cast<RwReal>(iIdxMax / dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 
// 				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(bboxSrc.inf, FALSE);
// 				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(bboxSrc.sup, FALSE);
// 
// 				iIdxMin = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; iIdxMax = 0;
// 				for (int i = 0; i < (int)vecIdxFieldDst.size(); ++i)
// 				{
// 					if (iIdxMin > vecIdxFieldDst.at(i))
// 					{
// 						iIdxMin = vecIdxFieldDst.at(i);
// 					}
// 					if (iIdxMax < vecIdxFieldDst.at(i))
// 					{
// 						iIdxMax = vecIdxFieldDst.at(i);
// 					}
// 				}
// 
// 				bboxDst.inf.x = static_cast<RwReal>(iIdxMin % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxDst.inf.z = static_cast<RwReal>(iIdxMin / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxDst.sup.x = static_cast<RwReal>(iIdxMax % dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 				bboxDst.sup.z = static_cast<RwReal>(iIdxMax / dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
// 
// 				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(bboxDst.inf, FALSE);
// 				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(bboxDst.sup, FALSE);
// 
// 				if (CNtlWorldMergeManager::GetInstance()->MergeSpawn(bboxDst, bboxSrc))
// 				{
// 					GetSpawnInstance()->LoadFromFile(dGETFRM()->m_pFieldUI->m_MergeSpawnDstName.GetBuffer());
// 				}				
// 			}

			// refresh field prop.
			dGETMGR()->SetAnotherField();

			WndProgress.SetText("Done.");
		}
		else
		{
			::AfxMessageBox("size of sorce and dest is not same.", MB_OK);
		}
	}
	else
	{
		::AfxMessageBox("invalid start field or end field", MB_OK);
	}

	// F4 palette should need to refresh
	if (dGETFRM()->m_pFieldUI->m_M0.GetCheck() || dGETFRM()->m_pFieldUI->m_MergeAll.GetCheck())
	{
		dGETFRM()->m_pTerrainPalette->UpdateFieldTextureInfo(pMgr->GetCurField());
	}
	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_ANIMATIBLE_DOODAD_DUMMY))
	{
		dGETHDL()->SetAnimatibleDoodadsDummyVision(TRUE);
	}

	Sleep(100);
	return TRUE;
}

void CMergeUIManager::Update(CFieldUIManager* _pFieldUIManager)
{    
	if(_pFieldUIManager)
	{
		if(_pFieldUIManager->m_State == eFC_MOVE_MAP)
		{
			// identity field ui uv with merge ui uv
			RwIm2DVertexSetU(&m_MergeUIMap._Panel[0], RwIm2DVertexGetU(&_pFieldUIManager->m_FieldUIMap._Panel[0]), recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[0], RwIm2DVertexGetV(&_pFieldUIManager->m_FieldUIMap._Panel[0]), recipCameraZ);

			RwIm2DVertexSetU(&m_MergeUIMap._Panel[1], RwIm2DVertexGetU(&_pFieldUIManager->m_FieldUIMap._Panel[1]), recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[1], RwIm2DVertexGetV(&_pFieldUIManager->m_FieldUIMap._Panel[1]), recipCameraZ);

			RwIm2DVertexSetU(&m_MergeUIMap._Panel[2], RwIm2DVertexGetU(&_pFieldUIManager->m_FieldUIMap._Panel[2]), recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[2], RwIm2DVertexGetV(&_pFieldUIManager->m_FieldUIMap._Panel[2]), recipCameraZ);

			RwIm2DVertexSetU(&m_MergeUIMap._Panel[3], RwIm2DVertexGetU(&_pFieldUIManager->m_FieldUIMap._Panel[3]), recipCameraZ);
			RwIm2DVertexSetV(&m_MergeUIMap._Panel[3], RwIm2DVertexGetV(&_pFieldUIManager->m_FieldUIMap._Panel[3]), recipCameraZ);
		}
	} 

	if(!m_Working)
		return;

	if(!m_MergeUIMap._pTextureField)
		return;

	// Update the camera field & the loaded fields
	UpdateRenderInfo();

	if(dOKEY(DIK_SPACE))
	{
		m_MergeDrag._bRender = FALSE;
	}

	if(!dGET_MOUSE()->GetBehavior(MB_LBDN) && !dGET_MOUSE()->GetLBUp())
	{
		return;
	}

	if(dGET_MOUSE()->GetBehavior(MB_RBDN))
	{
		return;
	}

	POINT Pt;
	Pt.x = dGET_MOUSE()->GetCusPos().x;
	Pt.y = dGET_MOUSE()->GetCusPos().y;

	// update protection flag
	if(dGET_MOUSE()->GetLBOnce())
	{
		if(IsCollided(Pt.x, Pt.y))
		{
			m_Update = TRUE;
		}
	}

	if(dGET_MOUSE()->GetLBUp())
	{
		m_State = eFC_IDLE_MAP;
		m_Update = FALSE;
	}

	if(!m_Update)
	{
		m_State = eFC_IDLE_MAP;
		return;
	}

	switch(m_State)
	{
	case eFC_IDLE_MAP:
		{
			if(!IsCollided(Pt.x, Pt.y))
			{
				break;
			}

			if(dKEY(DIK_LCONTROL))
			{
				m_State = eFC_MOVE_MAP;
			}
			else if(dGET_MOUSE()->GetBehavior(MB_LBDN))
			{
				m_MergeDrag._SPos = Pt;
				m_State = eFC_DRAG_MAP;
			}
		}
		break;

	case eFC_MOVE_MAP:
		{
			m_MergeDrag._bRender = FALSE;

			if(!dKEY(DIK_LCONTROL) || dGET_MOUSE()->GetLBUp())
			{
				m_State = eFC_IDLE_MAP;
				break;
			}
			else
			{
				RwReal TabSize		= 1.0f;
				RwReal XMovement	= dGET_MOUSE()->GetXMovementWithRate(1.0f);
				RwReal YMovement	= dGET_MOUSE()->GetYMovementWithRate(1.0f);

				if(XMovement > +TabSize)	MoveFieldUIMap(eFD_W);
				if(XMovement < -TabSize)	MoveFieldUIMap(eFD_E);
				if(YMovement > +TabSize)	MoveFieldUIMap(eFD_N);
				if(YMovement < -TabSize)	MoveFieldUIMap(eFD_S);
			}
		}
		break;

	case eFC_DRAG_MAP:
		{
			if(dGET_MOUSE()->GetLBUp())
			{
				m_State = eFC_IDLE_MAP;
				break;
			}
			else
			{
				m_MergeDrag._EPos.x = dGET_MOUSE()->GetCusPos().x;
				m_MergeDrag._EPos.y = dGET_MOUSE()->GetCusPos().y;

				if(m_MergeDrag._SPos.x < m_MergeDrag._EPos.x)
				{
					m_MergeDrag._Rect.left	= m_MergeDrag._SPos.x;
					m_MergeDrag._Rect.right	= m_MergeDrag._EPos.x;
				}
				else
				{
					m_MergeDrag._Rect.left	= m_MergeDrag._EPos.x;
					m_MergeDrag._Rect.right	= m_MergeDrag._SPos.x;
				}

				if(m_MergeDrag._SPos.y < m_MergeDrag._EPos.y)
				{
					m_MergeDrag._Rect.top		= m_MergeDrag._SPos.y;
					m_MergeDrag._Rect.bottom	= m_MergeDrag._EPos.y;
				}
				else
				{
					m_MergeDrag._Rect.top		= m_MergeDrag._EPos.y;
					m_MergeDrag._Rect.bottom	= m_MergeDrag._SPos.y;
				}

				m_MergeDrag._Rect.left		= m_MergeDrag._Rect.left   / m_FieldGridSize * m_FieldGridSize;
				m_MergeDrag._Rect.top		= m_MergeDrag._Rect.top	   / m_FieldGridSize * m_FieldGridSize;
				m_MergeDrag._Rect.right		= m_MergeDrag._Rect.right  / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
				m_MergeDrag._Rect.bottom	= m_MergeDrag._Rect.bottom / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
				CLAMP(m_MergeDrag._Rect.left, 512, m_FieldGridNum * m_FieldGridSize * 2);
				CLAMP(m_MergeDrag._Rect.top, 0, m_FieldGridNum * m_FieldGridSize);
				CLAMP(m_MergeDrag._Rect.right, 512, m_FieldGridNum * m_FieldGridSize * 2);
				CLAMP(m_MergeDrag._Rect.bottom, 0, m_FieldGridNum * m_FieldGridSize);

				// update geometry
				RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

				RwIm2DVertexSetScreenX(&m_MergeDrag._Line[0], m_MergeDrag._Rect.left - 0.5f);
				RwIm2DVertexSetScreenY(&m_MergeDrag._Line[0], m_MergeDrag._Rect.top - 0.5f);
				RwIm2DVertexSetScreenZ(&m_MergeDrag._Line[0], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_MergeDrag._Line[0], recipCameraZ);
				RwIm2DVertexSetU(&m_MergeDrag._Line[0], 0, recipCameraZ);
				RwIm2DVertexSetV(&m_MergeDrag._Line[0], 0, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_MergeDrag._Line[0], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_MergeDrag._Line[1], m_MergeDrag._Rect.left - 0.5f);
				RwIm2DVertexSetScreenY(&m_MergeDrag._Line[1], m_MergeDrag._Rect.bottom - 0.5f);
				RwIm2DVertexSetScreenZ(&m_MergeDrag._Line[1], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_MergeDrag._Line[1], recipCameraZ);
				RwIm2DVertexSetU(&m_MergeDrag._Line[1], 0, recipCameraZ);
				RwIm2DVertexSetV(&m_MergeDrag._Line[1], 1, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_MergeDrag._Line[1], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_MergeDrag._Line[2], m_MergeDrag._Rect.right - 0.5f);
				RwIm2DVertexSetScreenY(&m_MergeDrag._Line[2], m_MergeDrag._Rect.top - 0.5f);
				RwIm2DVertexSetScreenZ(&m_MergeDrag._Line[2], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_MergeDrag._Line[2], recipCameraZ);
				RwIm2DVertexSetU(&m_MergeDrag._Line[2], 1, recipCameraZ);
				RwIm2DVertexSetV(&m_MergeDrag._Line[2], 0, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_MergeDrag._Line[2], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_MergeDrag._Line[3], m_MergeDrag._Rect.right - 0.5f);
				RwIm2DVertexSetScreenY(&m_MergeDrag._Line[3], m_MergeDrag._Rect.bottom - 0.5f);
				RwIm2DVertexSetScreenZ(&m_MergeDrag._Line[3], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_MergeDrag._Line[3], recipCameraZ);
				RwIm2DVertexSetU(&m_MergeDrag._Line[3], 1, recipCameraZ);
				RwIm2DVertexSetV(&m_MergeDrag._Line[3], 1, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_MergeDrag._Line[3], 255, 0, 0, 100);

				m_MergeDrag._bRender = TRUE;
			}
		}
		break;
	}
}

RwBool CMergeUIManager::IsWorking()
{
	if(m_MergeUIMap._pTextureField && m_Working)// && m_FieldInfo._bRender) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

RwBool CMergeUIManager::IsSingleSelected()
{
	if(	m_MergeDrag._Rect.right - m_MergeDrag._Rect.left > m_FieldGridSize ||
		m_MergeDrag._Rect.bottom - m_MergeDrag._Rect.top > m_FieldGridSize)	
	{
		return FALSE;
	}

	return TRUE;
}

RECT CMergeUIManager::GetFieldDragInfo()
{
	return m_MergeDrag._Rect;
}

RwV3d CMergeUIManager::GetFieldMidPos()
{
	RwV3d RetPT;

	RetPT.x = (m_MergeDrag._Rect.left - 512) + (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.z = m_MergeDrag._Rect.top	+ (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.y = 0.0f;

	return RetPT;
}

void CMergeUIManager::RenderMergeArea()
{
	RwInt32 TotMergedIndicesNum				= m_MergedInfo._vecMergedField.size();
	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	CNtlWorldField*			pFields			= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwInt32					Cnt				= 0;
	RwReal					recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32					HalfGridSize	= m_FieldGridSize;
	POINT					SPos, EPos;
	RwV3d					vFieldSPos;
	

	RwD3D9SetTexture(m_MergedInfo._pMergedTex, 0);

	for(RwInt32 i = 0; i < TotMergedIndicesNum; ++i)
	{
		// culling
		vFieldSPos = pFields[m_MergedInfo._vecMergedField[i]].GetSPos();
		if(IsCollidedInCurFieldArea(static_cast<RwInt32>(vFieldSPos.x) + HalfGridSize, static_cast<RwInt32>(vFieldSPos.z) + HalfGridSize))
		{
			// unify buffering 4 fast rendering but non local
			EPos = GetFieldSPosInFieldUIMap(vFieldSPos.x, vFieldSPos.z);
			SPos = EPos;
			EPos.x -= m_FieldGridSize;
			EPos.y -= m_FieldGridSize;

			RwIm2DVertexSetScreenX(&m_MergedInfo._VertBuf[0], SPos.x - 0.5f);
			RwIm2DVertexSetScreenY(&m_MergedInfo._VertBuf[0], SPos.y - 0.5f);
			RwIm2DVertexSetScreenZ(&m_MergedInfo._VertBuf[0], RwIm2DGetNearScreenZ());
			RwIm2DVertexSetRecipCameraZ(&m_MergedInfo._VertBuf[0], recipCameraZ);
			RwIm2DVertexSetU(&m_MergedInfo._VertBuf[0], 0, recipCameraZ);
			RwIm2DVertexSetV(&m_MergedInfo._VertBuf[0], 0, recipCameraZ);
			RwIm2DVertexSetIntRGBA(&m_MergedInfo._VertBuf[0], 255, 255, 255, 255);

			RwIm2DVertexSetScreenX(&m_MergedInfo._VertBuf[1], SPos.x - 0.5f);
			RwIm2DVertexSetScreenY(&m_MergedInfo._VertBuf[1], EPos.y - 0.5f);
			RwIm2DVertexSetScreenZ(&m_MergedInfo._VertBuf[1], RwIm2DGetNearScreenZ());
			RwIm2DVertexSetRecipCameraZ(&m_MergedInfo._VertBuf[1], recipCameraZ);
			RwIm2DVertexSetU(&m_MergedInfo._VertBuf[1], 0, recipCameraZ);
			RwIm2DVertexSetV(&m_MergedInfo._VertBuf[1], 1, recipCameraZ);
			RwIm2DVertexSetIntRGBA(&m_MergedInfo._VertBuf[1], 255, 255, 255, 255);

			RwIm2DVertexSetScreenX(&m_MergedInfo._VertBuf[2], EPos.x - 0.5f);
			RwIm2DVertexSetScreenY(&m_MergedInfo._VertBuf[2], SPos.y - 0.5f);
			RwIm2DVertexSetScreenZ(&m_MergedInfo._VertBuf[2], RwIm2DGetNearScreenZ());
			RwIm2DVertexSetRecipCameraZ(&m_MergedInfo._VertBuf[2], recipCameraZ);
			RwIm2DVertexSetU(&m_MergedInfo._VertBuf[2], 1, recipCameraZ);
			RwIm2DVertexSetV(&m_MergedInfo._VertBuf[2], 0, recipCameraZ);
			RwIm2DVertexSetIntRGBA(&m_MergedInfo._VertBuf[2], 255, 255, 255, 255);

			RwIm2DVertexSetScreenX(&m_MergedInfo._VertBuf[3], EPos.x - 0.5f);
			RwIm2DVertexSetScreenY(&m_MergedInfo._VertBuf[3], EPos.y - 0.5f);
			RwIm2DVertexSetScreenZ(&m_MergedInfo._VertBuf[3], RwIm2DGetNearScreenZ());
			RwIm2DVertexSetRecipCameraZ(&m_MergedInfo._VertBuf[3], recipCameraZ);
			RwIm2DVertexSetU(&m_MergedInfo._VertBuf[3], 1, recipCameraZ);
			RwIm2DVertexSetV(&m_MergedInfo._VertBuf[3], 1, recipCameraZ);
			RwIm2DVertexSetIntRGBA(&m_MergedInfo._VertBuf[3], 255, 255, 255, 255);

			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_MergedInfo._VertBuf, 4);
		}
	}
}

void CMergeUIManager::RenderText(RtCharset* _pCharset)
{
	if(m_State == eFC_DRAG_MAP)
	{
		RwChar	caption[20] = {0,};
		RwInt32 WidthGridNum, HeightColGridNum;

		WidthGridNum		= abs(m_MergeDrag._Rect.left - m_MergeDrag._Rect.right) / m_FieldGridSize;
		HeightColGridNum	= abs(m_MergeDrag._Rect.top- m_MergeDrag._Rect.bottom) / m_FieldGridSize;

		RsSprintf(caption, RWSTRING("(%d, %d)"), WidthGridNum, HeightColGridNum);
		RsCharsetPrint(_pCharset, caption, dGET_MOUSE()->GetCusPos().x + 10, dGET_MOUSE()->GetCusPos().y);
	}
}

void CMergeUIManager::Render()
{
	if(!m_Working || CMiniMapManager::GetInstance()->IsCreating())
		return;

	// render drag info
	if(m_MergeDrag._bRender)
	{
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_MergeDrag._Line, 4);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	}

	// render current working fields
	if(m_FieldInfo._bRender)
	{
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldInfo._Line, 4);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	}

	// render field ui map
	if(m_MergeUIMap._pTextureField)
	{
		RwD3D9SetTexture(m_MergeUIMap._pTextureField, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_MergeUIMap._Panel, 4);
	}

	RwD3D9SetTexture(m_pTextureGrid, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_GridPanel, 4);

	// Main camera info
	if(m_CamInfo._bRender)
	{
		RwD3D9SetTexture(m_CamInfo._pTextureCamera, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_CamInfo._Line, 4);
	}

	RenderMergeArea();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////