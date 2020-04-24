#include "StdAfx.h"

#include "NtlWE.h"
#include "NtlWEDoc.h"

#include "NtlWEApplication.h"

#include "NtlText.h"

#include "NtlWorldMouse.h"
#include "NtlWorldDI.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLResourceManager.h"

#include "MergeUIManager.h"
#include "MiniMapManager.h"
#include "FieldUIManager.h"

// CFieldUIManager::CFieldUIManager(void)
// {
// 	m_pTextureGrid = NULL;
// 	::ZeroMemory(&m_FieldUIMap, sizeof(sFIELD_UIMAP));
// 	::ZeroMemory(&m_FieldDrag, sizeof(sDRAG_INFO));
// 	::ZeroMemory(&m_CamInfo, sizeof(sCAM_INFO));
// 	::ZeroMemory(&m_FieldInfo, sizeof(sFIELD_INFO));
// 	m_State = eFC_IDLE_MAP;
// 	m_Working = TRUE;
// 	m_Update = FALSE;
// }
// 
// CFieldUIManager::~CFieldUIManager(void)
// {
// 	Delete();
// }
// 
// void CFieldUIManager::Delete()
// {
// 	if(m_pTextureGrid)
// 	{
// 		RwTextureDestroy(m_pTextureGrid);
// 	}
// 
// 	if(m_FieldUIMap._pTextureField)
// 	{
// 		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_FieldUIMap._pTextureField);
// 		//RwTextureDestroy(m_FieldUIMap._pTextureField);
// 		m_FieldUIMap._pTextureField = NULL;
// 	}
// 
// 	if(m_FieldDrag._pTextureField)
// 	{
// 		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_FieldDrag._pTextureField);
// 		//RwTextureDestroy(m_FieldDrag._pTextureField);
// 		m_FieldDrag._pTextureField = NULL;
// 	}
// 
// 	if(m_CamInfo._pTextureCamera)
// 	{
// 		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_CamInfo._pTextureCamera);
// 		//RwTextureDestroy(m_CamInfo._pTextureCamera);
// 		m_CamInfo._pTextureCamera = NULL;
// 	}
// }
// 
// void CFieldUIManager::Create()
// {
// 	if(dGET_WORLD_PARAM()->WorldFieldNum == 8)
// 	{
// 		m_FieldGridNum	= 8;
// 		m_FieldGridSize	= 64;
// 	}
// 	else
// 	{
// 		m_FieldGridNum	= 16;
// 		m_FieldGridSize	= 32;
// 	}
// 
// 	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
// 	char szNewDir[NTL_MAX_DIR_PATH] = {0,};
// 
// 	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
// 	::strcpy(szNewDir, "Tool\\SABER\\Texture\\UI\\");
// 
// 	m_CamInfo._pTextureCamera = CNtlPLResourceManager::GetInstance()->LoadTexture("Camerafield", szNewDir);
// 
// 	if(dGET_WORLD_PARAM()->WorldFieldNum == 8)
// 	{
// 		m_pTextureGrid = CNtlPLResourceManager::GetInstance()->LoadTexture("Grid8x8", szNewDir);
// 	}
// 	else
// 	{
// 		m_pTextureGrid = CNtlPLResourceManager::GetInstance()->LoadTexture("Grid16x16", szNewDir);
// 	}
// 
// 	:: SetCurrentDirectory(szOldDir);
// 
// 	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
// 	RwInt32 nStartX = 0;
// 	RwInt32 nStartY = 0;
// 	RwInt32 width	= m_FieldGridNum * m_FieldGridSize;
// 	RwInt32 height	= m_FieldGridNum * m_FieldGridSize;
// 
// 	// GridPanel
// 	RwIm2DVertexSetScreenX(&m_GridPanel[0], nStartX - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_GridPanel[0], nStartY - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_GridPanel[0], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[0], recipCameraZ);
// 	RwIm2DVertexSetU(&m_GridPanel[0], 0, recipCameraZ);
// 	RwIm2DVertexSetV(&m_GridPanel[0], 0, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_GridPanel[0], 255, 255, 255, 100); 
// 
// 	RwIm2DVertexSetScreenX(&m_GridPanel[1], nStartX - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_GridPanel[1], nStartY + height - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_GridPanel[1], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[1], recipCameraZ);
// 	RwIm2DVertexSetU(&m_GridPanel[1], 0, recipCameraZ);
// 	RwIm2DVertexSetV(&m_GridPanel[1], 1, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_GridPanel[1], 255, 255, 255, 100);
// 
// 	RwIm2DVertexSetScreenX(&m_GridPanel[2], nStartX + width - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_GridPanel[2], nStartY - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_GridPanel[2], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[2], recipCameraZ);
// 	RwIm2DVertexSetU(&m_GridPanel[2], 1, recipCameraZ);
// 	RwIm2DVertexSetV(&m_GridPanel[2], 0, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_GridPanel[2], 255, 255, 255, 100); 
// 
// 	RwIm2DVertexSetScreenX(&m_GridPanel[3], nStartX + width - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_GridPanel[3], nStartY + height - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_GridPanel[3], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_GridPanel[3], recipCameraZ);
// 	RwIm2DVertexSetU(&m_GridPanel[3], 1, recipCameraZ);
// 	RwIm2DVertexSetV(&m_GridPanel[3], 1, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_GridPanel[3], 255, 255, 255, 100);
// 
// 	// FieldUIMap
// 	RwInt32 BlankFieldNum		= static_cast<RwInt32>((dGET_WORLD_PARAM()->WorldFieldNum - m_FieldGridNum) / 2);
// 	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
// 	RwInt32 VisibleFieldNum		= m_FieldGridNum;
// 	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
// 
// 	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[0], nStartX - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[0], nStartY - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[0], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[0], recipCameraZ);
// 	RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[0], 255, 255, 255, 100); 
// 
// 	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[1], nStartX - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[1], nStartY + height - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[1], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[1], recipCameraZ);
// 	RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], 1.0f - BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[1], 255, 255, 255, 100);
// 
// 	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[2], nStartX + width - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[2], nStartY - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[2], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[2], recipCameraZ);
// 	RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[2], 255, 255, 255, 100); 
// 
// 	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[3], nStartX + width - 0.5f);
// 	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[3], nStartY + height - 0.5f);
// 	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[3], RwIm2DGetNearScreenZ());
// 	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[3], recipCameraZ);
// 	RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], 1.0f - BlankFieldUVSize, recipCameraZ);
// 	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[3], 255, 255, 255, 100);
// 
// 	m_FieldUIMap._Collision.left	= 0;
// 	m_FieldUIMap._Collision.top		= 0;
// 	m_FieldUIMap._Collision.right	= m_FieldGridNum * m_FieldGridSize;
// 	m_FieldUIMap._Collision.bottom	= m_FieldGridNum * m_FieldGridSize;
// 
// 	m_FieldUIMap._FieldUVTabSize = 1.0f / dGET_WORLD_PARAM()->WorldFieldNum;
// }
// 
// void CFieldUIManager::Restore()
// {
// }
// 
// void CFieldUIManager::LoadFieldTexture(const char* pName)
// {
// 	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
// 	char szNewDir[NTL_MAX_DIR_PATH] = {0,};
// 
// 	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
// 	:: SetCurrentDirectory(dGET_WORLD_PARAM()->CurWorkingFolderName);
// 	::strcpy(szNewDir, "Tool/SABER/Texture/Grid Map/");
// 
// 	if(m_FieldUIMap._pTextureField)
// 	{
// 		RwTextureDestroy(m_FieldUIMap._pTextureField);
// 	}
// 
// 	m_FieldUIMap._pTextureField = CNtlPLResourceManager::GetInstance()->LoadTexture(pName, szNewDir);
// 
// 	:: SetCurrentDirectory(szOldDir);
// }
// 
// RwBool CFieldUIManager::IsCollidedInCurFieldArea(RECT& rt)
// {
// 	RECT CurFieldArea = GetCurFieldArea2World();
// 
// 	if(	rt.left >= CurFieldArea.left &&
// 		rt.top >= CurFieldArea.top &&
// 		rt.right <= CurFieldArea.right &&
// 		rt.bottom <= CurFieldArea.bottom)
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 
// 	return FALSE;
// }
// 
// RwBool CFieldUIManager::IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY)
// {
// 	RECT CurFieldArea = GetCurFieldArea2World();
// 
// 	if(	nX <= CurFieldArea.left && 
// 		nX >= CurFieldArea.right &&
// 		nY <= CurFieldArea.top &&
// 		nY >= CurFieldArea.bottom)
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 
// 	return FALSE;
// }
// 
// RwBool CFieldUIManager::IsCollided(RwInt32 nX, RwInt32 nY)
// {
// 	if(	nX >= m_FieldUIMap._Collision.left && 
// 		nX <= m_FieldUIMap._Collision.right &&
// 		nY >= m_FieldUIMap._Collision.top &&
// 		nY <= m_FieldUIMap._Collision.bottom)
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 
// 	return FALSE;
// }
// 
// void CFieldUIManager::MoveFieldUIMap(eFIELDUI_DIR Dir)
// {
// 	// 0(0, 0) 2(1, 0)
// 	// 1(0, 1) 3(1, 1)
// 
// 	RwInt32 BlankFieldNum		= static_cast<RwInt32>((dGET_WORLD_PARAM()->WorldFieldNum - m_FieldGridNum) / 2);
// 	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
// 	RwInt32 VisibleFieldNum		= m_FieldGridNum;
// 	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
// 	RwReal	Temp[4];
// 
// 	switch(Dir)
// 	{
// 	case eFD_N:
// 		{
// 			Temp[0] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[1] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[1]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[2] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[2]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[3] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[3]) - m_FieldUIMap._FieldUVTabSize;
// 
// 			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
// 			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);
// 
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
// 		}
// 		break;
// 	case eFD_E:
// 		{
// 			Temp[0] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[1] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[1]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[2] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[2]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[3] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[3]) + m_FieldUIMap._FieldUVTabSize;
// 
// 			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
// 			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);
// 
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
// 		}
// 		break;
// 	case eFD_S:
// 		{
// 			Temp[0] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[1] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[1]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[2] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[2]) + m_FieldUIMap._FieldUVTabSize;
// 			Temp[3] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[3]) + m_FieldUIMap._FieldUVTabSize;
// 
// 			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
// 			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);
// 
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
// 			RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
// 		}
// 	    break;
// 	case eFD_W:
// 		{
// 			Temp[0] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[1] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[1]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[2] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[2]) - m_FieldUIMap._FieldUVTabSize;
// 			Temp[3] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[3]) - m_FieldUIMap._FieldUVTabSize;
// 
// 			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
// 			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
// 			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);
// 							
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
// 			RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
// 		}
// 	    break;
// 	}
// }
// 
// RECT CFieldUIManager::GetFieldDragInfo()
// {
// 	return m_FieldDrag._Rect;
// }
// 
// RwV3d CFieldUIManager::GetFieldMidPos()
// {
// 	RwV3d RetPT;
// 
// 	RetPT.x = m_FieldDrag._Rect.left + (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
// 	RetPT.z = m_FieldDrag._Rect.top	+ (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
// 	RetPT.y = 0.0f;
// 
// 	return RetPT;
// }
// 
// RwV3d CFieldUIManager::GetWorldPT(RwV3d& FieldUIMapPT)
// {
// 	RwV3d RetPT;
// 
// 	RwReal fXMove = FieldUIMapPT.x / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
// 	RwReal fYMove = FieldUIMapPT.z / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
// 	fXMove *= (dGET_WORLD_PARAM()->WorldFieldSize * m_FieldGridNum);
// 	fYMove *= (dGET_WORLD_PARAM()->WorldFieldSize * m_FieldGridNum);
// 
// 	RECT CurWorldFieldAreaRT = GetCurFieldArea2World();
// 	RetPT.x = CurWorldFieldAreaRT.left - fXMove;
// 	RetPT.z = CurWorldFieldAreaRT.top - fYMove;
// 	RetPT.y = FieldUIMapPT.y;
// 	
// 	return RetPT;
// }
// 
// RECT CFieldUIManager::GetCurFieldArea2World()
// {
// 	// 0(0, 0) 2(1, 0)
// 	// 1(0, 1) 3(1, 1)
// 
// 	RECT rt;
// 	rt.left		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) / m_FieldUIMap._FieldUVTabSize * dGET_WORLD_PARAM()->WorldFieldSize));
// 	rt.top		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) / m_FieldUIMap._FieldUVTabSize * dGET_WORLD_PARAM()->WorldFieldSize));
// 	rt.right	= rt.left - m_FieldGridNum * dGET_WORLD_PARAM()->WorldFieldSize;
// 	rt.bottom	= rt.top - m_FieldGridNum * dGET_WORLD_PARAM()->WorldFieldSize;
// 
// 	return rt;
// }
// 
// // This function will be called when the param. must be existed in fielduimap area; see IsCollidedInCurFieldArea
// POINT CFieldUIManager::GetFieldSPosInFieldUIMap(RwReal x, RwReal z)
// {
// 	POINT	pt;
// 	RECT	CurFieldArea = GetCurFieldArea2World();
// 
// 	CurFieldArea.left	+= dGET_WORLD_PARAM()->WorldSizeHalf;
// 	CurFieldArea.top	+= dGET_WORLD_PARAM()->WorldSizeHalf;
// 	x					+= dGET_WORLD_PARAM()->WorldSizeHalf;
// 	z					+= dGET_WORLD_PARAM()->WorldSizeHalf;
// 	pt.x				= static_cast<LONG>(CurFieldArea.left - x);
// 	pt.y				= static_cast<LONG>(CurFieldArea.top - z);
// 	pt.x				/= dGET_WORLD_PARAM()->WorldFieldSize;
// 	pt.y				/= dGET_WORLD_PARAM()->WorldFieldSize;
// 	pt.x				*= m_FieldGridSize;
// 	pt.y				*= m_FieldGridSize;
// 
// 	return pt;
// }
// 
// void CFieldUIManager::UpdateRenderInfo()
// {
// 	// The field where the current camera's being
// 	RwV3d CamPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 
// 	if(IsCollidedInCurFieldArea(static_cast<RwInt32>(CamPos.x), static_cast<RwInt32>(CamPos.z)))
// 	{
// 		POINT SPos = GetFieldSPosInFieldUIMap(CamPos.x, CamPos.z);
// 
// 		RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
// 
// 		RwIm2DVertexSetScreenX(&m_CamInfo._Line[0], SPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_CamInfo._Line[0], SPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[0], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[0], recipCameraZ);
// 		RwIm2DVertexSetU(&m_CamInfo._Line[0], 0, recipCameraZ);
// 		RwIm2DVertexSetV(&m_CamInfo._Line[0], 0, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[0], 255, 255, 0, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_CamInfo._Line[1], SPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_CamInfo._Line[1], SPos.y + m_FieldGridSize - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[1], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[1], recipCameraZ);
// 		RwIm2DVertexSetU(&m_CamInfo._Line[1], 0, recipCameraZ);
// 		RwIm2DVertexSetV(&m_CamInfo._Line[1], 1, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[1], 255, 255, 0, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_CamInfo._Line[2], SPos.x + m_FieldGridSize - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_CamInfo._Line[2], SPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[2], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[2], recipCameraZ);
// 		RwIm2DVertexSetU(&m_CamInfo._Line[2], 1, recipCameraZ);
// 		RwIm2DVertexSetV(&m_CamInfo._Line[2], 0, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[2], 255, 255, 0, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_CamInfo._Line[3], SPos.x + m_FieldGridSize - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_CamInfo._Line[3], SPos.y + m_FieldGridSize - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_CamInfo._Line[3], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_CamInfo._Line[3], recipCameraZ);
// 		RwIm2DVertexSetU(&m_CamInfo._Line[3], 1, recipCameraZ);
// 		RwIm2DVertexSetV(&m_CamInfo._Line[3], 1, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_CamInfo._Line[3], 255, 255, 0, 100);
// 
// 		m_CamInfo._bRender = TRUE;
// 	}
// 	else
// 	{
// 		m_CamInfo._bRender = FALSE;
// 	}
// 
// 	// The fields loaded currently
// 	CNtlWorldFieldManager*	pFMgr = dGETMGR();
// 	RwInt32					FieldIndex[2] = {-999, -999};
// 
// 	for(int i = 35; i >= 0; --i)
// 	{
// 		if(pFMgr->m_Fields6x6[1][i] != -1)
// 		{
// 			FieldIndex[0] = pFMgr->m_Fields6x6[1][i];
// /*
// 			if(i == 35)
// 			{
// 				// full fields were loaded
// 				FieldIndex[0] = pFMgr->m_Fields6x6[1][35];
// 				FieldIndex[1] = pFMgr->m_Fields6x6[1][0];
// 			}*/
// 
// 			break;
// 		}
// 	}
// 
// 	if(FieldIndex[1] == -999)
// 	{
// 		for(int i = 0; i < 36; ++i)
// 		{
// 			if(pFMgr->m_Fields6x6[1][i] != -1)
// 			{
// 				FieldIndex[1] = pFMgr->m_Fields6x6[1][i];
// 				break;
// 			}
// 		}
// 	}
// 
// 	CNtlWorldField* pFields = const_cast<CNtlWorldField*>(pFMgr->GetFields());
// 	RwV3d SFieldWorldSPos	= pFields[FieldIndex[0]].GetSPos();
// 	RwV3d EFieldWorldSPos	= pFields[FieldIndex[1]].GetSPos();
// 
// 	// add WorldFieldSize 'cos of the drawing point
// 	SFieldWorldSPos.x		+= dGET_WORLD_PARAM()->WorldFieldSize;
// 	SFieldWorldSPos.z		+= dGET_WORLD_PARAM()->WorldFieldSize;
// 
// 	SFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
// 	SFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;
// 	EFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
// 	EFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;
// 
// 	RwV3d SFieldWorldSPosFinal;
// 	RwV3d EFieldWorldSPosFinal;
// 
// 	// Find the start and the end point of a drawing area
// 	RwBool Collided[2] = {FALSE, FALSE};
// 
// 	for(int j = static_cast<int>(EFieldWorldSPos.z); j <= static_cast<int>(SFieldWorldSPos.z); j += dGET_WORLD_PARAM()->WorldFieldSize)
// 	{
// 		for(int i = static_cast<int>(EFieldWorldSPos.x); i <= static_cast<int>(SFieldWorldSPos.x); i += dGET_WORLD_PARAM()->WorldFieldSize)
// 		{
// 			if(IsCollidedInCurFieldArea(i - dGET_WORLD_PARAM()->WorldSizeHalf, j - dGET_WORLD_PARAM()->WorldSizeHalf))
// 			{
// 				EFieldWorldSPosFinal.x = static_cast<RwReal>(i);
// 				EFieldWorldSPosFinal.z = static_cast<RwReal>(j);
// 				EFieldWorldSPosFinal.x -= dGET_WORLD_PARAM()->WorldSizeHalf;
// 				EFieldWorldSPosFinal.z -= dGET_WORLD_PARAM()->WorldSizeHalf;
// 
// 				Collided[0] = TRUE;
// 				break;
// 			}
// 		}
// 
// 		if(Collided[0])
// 			break;
// 	}
// 
// 	if(Collided[0])
// 	{
// 		for(int j = static_cast<int>(SFieldWorldSPos.z); j >= static_cast<int>(EFieldWorldSPos.z); j -= dGET_WORLD_PARAM()->WorldFieldSize)
// 		{
// 			for(int i = static_cast<int>(SFieldWorldSPos.x); i >= static_cast<int>(EFieldWorldSPos.x); i -= dGET_WORLD_PARAM()->WorldFieldSize)
// 			{
// 				if(IsCollidedInCurFieldArea(i - dGET_WORLD_PARAM()->WorldSizeHalf, j - dGET_WORLD_PARAM()->WorldSizeHalf))
// 				{
// 					SFieldWorldSPosFinal.x = static_cast<RwReal>(i);
// 					SFieldWorldSPosFinal.z = static_cast<RwReal>(j);
// 					SFieldWorldSPosFinal.x -= dGET_WORLD_PARAM()->WorldSizeHalf;
// 					SFieldWorldSPosFinal.z -= dGET_WORLD_PARAM()->WorldSizeHalf;
// 
// 					Collided[1] = TRUE;
// 					break;
// 				}
// 			}
// 
// 			if(Collided[1])
// 				break;
// 		}
// 	}
// 
// 	if(Collided[0] && Collided[1])
// 	{
// 		POINT SPos = GetFieldSPosInFieldUIMap(SFieldWorldSPosFinal.x, SFieldWorldSPosFinal.z);
// 		POINT EPos = GetFieldSPosInFieldUIMap(EFieldWorldSPosFinal.x, EFieldWorldSPosFinal.z);
// 
// 		RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
// 
// 		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[0], SPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[0], SPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[0], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[0], recipCameraZ);
// 		RwIm2DVertexSetU(&m_FieldInfo._Line[0], 0, recipCameraZ);
// 		RwIm2DVertexSetV(&m_FieldInfo._Line[0], 0, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[0], 0, 0, 255, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[1], SPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[1], EPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[1], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[1], recipCameraZ);
// 		RwIm2DVertexSetU(&m_FieldInfo._Line[1], 0, recipCameraZ);
// 		RwIm2DVertexSetV(&m_FieldInfo._Line[1], 1, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[1], 0, 0, 255, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[2], EPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[2], SPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[2], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[2], recipCameraZ);
// 		RwIm2DVertexSetU(&m_FieldInfo._Line[2], 1, recipCameraZ);
// 		RwIm2DVertexSetV(&m_FieldInfo._Line[2], 0, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[2], 0, 0, 255, 100);
// 
// 		RwIm2DVertexSetScreenX(&m_FieldInfo._Line[3], EPos.x - 0.5f);
// 		RwIm2DVertexSetScreenY(&m_FieldInfo._Line[3], EPos.y - 0.5f);
// 		RwIm2DVertexSetScreenZ(&m_FieldInfo._Line[3], RwIm2DGetNearScreenZ());
// 		RwIm2DVertexSetRecipCameraZ(&m_FieldInfo._Line[3], recipCameraZ);
// 		RwIm2DVertexSetU(&m_FieldInfo._Line[3], 1, recipCameraZ);
// 		RwIm2DVertexSetV(&m_FieldInfo._Line[3], 1, recipCameraZ);
// 		RwIm2DVertexSetIntRGBA(&m_FieldInfo._Line[3], 0, 0, 255, 100);
// 
// 		m_FieldInfo._bRender = TRUE;
// 	}
// 	else
// 	{
// 		m_FieldInfo._bRender = FALSE;
// 	}
// }
// 
// RwBool CFieldUIManager::IsSelected()
// {
// 	if(!m_FieldDrag._bRender)
// 		return FALSE;
// 
// 	return TRUE;
// }
// 
// RwBool CFieldUIManager::IsSingleSelected()
// {
// 	if(!m_FieldDrag._bRender)
// 		return FALSE;
// 
// 	if(	m_FieldDrag._Rect.right - m_FieldDrag._Rect.left > m_FieldGridSize ||
// 		m_FieldDrag._Rect.bottom - m_FieldDrag._Rect.top > m_FieldGridSize)	
// 	{
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }
// 
// void CFieldUIManager::Update()
// {               
// 	if(!m_Working)
// 		return;
// 
// 	if(!m_FieldUIMap._pTextureField)
// 		return;
// 
// 	// Update the camera field & the loaded fields
// 	UpdateRenderInfo();
// 
// 	if(dOKEY(DIK_SPACE))
// 	{
// 		m_FieldDrag._bRender = FALSE;
// 	}
// 
// 	if(!dGET_MOUSE()->GetBehavior(MB_LBDN) && !dGET_MOUSE()->GetLBUp())
// 	{
// 		return;
// 	}
// 
// 	if(dGET_MOUSE()->GetBehavior(MB_RBDN))
// 	{
// 		return;
// 	}
// 
// 	POINT Pt;
// 	Pt.x = dGET_MOUSE()->GetCusPos().x;
// 	Pt.y = dGET_MOUSE()->GetCusPos().y;
// 
// 	// update protection flag
// 	if(dGET_MOUSE()->GetLBOnce())
// 	{
// 		if(IsCollided(Pt.x, Pt.y))
// 		{
// 			m_Update = TRUE;
// 		}
// 	}
// 
// 	if(dGET_MOUSE()->GetLBUp())
// 	{
// 		m_State = eFC_IDLE_MAP;
// 		m_Update = FALSE;
// 	}
// 
// 	if(!m_Update)
// 	{
// 		m_State = eFC_IDLE_MAP;
// 		return;
// 	}
// 
// 	switch(m_State)
// 	{
// 	case eFC_IDLE_MAP:
// 		{
// 			if(!IsCollided(Pt.x, Pt.y))
// 			{
// 				break;
// 			}
// 
// 			if(dKEY(DIK_LCONTROL))
// 			{
// 				m_State = eFC_MOVE_MAP;
// 			}
// 			else if(dGET_MOUSE()->GetBehavior(MB_LBDN))
// 			{
// 				m_FieldDrag._SPos = Pt;
// 				m_State = eFC_DRAG_MAP;
// 			}
// 		}
// 		break;
// 
// 	case eFC_MOVE_MAP:
// 		{
// 			m_FieldDrag._bRender = FALSE;
// 			dGETHDL()->m_pMergeUIManager->m_MergeDrag._bRender = FALSE;
// 
// 			if(!dKEY(DIK_LCONTROL) || dGET_MOUSE()->GetLBUp())
// 			{
// 				m_State = eFC_IDLE_MAP;
// 				break;
// 			}
// 			else
// 			{
// 				RwReal TabSize		= 1.0f;
// 				RwReal XMovement	= dGET_MOUSE()->GetXMovementWithRate(1.0f);
// 				RwReal YMovement	= dGET_MOUSE()->GetYMovementWithRate(1.0f);
// 
// 				if(XMovement > +TabSize)	MoveFieldUIMap(eFD_W);
// 				if(XMovement < -TabSize)	MoveFieldUIMap(eFD_E);
// 				if(YMovement > +TabSize)	MoveFieldUIMap(eFD_N);
// 				if(YMovement < -TabSize)	MoveFieldUIMap(eFD_S);
// 			}
// 		}
// 		break;
// 
// 	case eFC_DRAG_MAP:
// 		{
// 			if(dGET_MOUSE()->GetLBUp())
// 			{
// 				m_State = eFC_IDLE_MAP;
// 				break;
// 			}
// 			else
// 			{
// 				m_FieldDrag._EPos.x = dGET_MOUSE()->GetCusPos().x;
// 				m_FieldDrag._EPos.y = dGET_MOUSE()->GetCusPos().y;
// 
// 				if(m_FieldDrag._SPos.x < m_FieldDrag._EPos.x)
// 				{
// 					m_FieldDrag._Rect.left	= m_FieldDrag._SPos.x;
// 					m_FieldDrag._Rect.right	= m_FieldDrag._EPos.x;
// 				}
// 				else
// 				{
// 					m_FieldDrag._Rect.left	= m_FieldDrag._EPos.x;
// 					m_FieldDrag._Rect.right	= m_FieldDrag._SPos.x;
// 				}
// 
// 				if(m_FieldDrag._SPos.y < m_FieldDrag._EPos.y)
// 				{
// 					m_FieldDrag._Rect.top		= m_FieldDrag._SPos.y;
// 					m_FieldDrag._Rect.bottom	= m_FieldDrag._EPos.y;
// 				}
// 				else
// 				{
// 					m_FieldDrag._Rect.top		= m_FieldDrag._EPos.y;
// 					m_FieldDrag._Rect.bottom	= m_FieldDrag._SPos.y;
// 				}
// 
// 				m_FieldDrag._Rect.left		= m_FieldDrag._Rect.left   / m_FieldGridSize * m_FieldGridSize;
// 				m_FieldDrag._Rect.top		= m_FieldDrag._Rect.top	   / m_FieldGridSize * m_FieldGridSize;
// 				m_FieldDrag._Rect.right		= m_FieldDrag._Rect.right  / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
// 				m_FieldDrag._Rect.bottom	= m_FieldDrag._Rect.bottom / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
// 				CLAMP(m_FieldDrag._Rect.left, 0, m_FieldGridNum * m_FieldGridSize);
// 				CLAMP(m_FieldDrag._Rect.top, 0, m_FieldGridNum * m_FieldGridSize);
// 				CLAMP(m_FieldDrag._Rect.right, 0, m_FieldGridNum * m_FieldGridSize);
// 				CLAMP(m_FieldDrag._Rect.bottom, 0, m_FieldGridNum * m_FieldGridSize);
// 
// 				// update geometry
// 				RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
// 
// 				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[0], m_FieldDrag._Rect.left - 0.5f);
// 				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[0], m_FieldDrag._Rect.top - 0.5f);
// 				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[0], RwIm2DGetNearScreenZ());
// 				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[0], recipCameraZ);
// 				RwIm2DVertexSetU(&m_FieldDrag._Line[0], 0, recipCameraZ);
// 				RwIm2DVertexSetV(&m_FieldDrag._Line[0], 0, recipCameraZ);
// 				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[0], 255, 0, 0, 100);
// 
// 				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[1], m_FieldDrag._Rect.left - 0.5f);
// 				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[1], m_FieldDrag._Rect.bottom - 0.5f);
// 				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[1], RwIm2DGetNearScreenZ());
// 				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[1], recipCameraZ);
// 				RwIm2DVertexSetU(&m_FieldDrag._Line[1], 0, recipCameraZ);
// 				RwIm2DVertexSetV(&m_FieldDrag._Line[1], 1, recipCameraZ);
// 				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[1], 255, 0, 0, 100);
// 
// 				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[2], m_FieldDrag._Rect.right - 0.5f);
// 				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[2], m_FieldDrag._Rect.top - 0.5f);
// 				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[2], RwIm2DGetNearScreenZ());
// 				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[2], recipCameraZ);
// 				RwIm2DVertexSetU(&m_FieldDrag._Line[2], 1, recipCameraZ);
// 				RwIm2DVertexSetV(&m_FieldDrag._Line[2], 0, recipCameraZ);
// 				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[2], 255, 0, 0, 100);
// 
// 				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[3], m_FieldDrag._Rect.right - 0.5f);
// 				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[3], m_FieldDrag._Rect.bottom - 0.5f);
// 				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[3], RwIm2DGetNearScreenZ());
// 				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[3], recipCameraZ);
// 				RwIm2DVertexSetU(&m_FieldDrag._Line[3], 1, recipCameraZ);
// 				RwIm2DVertexSetV(&m_FieldDrag._Line[3], 1, recipCameraZ);
// 				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[3], 255, 0, 0, 100);
// 
// 				m_FieldDrag._bRender = TRUE;
// 			}
// 		}
// 		break;
// 	}
// }
// 
// RwBool CFieldUIManager::IsWorking()
// {
// 	if(m_FieldUIMap._pTextureField && m_Working)// && m_FieldInfo._bRender) 
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// }
// 
// void CFieldUIManager::RenderText(RtCharset* _pCharset)
// {
// 	if(m_State == eFC_DRAG_MAP)
// 	{
// 		RwChar	caption[20] = {0,};
// 		RwInt32 WidthGridNum, HeightColGridNum;
// 		
// 		WidthGridNum		= abs(m_FieldDrag._Rect.left - m_FieldDrag._Rect.right) / m_FieldGridSize;
// 		HeightColGridNum	= abs(m_FieldDrag._Rect.top- m_FieldDrag._Rect.bottom) / m_FieldGridSize;
// 
// 		RsSprintf(caption, RWSTRING("(%d, %d)"), WidthGridNum, HeightColGridNum);
// 		RsCharsetPrint(_pCharset, caption, dGET_MOUSE()->GetCusPos().x + 10, dGET_MOUSE()->GetCusPos().y);
// 	}
// }
// 
// void CFieldUIManager::Render()
// {
// 	if(!m_Working || CMiniMapManager::GetInstance()->IsCreating())
// 		return;
// 
// 	// render drag info
// 	if(m_FieldDrag._bRender)
// 	{
// 		RwD3D9SetTexture(NULL, 0);
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
// 
// 		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldDrag._Line, 4);
// 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
// 	}
// 
// 	// render current working fields
// 	if(m_FieldInfo._bRender)
// 	{
// 		RwD3D9SetTexture(NULL, 0);
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
// 
// 		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldInfo._Line, 4);
// 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
// 		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	}
// 
// 	// render field ui map
// 	if(m_FieldUIMap._pTextureField)
// 	{
// 		RwD3D9SetTexture(m_FieldUIMap._pTextureField, 0);
// 		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldUIMap._Panel, 4);
// 	}
// 
// 	RwD3D9SetTexture(m_pTextureGrid, 0);
// 	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_GridPanel, 4);
// 
// 	// Main camera info
// 	if(m_CamInfo._bRender)
// 	{
// 		RwD3D9SetTexture(m_CamInfo._pTextureCamera, 0);
// 		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_CamInfo._Line, 4);
// 	}
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFieldUIManager::CFieldUIManager(void)
{
	m_pTextureGrid = NULL;
	::ZeroMemory(&m_FieldUIMap, sizeof(sFIELD_UIMAP));
	::ZeroMemory(&m_FieldDrag, sizeof(sDRAG_INFO));
	::ZeroMemory(&m_CamInfo, sizeof(sCAM_INFO));
	::ZeroMemory(&m_FieldInfo, sizeof(sFIELD_INFO));
	m_State = eFC_IDLE_MAP;
	m_Working = TRUE;
	m_Update = FALSE;
}

