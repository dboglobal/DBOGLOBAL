#include "StdAfx.h"
#include "PathHandler.h"
#include "NtlWE.h"
#include "PaletteSpawn.h"
#include "paletteSpawnPath.h"
#include "NtlPLWorldEntity.h"
#include "C2DAABB.h"
#include "NtlWorldFieldManager.h"
#include "PathList.h"
#include "PathPoint.h"
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlMath.h"
#include "NtlPLWorldEntity.h"
#include "NtlText.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlFileSerializer.h"
#include "NtlPLRenderState.h"


CPathHandler::CPathHandler(void)
{
	m_RenderOnly		= FALSE;
	m_pCurPathList		= NULL;
	m_pSelPathList		= NULL;
	m_pCurPathPoint		= NULL;
	m_pSnaPathPoint		= NULL;
	m_pDummyLineEntity	= NULL;
	m_EndPPFlow			= FALSE;
	m_vecPathList.reserve(dMAX_PATH_POINT);
}

CPathHandler::~CPathHandler(void)
{
	OnDestroy();
}

VOID CPathHandler::OnDestroy()
{
	if(m_vecPathList.size())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			CPathList* pDel = *The;
			NTL_DELETE(pDel);

			The = m_vecPathList.erase(The);
		}

		m_vecPathList.clear();
	}

	if(m_pDummyLineEntity)
	{
		GetSceneManager()->DeleteEntity(m_pDummyLineEntity);
		m_pDummyLineEntity = NULL;
	}

	m_RenderOnly		= FALSE;
	m_pCurPathList		= NULL;
	m_pSelPathList		= NULL;
	m_pCurPathPoint		= NULL;
	m_pSnaPathPoint		= NULL;
	m_pDummyLineEntity	= NULL;
	m_EndPPFlow			= FALSE;
	m_vecPathList.reserve(dMAX_PATH_POINT);
}

VOID CPathHandler::SetRenderOnlyMode(RwBool _RenderOnly/* = FALSE*/)
{
	// just return if the path interface dialog's activating
}

VOID CPathHandler::CullPathList()
{
	NTL_PRE(dGETMGR());


	CNtlWorldFieldManager*	pMgr = dGETMGR();

	C2DAABB	AABB2D;
	pMgr->GetVisible2DAABB(AABB2D);

	tVECTOR_PL_ITER The = m_vecPathList.begin();	
	while(The != m_vecPathList.end())
	{
		(*The)->Cull(&AABB2D);

		++The;
	}
}

RwBool CPathHandler::VerifyPathListUID()
{
	RwInt32 CurUID = dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetCurUID();

	tVECTOR_PL_ITER The = m_vecPathList.begin();
	while(The != m_vecPathList.end())
	{
		if((*The)->CompareUID(CurUID))
		{
			return FALSE;
		}

		++The;
	}

	return TRUE;
}

RwBool CPathHandler::OnCreateNewPathList()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	if(!VerifyPathListUID())
	{
		AfxMessageBox("The UID already exist.");
		return FALSE;
	}

	if(m_pCurPathList)
	{
		AfxMessageBox("can't create another path list during working one.");
		return FALSE;
	}

	RwInt32 CurUID		= dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetCurUID();
	RwReal	CurRadius	= dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetCurRadius();
	RwChar*	pMemo		= dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetMemo();

	CPathList* pPathList = NTL_NEW CPathList(CurUID, CurRadius, pMemo);
	m_vecPathList.push_back(pPathList);

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, static_cast<RwInt32>(m_vecPathList.size()));

	m_pCurPathList	= pPathList;
	m_pSelPathList	= pPathList;

	if(m_pCurPathPoint)
	{
		RwRGBA SelectionColor = {255, 255, 255, 255};
		m_pCurPathPoint->SetColor(SelectionColor);
		m_pCurPathPoint = NULL;
	}

	return TRUE;
}

CPathPoint* CPathHandler::OnPickPathPoint()
{
	return m_pSelPathList->Pick(dGET_MOUSE()->GetCusPos().x, dGET_MOUSE()->GetCusPos().y);
}

RwBool CPathHandler::OnAppendNewPathPoint(RwV3d& _IntersectionPt, RwBool _EndPP)
{
	if(!m_pSelPathList)
	{
		return FALSE;
	}

	VerifyPathListUID();

	CPathPoint* pPathPoint = NTL_NEW CPathPoint(_IntersectionPt);
	m_pSelPathList->Insert(pPathPoint, _EndPP);
	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, m_pSelPathList->GetPathPointSize());

	return TRUE;
}

