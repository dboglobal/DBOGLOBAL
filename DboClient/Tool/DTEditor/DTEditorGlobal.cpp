#include "stdafx.h"
#include "DTEditor.h"
#include "MainFrm.h"
#include "DTEditorDoc.h"
#include "DTEditorView.h"

#include "DTEditorGlobal.h"
#include "DTServer.h"

// shared
#include "NtlCharacter.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlSLLogic.h"
#include "NtlSoundMoodManager.h"


#define DT_GRID_LINE_X			40
#define DT_GRID_LINE_Y			40

#define DT_GRID_INTERVAL		1.0f

CNtlDTProp		*g_pActiveDTProp	= NULL;
EEditModeType	g_EditModeType		= EDIT_MODE_THIRD_CAMERA;
RwV3d			g_vSplineCenter;

int				g_nSelSx			= -1;
int				g_nSelEx			= -1;
int				g_nFocusIdx			= -1;


RwV3d			g_vGridCenter		= { 0.0f, 0.0f, 0.0f} ;
RwInt32			g_iLineCx			= DT_GRID_LINE_X;
RwInt32			g_iLineCy			= DT_GRID_LINE_Y;
RwReal			g_fGridInterval		= DT_GRID_INTERVAL;

RwIm3DVertex	*g_pAxisXVertex		= NULL;
RwIm3DVertex	*g_pAxisZVertex		= NULL;
RwIm3DVertex	*g_pLineXVertex		= NULL;
RwIm3DVertex	*g_pLineYVertex		= NULL;

RwBool			g_bChangePointPos		= TRUE;
RwBool			g_bChangePointDir		= TRUE;
RwBool			g_bCameraMovePointClick	= FALSE;

RwReal			g_fSplineBoxSize	= 0.05f;	
RwBool			g_bCustomDataRender = FALSE;


// veiwer control data

CDTServer				g_DTServer;
RwUInt32				g_uiSerialId = 0;
SERIAL_HANDLE			g_hSelHandle = INVALID_SERIAL_ID;
RwV3d					g_vSplineStandardPos = {0.0f, 0.0f, 0.0f};

const sWORLD_TBLDAT		*g_pActiveWorldTable	= NULL;
const sMOB_TBLDAT		*g_pActiveMonsterTable	= NULL;
const sHTB_SET_TBLDAT	*g_pHTBSetTblData		= NULL;

RwUInt8					g_byRace	= RACE_HUMAN;
RwUInt8					g_byClass	= PC_CLASS_STREET_FIGHTER;
RwUInt8					g_byGender	= GENDER_MALE;

// camera attribute

RwReal					g_fFreeCameraSpeed = 0.1f;


void MakeLineAxisVertex(RwReal fLineCx, RwReal fLineCy)
{
	RwInt32 iIdx;

	// x-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&g_pAxisXVertex[iIdx], 
						-(fLineCy/2)*g_fGridInterval,
						0.05f, 
						0.0f);
	RwIm3DVertexSetNormal(&g_pAxisXVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&g_pAxisXVertex[iIdx], 255, 0, 0, 100);
	g_pAxisXVertex[iIdx].u = 0.0f;
	g_pAxisXVertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&g_pAxisXVertex[iIdx], 
						(fLineCy/2)*g_fGridInterval,
						0.05f, 
						0.0f);
	RwIm3DVertexSetNormal(&g_pAxisXVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&g_pAxisXVertex[iIdx], 255, 0, 0, 100);
	g_pAxisXVertex[iIdx].u = 0.0f;
	g_pAxisXVertex[iIdx].v = 0.0f;

	// z-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&g_pAxisZVertex[iIdx], 
						0.0f,
						0.05f, 
						-(fLineCx/2)*g_fGridInterval);
	RwIm3DVertexSetNormal(&g_pAxisZVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&g_pAxisZVertex[iIdx], 0, 0, 255, 100);
	g_pAxisZVertex[iIdx].u = 0.0f;
	g_pAxisZVertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&g_pAxisZVertex[iIdx], 
						0.0f,
						0.05f, 
						(fLineCx/2)*g_fGridInterval);
	RwIm3DVertexSetNormal(&g_pAxisZVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&g_pAxisZVertex[iIdx], 0, 0, 255, 100);
	g_pAxisZVertex[iIdx].u = 0.0f;
	g_pAxisZVertex[iIdx].v = 0.0f;
}