CFieldUIManager::~CFieldUIManager(void)
{
	Delete();
}

void CFieldUIManager::Delete()
{
	if(m_pTextureGrid)
	{
		RwTextureDestroy(m_pTextureGrid);
	}

	if(m_FieldUIMap._pTextureField)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_FieldUIMap._pTextureField);
		//RwTextureDestroy(m_FieldUIMap._pTextureField);
		m_FieldUIMap._pTextureField = NULL;
	}

	if(m_FieldDrag._pTextureField)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_FieldDrag._pTextureField);
		//RwTextureDestroy(m_FieldDrag._pTextureField);
		m_FieldDrag._pTextureField = NULL;
	}

	if(m_CamInfo._pTextureCamera)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_CamInfo._pTextureCamera);
		//RwTextureDestroy(m_CamInfo._pTextureCamera);
		m_CamInfo._pTextureCamera = NULL;
	}
}

void CFieldUIManager::Create(RwInt32 iWorldFieldNum, RwInt32 iFieldSize)
{
	m_iWorldFieldSize	= iFieldSize;
	m_iWorldFieldNum	= iWorldFieldNum;

	if(m_iWorldFieldNum == 8)
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

	if(m_iWorldFieldNum == 8)
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
	RwInt32 nStartX = 0;
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
	RwInt32 BlankFieldNum		= static_cast<RwInt32>((m_iWorldFieldNum - m_FieldGridNum) / 2);
	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(m_iWorldFieldNum);
	RwInt32 VisibleFieldNum		= m_FieldGridNum;
	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(m_iWorldFieldNum);

	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[0], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[0], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[0], recipCameraZ);
	RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[0], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[1], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[1], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[1], recipCameraZ);
	RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], 1.0f - BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[1], 255, 255, 255, 100);

	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[2], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[2], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[2], recipCameraZ);
	RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[2], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_FieldUIMap._Panel[3], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_FieldUIMap._Panel[3], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_FieldUIMap._Panel[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_FieldUIMap._Panel[3], recipCameraZ);
	RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], BlankFieldUVSize + VisibleFieldUVSize, recipCameraZ);
	RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], 1.0f - BlankFieldUVSize, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_FieldUIMap._Panel[3], 255, 255, 255, 100);

	m_FieldUIMap._Collision.left	= 0;
	m_FieldUIMap._Collision.top		= 0;
	m_FieldUIMap._Collision.right	= m_FieldGridNum * m_FieldGridSize;
	m_FieldUIMap._Collision.bottom	= m_FieldGridNum * m_FieldGridSize;

	m_FieldUIMap._FieldUVTabSize = 1.0f / m_iWorldFieldNum;
}