VOID CPathHandler::OnCreateNewPathPoint(RwV3d& _IntersectionPt)
{
	CPathPoint* pPathPoint = NTL_NEW CPathPoint(_IntersectionPt);

	if(m_pSnaPathPoint)
	{
		m_pCurPathList->Insert(pPathPoint, m_EndPPFlow);
		m_pSnaPathPoint = pPathPoint;
	}
	else
	{
		m_pCurPathList->Insert(pPathPoint, TRUE);
	}

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pCurPathList, m_pCurPathList->GetPathPointSize());

	if(m_pCurPathPoint)
	{
		RwRGBA SelectionColor = {255, 255, 255, 255};
		m_pCurPathPoint->SetColor(SelectionColor);
		m_pCurPathPoint = NULL;
	}
}

VOID CPathHandler::OnCancelCurPathList()
{
	if(!m_pCurPathList)
	{
		AfxMessageBox("There's no a working path list.");
		return;
	}

	if(m_pDummyLineEntity)
	{
		GetSceneManager()->DeleteEntity(m_pDummyLineEntity);
		m_pDummyLineEntity = NULL;
	}

	m_pCurPathList	= NULL;
	m_pSnaPathPoint	= NULL;
}

VOID CPathHandler::SetCurSelPathList(RwInt32 _SelIdx)
{
	m_pSelPathList = m_vecPathList[_SelIdx];
}

VOID CPathHandler::OnDeletePathPoints(std::vector<RwInt32>& _vecIndices)
{
	NTL_ASSERTE(m_pSelPathList);

	OnClearCurVertex();

	CPathList::tLIST_PP_ITER	The;
	RwInt32						Cnt = 0;
	std::vector<CPathPoint*>	vecThes;

	for(RwUInt32 i = 0; i < _vecIndices.size(); ++i)
	{
		The = m_pSelPathList->GetBEGIN();

		for(RwInt32 j = 0; j < _vecIndices[i]; ++j)
		{
			++The;
		}

		vecThes.push_back(*The);
	}

	for(RwUInt32 i = 0; i < vecThes.size(); ++i)
	{
		The = m_pSelPathList->GetBEGIN();
		while(The != m_pSelPathList->GetEND())
		{
			if(static_cast<CPathPoint*>(*The) == vecThes[i])
			{
				m_pSelPathList->Delete(*The);
				break;
			}

			++The;
		}
	}

	vecThes.clear();

	if(!m_pSelPathList->GetPathPointSize())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			if(static_cast<CPathList*>(*The) == m_pSelPathList)
			{
				m_vecPathList.erase(The);
				m_pSelPathList = NULL;
				break;
			}

			++The;
		}
	}

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);

	if(m_pSelPathList)
	{
		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, m_pSelPathList->GetPathPointSize());
	}
	else
	{
		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, -999);
	}

	m_pCurPathPoint = NULL;
}

VOID CPathHandler::OnDeletePathPoint()
{	
	if(!m_pCurPathPoint || m_pCurPathList)
	{
		return;
	}

	m_pSelPathList->Delete(m_pCurPathPoint);
	m_pCurPathPoint = NULL;

	if(!m_pSelPathList->GetPathPointSize())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			if(static_cast<CPathList*>(*The) == m_pSelPathList)
			{
				m_vecPathList.erase(The);
				m_pSelPathList = NULL;
				break;
			}

			++The;
		}
	}

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);

	if(m_pSelPathList)
	{
		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, m_pSelPathList->GetPathPointSize());
	}
	else
	{
		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, -999);
	}
	
	m_pCurPathPoint = NULL;
}

VOID CPathHandler::OnInsertPathPoint(RwBool bBack)
{
	if(!m_pCurPathPoint || m_pCurPathList)
	{
		return;
	}

	RwV3d vPos = m_pCurPathPoint->GetPosition();

	CPathPoint* pPathPoint = NTL_NEW CPathPoint(vPos);
	m_pSelPathList->InsertN(pPathPoint, m_pCurPathPoint, bBack);

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, m_pSelPathList->GetPathPointSize());
}