void MakeGridLineXVertex(void)
{
	RwInt32 iIdx;
	RwInt32 iHalfCx = g_iLineCx/2;

	for(RwInt32 i = 0; i < iHalfCx; i++)
	{
		iIdx = i*2;
		RwIm3DVertexSetPos(&g_pLineXVertex[iIdx], 
							-g_fGridInterval*(RwReal)(i+1),
							0.05f, 
							g_fGridInterval*iHalfCx);
		RwIm3DVertexSetNormal(&g_pLineXVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 0, 100);
		g_pLineXVertex[iIdx].u = 0.0f;
		g_pLineXVertex[iIdx].v = 0.0f;

		iIdx = i*2+1;
		RwIm3DVertexSetPos(&g_pLineXVertex[iIdx], 
							-g_fGridInterval*(RwReal)(i+1),
							0.05f, 
							-g_fGridInterval*iHalfCx);
		RwIm3DVertexSetNormal(&g_pLineXVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 0, 100);
		g_pLineXVertex[iIdx].u = 0.0f;
		g_pLineXVertex[iIdx].v = 0.0f;
	}

	
	for(RwInt32 i = 0; i < iHalfCx; i++)
	{
		iIdx = (iHalfCx+i)*2;
		RwIm3DVertexSetPos(&g_pLineXVertex[iIdx], 
							g_fGridInterval*(RwReal)(i+1),
							0.05f, 
							g_fGridInterval*iHalfCx);
		RwIm3DVertexSetNormal(&g_pLineXVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 0, 100);
		g_pLineXVertex[iIdx].u = 0.0f;
		g_pLineXVertex[iIdx].v = 0.0f;

		iIdx = (iHalfCx+i)*2+1;
		RwIm3DVertexSetPos(&g_pLineXVertex[iIdx], 
							g_fGridInterval*(RwReal)(i+1),
							0.05f, 
							-g_fGridInterval*iHalfCx);
		RwIm3DVertexSetNormal(&g_pLineXVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineXVertex[iIdx], 255, 255, 0, 100);
		g_pLineXVertex[iIdx].u = 0.0f;
		g_pLineXVertex[iIdx].v = 0.0f;
	}
	
}

void MakeGridLineYVertex(void)
{
	RwInt32 iIdx;
	RwInt32 iHalfCy = g_iLineCy/2;

	for(RwInt32 i = 0; i < iHalfCy; i++)
	{
		iIdx = i*2;
		RwIm3DVertexSetPos(&g_pLineYVertex[iIdx], 
							-g_fGridInterval*iHalfCy,
							0.05f, 
							-g_fGridInterval*(RwReal)(i+1));
		RwIm3DVertexSetNormal(&g_pLineYVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 0, 100);
		g_pLineYVertex[iIdx].u = 0.0f;
		g_pLineYVertex[iIdx].v = 0.0f;

		iIdx = i*2+1;
		RwIm3DVertexSetPos(&g_pLineYVertex[iIdx], 
							g_fGridInterval*iHalfCy,
							0.05f, 
							-g_fGridInterval*(RwReal)(i+1));
		RwIm3DVertexSetNormal(&g_pLineYVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 0, 100);
		g_pLineYVertex[iIdx].u = 0.0f;
		g_pLineYVertex[iIdx].v = 0.0f;
	}

	
	for(RwInt32 i = 0; i < iHalfCy; i++)
	{
		iIdx = (iHalfCy+i)*2;
		RwIm3DVertexSetPos(&g_pLineYVertex[iIdx], 
							-g_fGridInterval*iHalfCy,
							0.05f, 
							g_fGridInterval*(RwReal)(i+1));
		RwIm3DVertexSetNormal(&g_pLineYVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 0, 100);
		g_pLineYVertex[iIdx].u = 0.0f;
		g_pLineYVertex[iIdx].v = 0.0f;

		iIdx = (iHalfCy+i)*2+1;
		RwIm3DVertexSetPos(&g_pLineYVertex[iIdx], 
							g_fGridInterval*iHalfCy,
							0.05f, 
							g_fGridInterval*(RwReal)(i+1));
		RwIm3DVertexSetNormal(&g_pLineYVertex[iIdx], 0.0f, 1.0f, 0.0f);
		RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 255, 100);
		if((i+1)%5 == 0)
			RwIm3DVertexSetRGBA(&g_pLineYVertex[iIdx], 255, 255, 0, 100);
		g_pLineYVertex[iIdx].u = 0.0f;
		g_pLineYVertex[iIdx].v = 0.0f;
	}
	
}