void CFieldUIManager::Restore()
{
}

void CFieldUIManager::LoadFieldTexture(const char* pName)
{
	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewDir[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	:: SetCurrentDirectory(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::strcpy(szNewDir, "Tool/SABER/Texture/Grid Map/");

	if(m_FieldUIMap._pTextureField)
	{
		RwTextureDestroy(m_FieldUIMap._pTextureField);
	}

	m_FieldUIMap._pTextureField = CNtlPLResourceManager::GetInstance()->LoadTexture(pName, szNewDir);
	DBO_ASSERT(m_FieldUIMap._pTextureField, "Texture load failed.");

	:: SetCurrentDirectory(szOldDir);
}

RwBool CFieldUIManager::IsCollidedInCurFieldArea(RECT& rt)
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

RwBool CFieldUIManager::IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY)
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

RwBool CFieldUIManager::IsCollided(RwInt32 nX, RwInt32 nY)
{
	if(	nX >= m_FieldUIMap._Collision.left && 
		nX <= m_FieldUIMap._Collision.right &&
		nY >= m_FieldUIMap._Collision.top &&
		nY <= m_FieldUIMap._Collision.bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void CFieldUIManager::MoveFieldUIMap(eFIELDUI_DIR Dir)
{
	// 0(0, 0) 2(1, 0)
	// 1(0, 1) 3(1, 1)

	RwInt32 BlankFieldNum		= static_cast<RwInt32>((m_iWorldFieldNum - m_FieldGridNum) / 2);
	RwReal	BlankFieldUVSize	= static_cast<RwReal>(BlankFieldNum) / static_cast<RwReal>(m_iWorldFieldNum);
	RwInt32 VisibleFieldNum		= m_FieldGridNum;
	RwReal	VisibleFieldUVSize	= static_cast<RwReal>(VisibleFieldNum) / static_cast<RwReal>(m_iWorldFieldNum);
	RwReal	Temp[4];

	switch(Dir)
	{
	case eFD_N:
		{
			Temp[0] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) - m_FieldUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[1]) - m_FieldUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[2]) - m_FieldUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[3]) - m_FieldUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	case eFD_E:
		{
			Temp[0] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) + m_FieldUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[1]) + m_FieldUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[2]) + m_FieldUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[3]) + m_FieldUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	case eFD_S:
		{
			Temp[0] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) + m_FieldUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[1]) + m_FieldUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[2]) + m_FieldUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetV(&m_FieldUIMap._Panel[3]) + m_FieldUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[2], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetV(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetV(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	case eFD_W:
		{
			Temp[0] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) - m_FieldUIMap._FieldUVTabSize;
			Temp[1] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[1]) - m_FieldUIMap._FieldUVTabSize;
			Temp[2] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[2]) - m_FieldUIMap._FieldUVTabSize;
			Temp[3] = RwIm2DVertexGetU(&m_FieldUIMap._Panel[3]) - m_FieldUIMap._FieldUVTabSize;

			CLAMP(Temp[0], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[1], 0.0f, 1.0f - VisibleFieldUVSize);
			CLAMP(Temp[2], VisibleFieldUVSize, 1.0f);
			CLAMP(Temp[3], VisibleFieldUVSize, 1.0f);

			RwIm2DVertexSetU(&m_FieldUIMap._Panel[0], Temp[0], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[1], Temp[1], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[2], Temp[2], recipCameraZ);
			RwIm2DVertexSetU(&m_FieldUIMap._Panel[3], Temp[3], recipCameraZ);
		}
		break;
	}
}