VOID CPathHandler::OnDeletePathList()
{
	if(!m_pSelPathList || m_pCurPathList)
	{
		return;
	}

	tVECTOR_PL_ITER The = m_vecPathList.begin();
	while(The != m_vecPathList.end())
	{
		if(static_cast<CPathList*>(*The) == m_pSelPathList)
		{
			CPathList* pDel = (*The);
			NTL_DELETE(pDel);
			m_vecPathList.erase(The);
			break;
		}

		++The;
	}

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);

	m_pCurPathList = NULL;
	m_pSelPathList = NULL;

	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(m_pSelPathList, -999);
}

VOID CPathHandler::OnChangedRadius()
{
	if(m_pSelPathList && !m_pCurPathList)
	{
		m_pSelPathList->SetRadius(dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetCurRadius());
		
		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);
	}
}

VOID CPathHandler::OnChangedMemo()
{
	if(m_pSelPathList && !m_pCurPathList)
	{
		m_pSelPathList->SetMemo(dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->GetMemo());

		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);
	}
}

VOID CPathHandler::OnClearCurVertex()
{
	if(m_pCurPathPoint)
	{
		RwRGBA SelectionColor = {255, 255, 255, 255};
		m_pCurPathPoint->SetColor(SelectionColor);
		m_pCurPathPoint = NULL;
	}
}