RwBool CreateDTEditorGlobal(void)
{
	CNtlMath::MathRwV3dAssign(&g_vSplineCenter, 0.0f, 0.0f, 0.0f);

	g_pAxisXVertex = NTL_NEW RwIm3DVertex[2];
	g_pAxisZVertex = NTL_NEW RwIm3DVertex[2];
	g_pLineXVertex = NTL_NEW RwIm3DVertex[g_iLineCx*2];
	g_pLineYVertex = NTL_NEW RwIm3DVertex[g_iLineCy*2];

	MakeLineAxisVertex(g_iLineCx, g_iLineCy);
	MakeGridLineXVertex();
	MakeGridLineYVertex();

	Logic_EnableCharacterCollisionSlant(FALSE);
	Logic_EnableCharacterSlipping(FALSE);

	return TRUE;
}

void DestroyDTEditorGlobal(void)
{
	if(g_pAxisXVertex)
	{
		NTL_ARRAY_DELETE(g_pAxisXVertex);
	}

	if(g_pAxisZVertex)
	{
		NTL_ARRAY_DELETE(g_pAxisZVertex);
	}

	if(g_pLineXVertex)
	{
		NTL_ARRAY_DELETE(g_pLineXVertex);
	}

	if(g_pLineYVertex)
	{
		NTL_ARRAY_DELETE(g_pLineYVertex);
	}
}

void DTEditorUpdate(RwReal fElapsed)
{
	CDTEditorView *pEditorView = (CDTEditorView*)( ((CMainFrame*)AfxGetMainWnd())->GetActiveView() );
	pEditorView->Update(fElapsed);
}

void SetEditModeType(EEditModeType eMode)
{
	g_EditModeType = eMode;
}

EEditModeType GetEditModeType(void)
{
	return g_EditModeType;
}

void SetActiveDTProp(const CNtlDTProp *pProp)
{
	g_pActiveDTProp = const_cast<CNtlDTProp*>( pProp );
}

CNtlDTProp*	GetActiveDTProp(void) 
{
	return g_pActiveDTProp;
}

void SetSplineEditPosition(RwV3d vPos)
{
	g_vSplineCenter = vPos;
}

RwV3d GetSplineEditPosition(void)
{
	return g_vSplineCenter;
}


void SetActiveSelectControlPoint(int nSelSx, int nSelEx)
{
	g_nSelSx = nSelSx;
	g_nSelEx = nSelEx;
}

void GetActiveSelectControlPoint(int& nSelSx, int& nSelEx)
{
	nSelSx = g_nSelSx;
	nSelEx = g_nSelEx;
}

void SetFocusControlPoint(int nFocusIdx)
{
	g_nFocusIdx = nFocusIdx;
}

int	GetFocusControlPoint(void)
{
	return g_nFocusIdx;
}


void SetGridCenter(RwV3d vCenter)
{
	g_vGridCenter = vCenter;
}

RwV3d GetGridCenter(void)
{
	return g_vGridCenter;
}

void SetGridLineCx(RwInt32 iCx)
{
	g_iLineCx = iCx;

	if(g_pLineXVertex)
	{
		NTL_ARRAY_DELETE(g_pLineXVertex);
	}

	g_pLineXVertex = NTL_NEW RwIm3DVertex[g_iLineCx*2];

	MakeGridLineXVertex();
}

RwInt32 GetGridLineCx(void)
{
	return g_iLineCx;
}

void SetGridLineCy(RwInt32 iCy)
{
	g_iLineCy = iCy;

	if(g_pLineYVertex)
	{
		NTL_ARRAY_DELETE(g_pLineYVertex);
	}

	g_pLineYVertex = NTL_NEW RwIm3DVertex[g_iLineCy*2];

	MakeGridLineYVertex();
}