RECT CFieldUIManager::GetFieldDragInfo()
{
	return m_FieldDrag._Rect;
}

RwV3d CFieldUIManager::GetFieldMidPos()
{
	RwV3d RetPT;

	RetPT.x = m_FieldDrag._Rect.left + (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.z = m_FieldDrag._Rect.top	+ (static_cast<RwReal>(m_FieldGridSize) / 2.0f);
	RetPT.y = 0.0f;

	return RetPT;
}

RwV3d CFieldUIManager::GetWorldPT(RwV3d& FieldUIMapPT)
{
	RwV3d RetPT;

	RwReal fXMove = FieldUIMapPT.x / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
	RwReal fYMove = FieldUIMapPT.z / static_cast<RwReal>(m_FieldGridNum * m_FieldGridSize);
	fXMove *= (m_iWorldFieldSize * m_FieldGridNum);
	fYMove *= (m_iWorldFieldSize * m_FieldGridNum);

	RECT CurWorldFieldAreaRT = GetCurFieldArea2World();
	RetPT.x = CurWorldFieldAreaRT.left - fXMove;
	RetPT.z = CurWorldFieldAreaRT.top - fYMove;
	RetPT.y = FieldUIMapPT.y;

	return RetPT;
}

RECT CFieldUIManager::GetCurFieldArea2World()
{
	// 0(0, 0) 2(1, 0)
	// 1(0, 1) 3(1, 1)

	RECT rt;
	rt.left		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetU(&m_FieldUIMap._Panel[0]) / m_FieldUIMap._FieldUVTabSize * m_iWorldFieldSize));
	rt.top		= static_cast<LONG>(dGET_WORLD_PARAM()->WorldValueMax - (RwIm2DVertexGetV(&m_FieldUIMap._Panel[0]) / m_FieldUIMap._FieldUVTabSize * m_iWorldFieldSize));
	rt.right	= rt.left - m_FieldGridNum * m_iWorldFieldSize;
	rt.bottom	= rt.top - m_FieldGridNum * m_iWorldFieldSize;

	return rt;
}