RwBool CPathHandler::OnSnapVertex()
{
	if(!m_pCurPathList && m_pSelPathList && m_pSelPathList->GetPathPointSize())
	{
		OnClearCurVertex();

		SWorldPickInfo	sPickInfo;
		CPathPoint*		pPathPoint	= NULL;
		RwBool			bPick		= FALSE;

		sPickInfo.pPLEntity			= NULL;
		sPickInfo.bTerrainExclusion = FALSE;
		bPick = GetSceneManager()->PickWorld(dGET_MOUSE()->GetCusPos().x, dGET_MOUSE()->GetCusPos().y, sPickInfo, 300.0f, 300.0f);
		if(bPick)
		{
			CPathList::tLIST_PP_ITER TheS = m_pSelPathList->GetBEGIN();
			CPathList::tLIST_PP_ITER TheE = m_pSelPathList->GetEND();
			--TheE;

			RwReal DistS = 1024.1f;
			RwReal DistE = 1024.1f;

			if((*TheS)->GetVisibility())
			{
				DistS = sqrtf(CNtlMath::MathRwV3dSquaredLength(&((*TheS)->GetPosition() - sPickInfo.vPickPos)));
			}

			if((*TheE)->GetVisibility())
			{
				DistE = sqrtf(CNtlMath::MathRwV3dSquaredLength(&((*TheE)->GetPosition() - sPickInfo.vPickPos)));
			}
		
			if(DistS < DistE)
			{
				if(DistS < dGET_WORLD_PARAM()->WorldSectorTileSize)
				{
				//	OnAppendNewPathPoint(sPickInfo.vPickPos, FALSE);
					m_pCurPathList	= m_pSelPathList;
					m_pSnaPathPoint	= (*TheS);
					m_EndPPFlow		= 0;
					return TRUE;
				}
			}
			else
			{
				if(DistE < dGET_WORLD_PARAM()->WorldSectorTileSize)
				{
				//	OnAppendNewPathPoint(sPickInfo.vPickPos, TRUE);
					m_pCurPathList	= m_pSelPathList;
					m_pSnaPathPoint	= (*TheE);
					m_EndPPFlow		= 1;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

VOID CPathHandler::OnPortalTo(RwInt32 _Index)
{
	if(!m_pSelPathList)
	{
		AfxMessageBox("choose a path point plz");
		return;
	}

	CPathList::tLIST_PP_ITER The = m_pSelPathList->GetBEGIN();
	for(RwInt32 i = 0; i < _Index; ++i)
	{
		++The;
	}

	RwV3d Pos = (*The)->GetPosition();
	GetSceneManager()->GetWorld()->SetPortalPosition(Pos);
}

VOID CPathHandler::OnDeleteDummyEntities()
{
	tVECTOR_PL_ITER The = m_vecPathList.begin();
	while(The != m_vecPathList.end())
	{
		CPathList* pPathList = static_cast<CPathList*>(*The);
		pPathList->DeleteDummyEntities();
		++The;
	}
}

VOID CPathHandler::OnSave()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "Save Path files", "*.pth4tool", OFN_HIDEREADONLY, "Path Files (*.pth4tool)|*.pth4tool||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Path";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// tool
	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".pth4tool";

	FileTitle = FileTitle + FileExt;

	if(::fopen_s(&pFile, FileTitle.c_str(), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}

	unsigned int NumPathList = m_vecPathList.size();
	fwrite(&NumPathList, sizeof(unsigned int), 1, pFile);

	if(m_vecPathList.size())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			static_cast<CPathList*>(*The)->Save(pFile, TRUE);
			++The;
		}
	}

	::fclose(pFile);

	// server
	pFile		= NULL;
	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	FileExt		= ".pth";

	FileTitle = FileTitle + FileExt;

	if(::fopen_s(&pFile, FileTitle.c_str(), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}

	::SetCurrentDirectory(dirBackup);

	NumPathList = m_vecPathList.size();
	fwrite(&NumPathList, sizeof(unsigned int), 1, pFile);

	if(m_vecPathList.size())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			static_cast<CPathList*>(*The)->Save(pFile, FALSE);
			++The;
		}
	}

	::fclose(pFile);
}

VOID CPathHandler::OnLoad()
{
	if(AfxMessageBox("Current working data will be deleted. Are you sure?", MB_YESNO) != IDYES)
	{
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "Load Path files", "*.pth4tool", OFN_HIDEREADONLY, "Path Files (*.pth)|*.pth||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Path";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// Create file and ready 2 save
	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".pth4tool";

	FileTitle = FileTitle + FileExt;

	if(::fopen_s(&pFile, FileTitle.c_str(), "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}

	::SetCurrentDirectory(dirBackup);

	// clear current path works
	OnDestroy();

	// create all
	unsigned int NumPathList;
	fread(&NumPathList, sizeof(unsigned int), 1, pFile);

	TBLIDX		WorldTblIdx;
	float		fRadius;
	BYTE		ByPointCount;
	RwChar		Memo[255];

	for(unsigned int i = 0; i < NumPathList; ++i)
	{
		fread(&WorldTblIdx, sizeof(TBLIDX), 1, pFile);
		fread(&fRadius, sizeof(float), 1, pFile);
		fread(&ByPointCount, sizeof(BYTE), 1, pFile);
		fread(Memo, sizeof(RwChar) * 255, 1, pFile);

		CPathList* pPathList = NTL_NEW CPathList(WorldTblIdx, fRadius, Memo);
		pPathList->Load(pFile, ByPointCount);

		m_vecPathList.push_back(pPathList);
	}

	// update the dialogs
	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathList(this, -999);
	dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->CreatePathPoint(NULL, -999);

	::fclose(pFile);

	CullPathList();
}

VOID CPathHandler::OnExport()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "Export Path files", "*.txt", OFN_HIDEREADONLY, "Path Files (*.txt)|*.txt||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Path";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// Create file and ready 2 save
	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".txt";

	FileTitle = FileTitle + FileExt;

	if(::fopen_s(&pFile, FileTitle.c_str(), "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(m_vecPathList.size())
	{
		tVECTOR_PL_ITER The = m_vecPathList.begin();
		while(The != m_vecPathList.end())
		{
			static_cast<CPathList*>(*The)->Export(pFile);
			++The;
		}
	}

	::fclose(pFile);
}

VOID CPathHandler::Update()
{
	SWorldPickInfo	sPickInfo;
	CPathPoint*		pPathPoint	= NULL;
	RwBool			bPick		= FALSE;

	sPickInfo.pPLEntity			= NULL;
	sPickInfo.bTerrainExclusion = FALSE;

	// get the picking info.
	if(m_pCurPathList || dGET_MOUSE()->GetLBOnce() || (dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE)))
	{
		bPick = GetSceneManager()->PickWorld(dGET_MOUSE()->GetCusPos().x, dGET_MOUSE()->GetCusPos().y, sPickInfo, 300.0f, 300.0f);
		if(bPick)
		{
			if (!sPickInfo.pPLEntity || !(sPickInfo.pPLEntity->GetFlags() & NTL_PLEFLAG_TOOL_PICK))
			{
				m_Intersection = sPickInfo.vPickPos;
			}			
		}

		if(m_pSelPathList)
		{
			pPathPoint = OnPickPathPoint();
		}
	}

	// create a new path list
	if(dGET_MOUSE()->GetLBOnce() && bPick)
	{
		if(dKEY(DIK_LSHIFT))
		{
			if(OnCreateNewPathList())
			{
				OnCreateNewPathPoint(sPickInfo.vPickPos);
			}
		}
		else
		{
			if(m_pCurPathList)
			{
				OnCreateNewPathPoint(sPickInfo.vPickPos);
			}
			else if(m_pSelPathList)
			{
				if(m_pCurPathPoint)
				{
					RwRGBA SelectionColor = {255, 255, 255, 255};
					m_pCurPathPoint->SetColor(SelectionColor);
					m_pCurPathPoint = NULL;
				}

				if(pPathPoint)
				{ 
					m_pCurPathPoint = pPathPoint;

					RwRGBA SelectionColor = {0, 0, 255, 255};
					m_pCurPathPoint->SetColor(SelectionColor);
				}
				else
				{
					m_pCurPathPoint = NULL;
				}
			}
		}
	}

	// Move the current picked path point
	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		if(!m_pCurPathList && m_pCurPathPoint && bPick)
		{
			m_pCurPathPoint->SetPosition(m_Intersection);
		}
	}

	// end placing
	if(dOKEY(DIK_SPACE))
	{
		OnCancelCurPathList();
	}

	// delete the selected path point
	if(dOKEY(DIK_DELETE))
	{
		OnDeletePathPoint();
	}

	if (dOKEY(DIK_INSERT))
	{
		if (dKEY(DIK_LSHIFT))
		{
			OnInsertPathPoint(FALSE);
		}
		else
		{
			OnInsertPathPoint(TRUE);
		}
	}

	// delete the selected path list
	if(dKEY(DIK_LSHIFT) && dOKEY(DIK_DELETE))
	{
		OnDeletePathList();
	}

	// snap a vertex to the current selected path list at the start and end point only
	if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce())
	{
		OnSnapVertex();
	}

	if (dGET_MOUSE()->GetLBDC())
	{
		for (int i = 0; i < (int)m_vecPathList.size(); ++i)
		{
			if (m_vecPathList[i] != m_pSelPathList)
			{
				if (m_vecPathList[i]->Pick(dGET_MOUSE()->GetCusPos().x, dGET_MOUSE()->GetCusPos().y))
				{
					dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnPath->SelectPathList(m_vecPathList[i]->GetUID());
					m_pSelPathList = m_vecPathList[i];						
				}
			}
		}
	}
}

VOID CPathHandler::Render()
{
	if(!m_vecPathList.size())
	{
		return;
	}
	
	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwD3D9SetFVF(D3DFVF_LINE3D);

	// the current selected path line display
	tVECTOR_PL_ITER The	= m_vecPathList.begin();
	while(The != m_vecPathList.end())
	{
		if((*The)->GetVisibility())
		{
			if(m_pSelPathList == static_cast<CPathList*>(*The))
			{
				(*The)->SetLineColor(static_cast<D3DCOLOR>(0xffff0000));
			}
			else
			{
				(*The)->SetLineColor(static_cast<D3DCOLOR>(0xffffffff));
			}

			(*The)->Render();
		}

		++The;
	}
	
	// the dummy path line
	if(m_pCurPathList)
	{
		if(!m_pDummyLineEntity)
		{
			SPLEntityCreateParam PLEntityCreateParam;
			PLEntityCreateParam.pPos = &m_Intersection;	

			m_pDummyLineEntity = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "EFFECT_DUMMY", &PLEntityCreateParam);
			DBO_ASSERT(m_pDummyLineEntity, "Entity create failed.");

			m_pDummyLineEntity->SetFlags(m_pDummyLineEntity->GetFlags() & NTL_PLEFLAG_TOOL_PICK);
			
			static_cast<CNtlPLObject*>(m_pDummyLineEntity)->SetFadeEnable(FALSE);

			RwV3d Scl = { 1.0f, 1.0f, 1.0f };
			RwV3d Rot = { 0.0f, 0.0f, 0.0f };
			RwV3d Pos = m_Intersection;

			static_cast<CNtlPLObject*>(m_pDummyLineEntity)->SetScale(&Scl);
			static_cast<CNtlPLObject*>(m_pDummyLineEntity)->SetRotate(Rot.x, Rot.y, Rot.z);
		}

		static_cast<CNtlPLObject*>(m_pDummyLineEntity)->SetPosition(&m_Intersection);

		sLINE3D	PathLine[2];

		PathLine[0].Col = 0xff00ff00;
		PathLine[1].Col = 0xff00ff00;

		if(m_pSnaPathPoint)
		{
			PathLine[0].Pos.x = m_pSnaPathPoint->GetPosition().x;
			PathLine[0].Pos.y = m_pSnaPathPoint->GetPosition().y + 1.0f;
			PathLine[0].Pos.z = m_pSnaPathPoint->GetPosition().z;
		}
		else
		{
			CPathList::tLIST_PP_ITER The = m_pCurPathList->GetEND(); --The;
			PathLine[0].Pos.x = (*The)->GetPosition().x;
			PathLine[0].Pos.y = (*The)->GetPosition().y + 1.0f;
			PathLine[0].Pos.z = (*The)->GetPosition().z;
		}

		PathLine[1].Pos.x = m_Intersection.x;
		PathLine[1].Pos.y = m_Intersection.y + 1.0f;
		PathLine[1].Pos.z = m_Intersection.z;

		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, PathLine, sizeof(sLINE3D));

		RwChar	acTemp[256];
		RwV3d	vPathPos[2];
		vPathPos[0].x = PathLine[0].Pos.x; vPathPos[0].y = PathLine[0].Pos.y; vPathPos[0].z = PathLine[0].Pos.z;
		vPathPos[1].x = PathLine[1].Pos.x; vPathPos[1].y = PathLine[1].Pos.y; vPathPos[1].z = PathLine[1].Pos.z;
		sprintf_s(acTemp, 256, "%.2f", RwV3dLength(&(vPathPos[1] - vPathPos[0])));
		dGETAPP().RenderText(vPathPos[0], acTemp, 255, 255, 0);		
	}

	if (m_pSelPathList && m_pCurPathPoint)
	{
		CPathPoint* pPre	= NULL;
		CPathPoint* pNext	= NULL;

		CPathPoint*	pCur	= NULL;

		CPathList::tLIST_PP_ITER itPP = m_pSelPathList->GetBEGIN();
		while(itPP != m_pSelPathList->GetEND())
		{
			CPathPoint* pPathPoint = *itPP;

			if (pPathPoint == m_pCurPathPoint)
			{
				pCur = pPathPoint; 
			}
			else if (pCur == NULL)
			{
				pPre = pPathPoint;
			}
			else if (pCur)
			{
				pNext = pPathPoint;
				break;
			}
			++itPP;
		}

		if (pPre && pCur)
		{
			RwChar acTemp[256];			
			sprintf_s(acTemp, 256, "%.2f", RwV3dLength(&(pPre->GetPosition() - pCur->GetPosition())));
			dGETAPP().RenderText(pPre->GetPosition(), acTemp, 255, 255, 0);			
		}
		if (pCur && pNext)
		{
			RwChar acTemp[256];			
			sprintf_s(acTemp, 256, "%.2f", RwV3dLength(&(pCur->GetPosition() - pNext->GetPosition())));
			dGETAPP().RenderText(pNext->GetPosition(), acTemp, 255, 255, 0);			
		}
	}

	EndD3DLineState();
}

VOID CPathHandler::RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	RwChar Caption[128];

	if(dGET_CUR_PALETTE() == ePM_SPAWN && dGET_WORLD_PARAM()->FlgObjInfoShow && m_pSelPathList)
	{
		if(m_pSelPathList->GetVisibility())
		{
			CPathPoint* pPP = (*m_pSelPathList->GetBEGIN());
			if(pPP->GetVisibility())
			{
				RwV3d	v2dPoint;
				RwV3d	v3dPoint = pPP->GetPosition();

				RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
				RwV3dTransformPoints(&v2dPoint, &v3dPoint, 1, pMatView);
				v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
				v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight;

				RwRGBA ClrFont;
				ClrFont.red		= 255;
				ClrFont.green	= 255;
				ClrFont.blue	= 255;
				RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
				RsSprintf(Caption, RWSTRING("%d : %.2f, %.2f, %.2f"), m_pSelPathList->GetUID(), v3dPoint.x, v3dPoint.y, v3dPoint.z);
				RsCharsetPrint(_pCharset, Caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
			}
		}
	}
}