RwInt32 GetGridLineCy(void)
{
	return g_iLineCy;
}

RwIm3DVertex* GetLineAxisXVertex(void)
{
	return g_pAxisXVertex;
}

RwIm3DVertex* GetLineAxisZVertex(void)
{
	return g_pAxisZVertex;
}

RwIm3DVertex* GetLineXVertex(void)
{
	return g_pLineXVertex;
}

RwIm3DVertex* GetLineYVertex(void)
{
	return g_pLineYVertex;
}

void EnableChangePointPosition(RwBool bEnable)
{
	g_bChangePointPos = bEnable;
}

RwBool IsChangePointPosition(void)
{
	return g_bChangePointPos;
}

void EnableChangePointDirection(RwBool bEnable)
{
	g_bChangePointDir = bEnable;
}

RwBool IsChangePointDirection(void)
{
	return g_bChangePointDir;
}

void EnableCameraMovePointClick(RwBool bEnable)
{
	g_bCameraMovePointClick = bEnable;
}

RwBool IsCameraMovePointClick(void)
{
	return g_bCameraMovePointClick;
}


void SetSplinePointBoxSize(RwReal fSize)
{
	g_fSplineBoxSize = fSize;
}

RwReal GetSplinePointBoxSize(void)
{
	return g_fSplineBoxSize;
}

void EnableCustomDataRender(RwBool bEnable)
{
	g_bCustomDataRender = bEnable;
}

RwBool IsCustomDataRender(void)
{
	return g_bCustomDataRender;
}



RwUInt32 AcquireSerialId(void)
{
	return ++g_uiSerialId;
}

CDTServer* GetDTServer(void)
{
	return &g_DTServer;
}

void SetSelectTarget(SERIAL_HANDLE hSerialId)
{
	g_hSelHandle = hSerialId;
}

SERIAL_HANDLE GetSelectTarget(void)
{
	return g_hSelHandle;
}

void SetSplineStandardPosition(RwV3d vPos)
{
	g_vSplineStandardPos = vPos;
}

RwV3d GetSplineStandardPosition(void)
{
	return g_vSplineStandardPos;
}


void SetActiveWorldTableData(sWORLD_TBLDAT *pWorldTblData)
{
	g_pActiveWorldTable = pWorldTblData;
	GetNtlSoundMoodManager()->Condition_EnterWorld(pWorldTblData);
}

sWORLD_TBLDAT* GetActiveWorldTableData(void)
{
	return (sWORLD_TBLDAT*)g_pActiveWorldTable;
}


void SetAvatarRace(RwUInt8 byRace)
{
	g_byRace = byRace;
}

RwUInt8	GetAvatarRace(void)
{
	return g_byRace;
}

void SetAvatarClass(RwUInt8 byClass)
{
	g_byClass = byClass;
}

RwUInt8	GetAvatarClass(void)
{
	return g_byClass;
}

void SetAvatarGender(RwUInt8 byGender)
{
	g_byGender = byGender;
}

RwUInt8 GetAvatarGender(void)
{
	return g_byGender;
}

void SetActiveMonsterTableData(const sMOB_TBLDAT *pMobTblData)
{
	g_pActiveMonsterTable = pMobTblData;
}

const sMOB_TBLDAT* GetActiveMonsterTableData(void)
{
	return g_pActiveMonsterTable;
}

void SetActiveHTBTableData(const sHTB_SET_TBLDAT *pHTBSetTblData)
{
	g_pHTBSetTblData = pHTBSetTblData;
}

const sHTB_SET_TBLDAT* GetActiveHTBTableData(void)
{
	return g_pHTBSetTblData;
}

// viewer rendering

void EditViewerRendering(RwReal fElapsed)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CControlPane *pPane = pFrame->GetControlPane();

	if(pPane == NULL)
		return;

	pPane->Render(fElapsed);
}

//
void SetFreeCameraSpeed(RwReal fSpeed)
{
	g_fFreeCameraSpeed = fSpeed;
}

RwReal GetFreeCameraSpeed(void)
{
	return g_fFreeCameraSpeed;
}