// This function will be called when the param. must be existed in fielduimap area; see IsCollidedInCurFieldArea
POINT CFieldUIManager::GetFieldSPosInFieldUIMap(RwReal x, RwReal z)
{
	POINT	pt;
	RECT	CurFieldArea = GetCurFieldArea2World();

	CurFieldArea.left	+= dGET_WORLD_PARAM()->WorldSizeHalf;
	CurFieldArea.top	+= dGET_WORLD_PARAM()->WorldSizeHalf;
	x					+= dGET_WORLD_PARAM()->WorldSizeHalf;
	z					+= dGET_WORLD_PARAM()->WorldSizeHalf;
	pt.x				= static_cast<LONG>(CurFieldArea.left - x);
	pt.y				= static_cast<LONG>(CurFieldArea.top - z);
	pt.x				/= m_iWorldFieldSize;
	pt.y				/= m_iWorldFieldSize;
	pt.x				*= m_FieldGridSize;
	pt.y				*= m_FieldGridSize;

	return pt;
}

void CFieldUIManager::UpdateRenderInfo()
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
			/*
			if(i == 35)
			{
			// full fields were loaded
			FieldIndex[0] = pFMgr->m_Fields6x6[1][35];
			FieldIndex[1] = pFMgr->m_Fields6x6[1][0];
			}*/

			break;
		}
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
	SFieldWorldSPos.x		+= dGET_WORLD_PARAM()->WorldFieldSize;//m_iWorldFieldSize;
	SFieldWorldSPos.z		+= dGET_WORLD_PARAM()->WorldFieldSize;//m_iWorldFieldSize;

	SFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
	SFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;
	EFieldWorldSPos.x += dGET_WORLD_PARAM()->WorldSizeHalf;
	EFieldWorldSPos.z += dGET_WORLD_PARAM()->WorldSizeHalf;

	RwV3d SFieldWorldSPosFinal;
	RwV3d EFieldWorldSPosFinal;

	// Find the start and the end point of a drawing area
	RwBool Collided[2] = {FALSE, FALSE};

	for(int j = static_cast<int>(EFieldWorldSPos.z); j <= static_cast<int>(SFieldWorldSPos.z); j += m_iWorldFieldSize)
	{
		for(int i = static_cast<int>(EFieldWorldSPos.x); i <= static_cast<int>(SFieldWorldSPos.x); i += m_iWorldFieldSize)
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
		for(int j = static_cast<int>(SFieldWorldSPos.z); j >= static_cast<int>(EFieldWorldSPos.z); j -= m_iWorldFieldSize)
		{
			for(int i = static_cast<int>(SFieldWorldSPos.x); i >= static_cast<int>(EFieldWorldSPos.x); i -= m_iWorldFieldSize)
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

RwBool CFieldUIManager::IsSelected()
{
	if(!m_FieldDrag._bRender)
		return FALSE;

	return TRUE;
}

RwBool CFieldUIManager::IsSingleSelected()
{
	if(!m_FieldDrag._bRender)
		return FALSE;

	if(	m_FieldDrag._Rect.right - m_FieldDrag._Rect.left > m_FieldGridSize ||
		m_FieldDrag._Rect.bottom - m_FieldDrag._Rect.top > m_FieldGridSize)	
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldUIManager::Update()
{               
	if(!m_Working)
		return;

	if(!m_FieldUIMap._pTextureField)
		return;

	// Update the camera field & the loaded fields
	UpdateRenderInfo();

	if(dOKEY(DIK_SPACE))
	{
		m_FieldDrag._bRender = FALSE;
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
				m_FieldDrag._SPos = Pt;
				m_State = eFC_DRAG_MAP;
			}
		}
		break;

	case eFC_MOVE_MAP:
		{
			m_FieldDrag._bRender = FALSE;
			dGETHDL()->m_pMergeUIManager->m_MergeDrag._bRender = FALSE;

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
				m_FieldDrag._EPos.x = dGET_MOUSE()->GetCusPos().x;
				m_FieldDrag._EPos.y = dGET_MOUSE()->GetCusPos().y;

				if(m_FieldDrag._SPos.x < m_FieldDrag._EPos.x)
				{
					m_FieldDrag._Rect.left	= m_FieldDrag._SPos.x;
					m_FieldDrag._Rect.right	= m_FieldDrag._EPos.x;
				}
				else
				{
					m_FieldDrag._Rect.left	= m_FieldDrag._EPos.x;
					m_FieldDrag._Rect.right	= m_FieldDrag._SPos.x;
				}

				if(m_FieldDrag._SPos.y < m_FieldDrag._EPos.y)
				{
					m_FieldDrag._Rect.top		= m_FieldDrag._SPos.y;
					m_FieldDrag._Rect.bottom	= m_FieldDrag._EPos.y;
				}
				else
				{
					m_FieldDrag._Rect.top		= m_FieldDrag._EPos.y;
					m_FieldDrag._Rect.bottom	= m_FieldDrag._SPos.y;
				}

				m_FieldDrag._Rect.left		= m_FieldDrag._Rect.left   / m_FieldGridSize * m_FieldGridSize;
				m_FieldDrag._Rect.top		= m_FieldDrag._Rect.top	   / m_FieldGridSize * m_FieldGridSize;
				m_FieldDrag._Rect.right		= m_FieldDrag._Rect.right  / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
				m_FieldDrag._Rect.bottom	= m_FieldDrag._Rect.bottom / m_FieldGridSize * m_FieldGridSize + m_FieldGridSize;
				CLAMP(m_FieldDrag._Rect.left, 0, m_FieldGridNum * m_FieldGridSize);
				CLAMP(m_FieldDrag._Rect.top, 0, m_FieldGridNum * m_FieldGridSize);
				CLAMP(m_FieldDrag._Rect.right, 0, m_FieldGridNum * m_FieldGridSize);
				CLAMP(m_FieldDrag._Rect.bottom, 0, m_FieldGridNum * m_FieldGridSize);

				// update geometry
				RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[0], m_FieldDrag._Rect.left - 0.5f);
				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[0], m_FieldDrag._Rect.top - 0.5f);
				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[0], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[0], recipCameraZ);
				RwIm2DVertexSetU(&m_FieldDrag._Line[0], 0, recipCameraZ);
				RwIm2DVertexSetV(&m_FieldDrag._Line[0], 0, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[0], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[1], m_FieldDrag._Rect.left - 0.5f);
				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[1], m_FieldDrag._Rect.bottom - 0.5f);
				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[1], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[1], recipCameraZ);
				RwIm2DVertexSetU(&m_FieldDrag._Line[1], 0, recipCameraZ);
				RwIm2DVertexSetV(&m_FieldDrag._Line[1], 1, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[1], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[2], m_FieldDrag._Rect.right - 0.5f);
				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[2], m_FieldDrag._Rect.top - 0.5f);
				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[2], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[2], recipCameraZ);
				RwIm2DVertexSetU(&m_FieldDrag._Line[2], 1, recipCameraZ);
				RwIm2DVertexSetV(&m_FieldDrag._Line[2], 0, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[2], 255, 0, 0, 100);

				RwIm2DVertexSetScreenX(&m_FieldDrag._Line[3], m_FieldDrag._Rect.right - 0.5f);
				RwIm2DVertexSetScreenY(&m_FieldDrag._Line[3], m_FieldDrag._Rect.bottom - 0.5f);
				RwIm2DVertexSetScreenZ(&m_FieldDrag._Line[3], RwIm2DGetNearScreenZ());
				RwIm2DVertexSetRecipCameraZ(&m_FieldDrag._Line[3], recipCameraZ);
				RwIm2DVertexSetU(&m_FieldDrag._Line[3], 1, recipCameraZ);
				RwIm2DVertexSetV(&m_FieldDrag._Line[3], 1, recipCameraZ);
				RwIm2DVertexSetIntRGBA(&m_FieldDrag._Line[3], 255, 0, 0, 100);

				m_FieldDrag._bRender = TRUE;
			}
		}
		break;
	}
}

RwBool CFieldUIManager::IsWorking()
{
	if(m_FieldUIMap._pTextureField && m_Working)// && m_FieldInfo._bRender) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CFieldUIManager::RenderText(RtCharset* _pCharset)
{
	if(m_State == eFC_DRAG_MAP)
	{
		RwChar	caption[20] = {0,};
		RwInt32 WidthGridNum, HeightColGridNum;

		WidthGridNum		= abs(m_FieldDrag._Rect.left - m_FieldDrag._Rect.right) / m_FieldGridSize;
		HeightColGridNum	= abs(m_FieldDrag._Rect.top- m_FieldDrag._Rect.bottom) / m_FieldGridSize;

		RsSprintf(caption, RWSTRING("(%d, %d)"), WidthGridNum, HeightColGridNum);
		RsCharsetPrint(_pCharset, caption, dGET_MOUSE()->GetCusPos().x + 10, dGET_MOUSE()->GetCusPos().y);
	}
}

void CFieldUIManager::Render()
{
	if(!m_Working || CMiniMapManager::GetInstance()->IsCreating())
		return;

	// render drag info
	if(m_FieldDrag._bRender)
	{
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldDrag._Line, 4);

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
	if(m_FieldUIMap._pTextureField)
	{
		RwD3D9SetTexture(m_FieldUIMap._pTextureField, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FieldUIMap._Panel, 4);
	}

	RwD3D9SetTexture(m_pTextureGrid, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_GridPanel, 4);

	// Main camera info
	if(m_CamInfo._bRender)
	{
		RwD3D9SetTexture(m_CamInfo._pTextureCamera, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_CamInfo._Line, 4);
	}
}

void CFieldUIManager::ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID)
{
	if(!IsSelected())
	{
		AfxMessageBox("can't get a prop. select field blocks.");
		return;
	}

	CNtlWorldFieldManager*	pFieldMgr = dGETMGR();
	if(IsSingleSelected())
	{
		RwV3d CurFieldMidPosInUI	= GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

		pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *pNtlFieldProp, eNtlFieldPropID);
		AfxMessageBox("Set.");
	}
	else
	{
		RECT RTInUI = GetFieldDragInfo();
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = GetWorldPT(STmp);
		RwV3d EPos = GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		for(RwReal i = EPos.z; i <= SPos.z - m_iWorldFieldSize; i += m_iWorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - m_iWorldFieldSize; j += m_iWorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (m_iWorldFieldSize / 2.0f);
				CurPos.z = i + (m_iWorldFieldSize / 2.0f);

				pFieldMgr->SetAFieldProp(CurPos, *pNtlFieldProp, eNtlFieldPropID);
			}
		}
		AfxMessageBox("Set.");
	}
}