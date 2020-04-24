#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldInterface.h"

#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWeControlUi.h"

#include "NtlPLWorldEntity.h"

#include "NtlWorldSectorInfo.h"

#include "NtlWorldPaletteDefine.h"

#include "CameraAttrDlg.h"
#include "PathMeshFindDlg.h"

#include "ProgressWnd.h"
#include "NtlWEUtil.h"

#include "NtlConvexHull.h"

#include "NtlWorldPathEngineManager.h"

#include "NtlPLObjectProperty.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

IMPLEMENT_DYNAMIC(CPathMeshFindDlg, CDialog)

CPathMeshFindDlg::CPathMeshFindDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPathMeshFindDlg::IDD, pParent)
//,m_pRwTexture(NULL)
,m_pRwImage(NULL)
{
}

CPathMeshFindDlg::~CPathMeshFindDlg()
{
// 	if (m_pRwTexture)
// 	{
// 		GetNtlResourceManager()->UnLoadTexture(m_pRwTexture);
// 		m_pRwTexture = NULL;
// 	}
	if (m_pRwImage)
	{
		RwImageDestroy(m_pRwImage);
		m_pRwImage = NULL;
	}

}

void CPathMeshFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_LIST, m_listSearch);
	DDX_Control(pDX, IDC_CHECK_NAVI_MESH, m_BtnNaviMesh);
	DDX_Control(pDX, IDC_CHECK_OBS_MESH, m_BtnObsMesh);
}


BEGIN_MESSAGE_MAP(CPathMeshFindDlg, CDialog)
	//ON_BN_CLICKED(IDC_BTN_SEARCH, &CPathMeshFindDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_PORTAL, &CPathMeshFindDlg::OnBnClickedBtnPortal)
	ON_BN_CLICKED(IDC_BTN_SEARCH_ROT, &CPathMeshFindDlg::OnBnClickedBtnSearchRot)
	ON_BN_CLICKED(IDC_BTN_SEARCH_AREA, &CPathMeshFindDlg::OnBnClickedBtnSearchArea)
	ON_BN_CLICKED(IDC_BTN_OBS_EXCEPTION, &CPathMeshFindDlg::OnBnClickedBtnObsException)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CPathMeshFindDlg::OnBnClickedBtnImport)
END_MESSAGE_MAP()


// CFieldSearchDlg 메시지 처리기입니다.

BOOL CPathMeshFindDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

#include <fstream>

BOOL CPathMeshFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listSearch.InsertColumn(ESEARCHLIST_NAME, "Name", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_X, "Pos.X", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_Y, "Pos.Y", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_Z, "Pos.Z", LVCFMT_CENTER, 80);
	m_listSearch.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	char acPath[NTL_MAX_DIR_PATH];
	char acBuffer[NTL_MAX_DIR_PATH];

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acPath);
	// Exception Name
	std::ifstream ifs;
	ifs.open("Tool\\Saber\\Script\\NaviMeshFindExc.txt");
	if (ifs)
	{
		while(!ifs.eof())
		{
			ifs.getline(acBuffer, NTL_MAX_DIR_PATH);
			m_vecExceptionList.push_back(acBuffer);
		}
		ifs.close();
	}
	SetCurrentDirectory(acPath);
	
	return TRUE;
}

RwBool CPathMeshFindDlg::OnSortData(RwInt32 _Index)
{
	CListCtrl*	pLc			= (CListCtrl*)GetDlgItem(IDC_SEARCH_LIST);
	RwInt32		totItems	= pLc->GetItemCount();

	// Callback Item Setting
	for(RwInt32 i = 0; i < totItems; i++)
	{
		DWORD	dw	= pLc->GetItemData(i);
		CString txt = pLc->GetItemText(i, _Index);

		pLc->SetItemData(i, (DWORD)(new CSortData(txt, dw)));
	}

	if(_Index == 0)
	{
		pLc->SortItems(StringCompare, m_AscendingSortFlags[_Index]);
	}
	else
	{
		pLc->SortItems(NumberCompare, m_AscendingSortFlags[_Index]);
	}

	for(RwInt32 i = 0; i < totItems; ++i)
	{
		CSortData* pItem = (CSortData*)(pLc->GetItemData(i));
		ASSERT(pItem);

		pLc->SetItemData(i, pItem->m_dw);

		delete pItem;
	}

	return TRUE;
}

RwBool CPathMeshFindDlg::IsInString(const char *pSrc, const char *pCmp)
{

	CString strSrc = pSrc;
	CString strCmp = pCmp;

	RwInt32 iLenSrc = strSrc.GetLength();
	RwInt32 iLenCmp = strCmp.GetLength();

	strSrc.MakeUpper();
	strCmp.MakeUpper();

	RwInt32 iPos = strSrc.Find(strCmp.GetBuffer());
	while (iPos != -1 && iPos <= iLenSrc - iLenCmp)
	{
		if (strSrc.Mid(iPos, iLenCmp) == strCmp)
		{
			return TRUE;
		}
		iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
	}

	return FALSE;
}

void CPathMeshFindDlg::AddSearchList(char* pcName, RwV3d* pvPos)
{
	CString strData[ESEARCHLIST_NUM];

	strData[ESEARCHLIST_NAME].Format("%s", pcName);
	strData[ESEARCHLIST_POS_X].Format("%.2f", pvPos->x);
	strData[ESEARCHLIST_POS_Y].Format("%.2f", pvPos->y);
	strData[ESEARCHLIST_POS_Z].Format("%.2f", pvPos->z);

	LVITEM	lvi;
	lvi.mask		= LVIF_TEXT;
	lvi.iItem		= m_listSearch.GetItemCount();
	lvi.iImage		= 0;
	for (int i = 0; i < ESEARCHLIST_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(strData[i].GetBuffer());
		if (i == ESEARCHLIST_NAME)
		{
			m_listSearch.InsertItem(&lvi);
		}
		else
		{
			m_listSearch.SetItem(&lvi);			
		}
	}
}

void CPathMeshFindDlg::OnBnClickedBtnPortal()
{
	RwInt32 iItem = m_listSearch.GetSelectionMark();

	if (iItem != -1)
	{
		RwV3d vPortal;
		vPortal.x = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_X).GetBuffer()));
		vPortal.y = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_Y).GetBuffer()));
		vPortal.z = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_Z).GetBuffer()));

		GetSceneManager()->GetWorld()->SetPortalPosition(vPortal);

		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

		((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteOrnament();
		dGETAPP().Update(0.0f, 0.0f);

		RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
		if (iSectorIdx)
		{
			CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
			for (int i = 0; i < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
			{
				if (m_listSearch.GetItemText(iItem, ESEARCHLIST_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName())
				{
					if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityOrnament[i]->GetPosition())) < 0.01f)
					{
						dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
					}
				}
			}
		}
	}
}

void CPathMeshFindDlg::OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(!OnSortData(pNMLV->iSubItem))
		return;

	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CPathMeshFindDlg::GetWorldObject(RwInt32 iFieldIdx, _CBOBJECT cbObject, void* pData)
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", iFieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	CFileMem	FileMem;
	BYTE*		pFileMemPos = NULL;

	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	pFileMemPos = FileMem.GetDat();
	//pFileMemPos = CNtlWorldFileMemAccessor::GetSPosOfMaterial(FileMem.GetDat());
	for (int iSectorCount = 0; iSectorCount < 4; ++iSectorCount)
	{
		pFileMemPos = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemPos);

		RwInt32	iNum;
		RwChar	cName[128];
		RwV3d	vPos, vScale, vRot;

		CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
		pFileMemPos += sizeof(RwInt32);
		for (int i = 0; i < iNum; ++i)
		{
			// Obj. Name
			CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
			pFileMemPos += sizeof(RwChar) * 128;

			// Scaling
			CopyMemory(&vScale, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			// Rotation
			CopyMemory(&vRot, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			// Translation
			CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			RwUInt32 uiObjectType;
			CopyMemory(&uiObjectType, pFileMemPos, sizeof(RwUInt32));
			pFileMemPos += sizeof(RwUInt32);

			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				// Object Propperty Range
				RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
					(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
					sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
					(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

				pFileMemPos += iPropertyRange;
			}

			// Trigger Obj Serial Id
			pFileMemPos += sizeof(RwUInt32);
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			// Object serial Id
			pFileMemPos += sizeof(RwUInt32);
#endif

			// Obj. Name Index
			pFileMemPos += sizeof(DWORD);

			// Visibility Distance
			pFileMemPos += sizeof(RwReal);

			// enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
			pFileMemPos += (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool));

			// Project Shadow
			RwInt32 iPSAtomicCnt = 0;
			CopyMemory(&iPSAtomicCnt, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);
			for (RwInt32 j = 0; j < iPSAtomicCnt; ++j)
			{
				// Map name
				pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				pFileMemPos += (sizeof(RwReal));

				// Light matrices
				pFileMemPos += (sizeof(RwMatrix));

				// Map size
				RwInt32 iSize = 0;
				CopyMemory(&iSize, pFileMemPos, sizeof(RwInt32));
				pFileMemPos += (sizeof(RwInt32));

				// Map
				pFileMemPos += (sizeof(DWORD) * iSize * iSize);
			}

			// milepost
			RwBool bMilePost;
			CopyMemory(&bMilePost, pFileMemPos, sizeof(RwBool));
			pFileMemPos += sizeof(RwBool);

			if(bMilePost)
			{
				pFileMemPos += sizeof(RwUInt32);
			}

			(this->*cbObject)(cName, &vScale, &vRot, &vPos, pData); 
		}

		pFileMemPos = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemPos);
	}

	FileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CPathMeshFindDlg::OnBnClickedBtnSearchRot()
{
	m_listSearch.DeleteAllItems();

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::SearchRot, NULL);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();
	}
}

void CPathMeshFindDlg::OnBnClickedBtnSearchArea()
{
	m_listSearch.DeleteAllItems();

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);

	char acTemp[256];
	GetDlgItemText(IDC_EDIT_SIZE, acTemp, 256);
	RwReal fSize = static_cast<RwReal>(::atof(acTemp));
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::SearchArea, &fSize);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();		
	}
}

void CPathMeshFindDlg::OnBnClickedBtnObsException()
{
	if (!CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		AfxMessageBox("Sorry... Use PathEngineMode.");
		GetDlgItem(IDC_BTN_OBS_EXCEPTION)->EnableWindow(FALSE);
		return;
	}

	std::map<RwInt32, RwReal*> mapPE;

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::ObsException, &mapPE);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();				
	}

	while (!mapPE.empty())
	{
		RwInt32 iIdx	= mapPE.begin()->first;
		RwReal* pList	= mapPE.begin()->second;
		CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(iIdx, pList);
		mapPE.erase(mapPE.begin());
		NTL_ARRAY_DELETE(pList);
	}

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->SetAnotherField();
}

void CPathMeshFindDlg::ObsException(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	CNtlPLObjectProperty* pProperty = (CNtlPLObjectProperty*)CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_OBJECT, pName);
	if (pProperty == NULL/* || !pProperty->IsNaviPress()*/)
	{
		return;
	}

	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	if (vecObsMesh.empty())
	{
		return;
	}

	for (int i = 0; i < (int)vecObsMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecObsMesh.at(i).c_str());

		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				std::vector<RwV3d> vecConvexHull;
				BuildClmupConvexHull(pRpClump, vecConvexHull);

				GenObsException(*((std::map<RwInt32, RwReal*>*)pData), vecConvexHull);
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}	
}

void CPathMeshFindDlg::GenObsException(std::map<RwInt32, RwReal*>& mapPE, std::vector<RwV3d>& vecConvexHull)
{
	RwBBox rwBBox;
	RwBBoxCalculate(&rwBBox, &vecConvexHull.front(), (RwInt32)vecConvexHull.size());

	CNtlWorldFieldManager*		pWFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager* pPEMgr = CNtlWorldPathEngineManager::GetInstance();

	rwBBox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.inf.x + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.inf.z + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.sup.x + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.sup.z + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);

	for (RwReal z = rwBBox.inf.z; z <= rwBBox.sup.z; z += pPEMgr->GetChunkSize())
	{
		for (RwReal x = rwBBox.inf.x; x <= rwBBox.sup.x; x += pPEMgr->GetChunkSize())
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.y = 0.0f;
			vPos.z = z;
			if (IsInConvexHull(vPos, vecConvexHull))
			{
				SetPathEngineTileProperty(vPos, RwRealMAXVAL, mapPE);
			}
		}
	}
}

void CPathMeshFindDlg::SetPathEngineTileProperty(RwV3d& vPos, RwReal fHeight, std::map<RwInt32, RwReal*>& mapPE)
{
	CNtlWorldFieldManager*		pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager*	pPEMgr		= CNtlWorldPathEngineManager::GetInstance();
	RwInt32						iSectorIdx	= pWFMgr->GetSectorIdx(vPos);

	if (iSectorIdx == -1)
	{
		return;
	}

	RwReal*									pHeightList	= NULL;
	std::map<RwInt32, RwReal*>::iterator	it			= mapPE.find(iSectorIdx);
	if (it == mapPE.end())
	{
		pHeightList = NTL_NEW RwReal [pPEMgr->GetChunkNum() * pPEMgr->GetChunkNum()];
		pPEMgr->LoadSwapFile(iSectorIdx, pHeightList);
		mapPE.insert(std::map<RwInt32, RwReal*>::value_type(iSectorIdx, pHeightList));
		pPEMgr->LoadSwapFile(iSectorIdx, pHeightList);
	}
	else
	{
		pHeightList = it->second;
	}

	RwV3d	vPosS;
	vPosS.x = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
	vPosS.y = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vPosS.z = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

	RwInt32	iMoveCntX	= static_cast<RwInt32>(abs(vPos.x - vPosS.x)) / pPEMgr->GetChunkSize();
	RwInt32	iMoveCntZ	= static_cast<RwInt32>(abs(vPos.z - vPosS.z)) / pPEMgr->GetChunkSize();
	RwInt32 iDataPos	= iMoveCntX + (iMoveCntZ * pPEMgr->GetChunkNum());

	pHeightList[iDataPos] = fHeight;
}

RwBool CPathMeshFindDlg::IsInConvexHull(RwV3d& vPos, std::vector<RwV3d>& vecConvexHull)
{
	for (int i = 0; i < (int)vecConvexHull.size(); ++i)
	{
		RwV3d vP1	= vecConvexHull.at(i);
		RwV3d vP2	= vecConvexHull.at((i + 1 == (int)vecConvexHull.size() ? 0 : i + 1));
		RwV3d vP	= vP2 - vP1;

		RwV3d vN;
		RwV3dCrossProduct(&vN, &vP, &CNtlPLGlobal::m_vYAxisV3);		
		RwV3dNormalize(&vN, &vN);

		RwReal fDist	= RwV3dDotProduct(&vN, &vPos);
		RwReal fD		= - vN.x * vP1.x - vN.y * vP1.y - vN.z * vP1.z;
		
		if (fDist + fD >= 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CPathMeshFindDlg::BuildClmupConvexHull(RpClump* pRpClump, std::vector<RwV3d>& vecResult)
{
	CNtlConvexHull::vecdef_POS_LIST vecPosList;
	RwLLLink*						pCur	= rwLinkListGetFirstLLLink(&pRpClump->atomicList);
	RwLLLink*						pEnd	= rwLinkListGetTerminator(&pRpClump->atomicList);
	RwLLLink*						pNext	= NULL;

	while(pCur != pEnd)
	{
		RpAtomic*		pCurAtomic		= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
		RpGeometry*		pCurGeometry	= RpAtomicGetGeometry(pCurAtomic);	
		RpMorphTarget*	pCurMorphTarget	= RpGeometryGetMorphTarget(pCurGeometry, 0);
		RpTriangle*		pCurTriangle	= RpGeometryGetTriangles(pCurGeometry);
		RwV3d*			pCurVertices	= RpMorphTargetGetVertices(pCurMorphTarget);
		RwMatrix*		pRwMatrix		= RwFrameGetLTM(RpAtomicGetFrame(pCurAtomic));

		for (int i = 0; i < RpGeometryGetNumTriangles(pCurGeometry); ++i)
		{
			CNtlConvexHull::pairdef_POS pairPos;
			RwV3d						vTemp;

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[0]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[1]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[2]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);
		}		

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	CNtlConvexHull ntlConvexHull(vecPosList); ntlConvexHull.BuildHull();
	CNtlConvexHull::pairdef_POS* pPairPos = ntlConvexHull.BeginHull();
	while (pPairPos)
	{
		RwV3d vTemp;
		vTemp.x = pPairPos->first;
		vTemp.y = 0.0f;
		vTemp.z = pPairPos->second;

		vecResult.push_back(vTemp);
		pPairPos = ntlConvexHull.NextHull();
	}
}

void CPathMeshFindDlg::SearchRot(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	if (m_BtnNaviMesh.GetCheck() && !vecNaviMesh.empty())
	{
		if (pvRot->x > 0.0f || pvRot->z > 0.0f ||
			pvRot->x < 0.0f || pvRot->z < 0.0f)
		{
			AddSearchList(pName, pvPos);
			return;
		}
	}
	if (m_BtnObsMesh.GetCheck() && !vecObsMesh.empty())
	{
		/*for (int i = 0; i < (int)m_vecExceptionList.size(); ++i)
		{
			if (IsInString(pName, m_vecExceptionList.at(i).c_str()))
			{
				return;
			}
		}*/
		if (pvRot->x > 0.0f || pvRot->z > 0.0f ||
			pvRot->x < 0.0f || pvRot->z < 0.0f)
		{
			AddSearchList(pName, pvPos);
			return;
		}
	}
}

void CPathMeshFindDlg::SearchArea(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	RwReal fSize = *((RwReal*)pData);

	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	for (int i = 0; i < (int)vecNaviMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecNaviMesh.at(i).c_str());
		
		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				RwSphere rwSphere;
				CalcClmupBoundingSphere(pRpClump, &rwSphere);

				RwV3d	vTemp = rwSphere.center;
				vTemp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.x = static_cast<RwReal>(static_cast<RwInt32>(vTemp.x / fSize) * fSize);
				vTemp.z = static_cast<RwReal>(static_cast<RwInt32>(vTemp.z / fSize) * fSize);
				vTemp.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

				if (rwSphere.center.x < vTemp.x + rwSphere.radius ||
					rwSphere.center.z < vTemp.z + rwSphere.radius ||
					rwSphere.center.x > vTemp.x + fSize - rwSphere.radius ||
					rwSphere.center.z > vTemp.z + fSize - rwSphere.radius)
				{
					AddSearchList(pName, pvPos);
				}
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}
	for (int i = 0; i < (int)vecObsMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecObsMesh.at(i).c_str());

		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				RwSphere rwSphere;
				CalcClmupBoundingSphere(pRpClump, &rwSphere);

				RwV3d	vTemp = rwSphere.center;
				vTemp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.x = static_cast<RwReal>(static_cast<RwInt32>(vTemp.x / fSize) * fSize);
				vTemp.z = static_cast<RwReal>(static_cast<RwInt32>(vTemp.z / fSize) * fSize);
				vTemp.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

				if (rwSphere.center.x < vTemp.x + rwSphere.radius ||
					rwSphere.center.z < vTemp.z + rwSphere.radius ||
					rwSphere.center.x > vTemp.x + fSize - rwSphere.radius ||
					rwSphere.center.z > vTemp.z + fSize - rwSphere.radius)
				{
					AddSearchList(pName, pvPos);
				}
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}
}

void CPathMeshFindDlg::SetClmupTransform(RpClump* pRpClump, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos)
{
	RwFrame*		pRwFrame	= RpClumpGetFrame(pRpClump); 
	RwMatrix*		pRwMatrix	= RwFrameGetMatrix(pRwFrame);

	RwMatrix		rwMatrix;
	RwV3d			vFront, vLeft, vUp;

	RwMatrixRotate(&rwMatrix, &CNtlPLGlobal::m_vYAxisV3, pvRot->y, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &CNtlPLGlobal::m_vZAxisV3, &rwMatrix);
	RwV3dTransformVector(&vLeft, &CNtlPLGlobal::m_vXAxisV3, &rwMatrix);

	RwMatrixRotate(&rwMatrix, &vLeft, pvRot->x, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &vFront, &rwMatrix);
	RwV3dTransformVector(&vUp, &CNtlPLGlobal::m_vYAxisV3, &rwMatrix);

	RwMatrixScale(pRwMatrix, pvScale, rwCOMBINEREPLACE);
	RwMatrixRotate(pRwMatrix, &CNtlPLGlobal::m_vYAxisV3, pvRot->y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pRwMatrix, &vLeft, pvRot->x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pRwMatrix, &vFront, pvRot->z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(pRwMatrix, pvPos, rwCOMBINEPOSTCONCAT);

	RwFrameUpdateObjects(pRwFrame);
}

void CPathMeshFindDlg::CalcClmupBoundingSphere(RpClump* pRpClump, RwSphere* pRwSphere)
{
	RwSphere*	pCurSphere;
	RwLLLink*	pCur;
	RwLLLink*	pEnd;
	RwLLLink*	pNext;
	RpAtomic*	pCurAtomic;
	RwInt32		NumSphere = 0;
	RpAtomic*	pAtomicOfMaxRadius;
	RwReal		LenMaxRadius;

	pRwSphere->center.x	= 0.0f;
	pRwSphere->center.y	= 0.0f;
	pRwSphere->center.z	= 0.0f;
	pRwSphere->radius	= 0.0f;

	pCur = rwLinkListGetFirstLLLink(&pRpClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pRpClump->atomicList);

	while(pCur != pEnd)
	{
		pCurAtomic	= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
		pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pCurAtomic));

		if(!NumSphere)
		{
			LenMaxRadius		= pCurSphere->radius;
			pAtomicOfMaxRadius	= pCurAtomic;

			CNtlMath::MathRwV3dAssign(&pRwSphere->center, pCurSphere->center.x, pCurSphere->center.y, pCurSphere->center.z);
		}
		else
		{
			if(LenMaxRadius < pCurSphere->radius)
			{
				LenMaxRadius		= pCurSphere->radius;
				pAtomicOfMaxRadius	= pCurAtomic;
			}

			pRwSphere->center.x += pCurSphere->center.x;
			pRwSphere->center.y += pCurSphere->center.y;
			pRwSphere->center.z += pCurSphere->center.z;
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;

		NumSphere++;
	}

	pRwSphere->center.x /= NumSphere;
	pRwSphere->center.y /= NumSphere;
	pRwSphere->center.z /= NumSphere;
	pRwSphere->radius	= LenMaxRadius + sqrtf(
		(pRwSphere->center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) * 
		(pRwSphere->center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) + 
		(pRwSphere->center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) * 
		(pRwSphere->center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) + 
		(pRwSphere->center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z) * 
		(pRwSphere->center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z)
		);
}

void CPathMeshFindDlg::GetPathEngineMeshName(char* pName, std::vector<std::string>& vecNaviMesh, std::vector<std::string>& vecObsMesh)
{
	WIN32_FIND_DATA findData;
	HANDLE			hfindFile;

	// NaviMesh Find
	RwChar acNaviFind[NTL_MAX_DIR_PATH];
	sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
		dGET_WORLD_PARAM()->CurWorkingFolderName, pName);

	hfindFile = ::FindFirstFile(acNaviFind, &findData);
	if(hfindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.')
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					continue;
				}
				else
				{
					vecNaviMesh.push_back(findData.cFileName);
				}
			}
		} while(::FindNextFile(hfindFile, &findData));

		::FindClose(hfindFile);
	}

	RwChar acObstFind[NTL_MAX_DIR_PATH];
	sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
		dGET_WORLD_PARAM()->CurWorkingFolderName, pName);

	hfindFile = ::FindFirstFile(acObstFind, &findData);
	if(hfindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.')
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					continue;
				}
				else
				{								
					vecObsMesh.push_back(findData.cFileName);
				}
			}
		} while(::FindNextFile(hfindFile, &findData));

		::FindClose(hfindFile);
	}
}

void CPathMeshFindDlg::OnBnClickedBtnImport()
{
	if (!CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		AfxMessageBox("Sorry... Use PathEngineMode.");
		GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
		return;
	}

	CFileDialog	FileDlg(TRUE,
		"select Import texture",
		"*.png; *.bmp;",
		OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		"Data Files (*.png;*.bmp;)|*.png;*.bmp||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\Grid Map";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		return;		
	}

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
	{
		return;
	}

	CProgressWnd	WndProgress(this, "Import Image");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum);

	m_pRwImage = RwImageRead(FileDlg.GetPathName().GetBuffer());

	std::map<RwInt32, RwReal*> mapPE;

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d sectors Searching", i, dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum);
		WndProgress.SetText(acText);

		GenPathEngineTilePropertyFromImage(i, m_pRwImage, mapPE);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();	
	}

	while (!mapPE.empty())
	{
		RwInt32 iIdx	= mapPE.begin()->first;
		RwReal* pList	= mapPE.begin()->second;
		CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(iIdx, pList);
		mapPE.erase(mapPE.begin());
		NTL_ARRAY_DELETE(pList);
	}

	if (m_pRwImage)
	{
		RwImageDestroy(m_pRwImage);
		m_pRwImage = NULL;
	}

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->SetAnotherField();
}

void CPathMeshFindDlg::GenPathEngineTilePropertyFromImage(RwInt32 iSectorIdx, RwImage* pRwImage, std::map<RwInt32,RwReal*> &mapPE)
{
	if (pRwImage == NULL || iSectorIdx == -1)
	{
		return;
	}
	CNtlWorldFieldManager*			pWFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager*		pPEMgr = CNtlWorldPathEngineManager::GetInstance();
	// SECTOR
	RwV3d vCurPos;
	vCurPos.x = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x;
	vCurPos.y = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vCurPos.z = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z;

	// TEXTURE SIZE
	//RwInt32 iTexWidth	= RwRasterGetWidth(pRwTexture->raster);
	//RwInt32 iTexHeight	= RwRasterGetHeight(pRwTexture->raster);
	RwInt32 iTexWidth	= RwImageGetWidth(pRwImage);
	RwInt32 iTexHeight	= RwImageGetHeight(pRwImage);

	// TEXTURE LOCK
	//DWORD *pBits = (DWORD*)RwRasterLock(pRwTexture->raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKREAD);

	DWORD *pBits = (DWORD*)RwImageGetPixels(pRwImage);

	// SECTOR VLIST
	RwV3d* pvList = NTL_NEW RwV3d [dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum];
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetPosVertList(vCurPos, pvList);

	for (RwInt32 z = static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2));
		z < static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z + (dGET_WORLD_PARAM()->WorldSectorSize / 2));
		z += pPEMgr->GetChunkSize())
	{
		RwReal fDeltaZ = 1.0f - (static_cast<RwReal>(z + dGET_WORLD_PARAM()->WorldSizeHalf) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
		for (RwInt32 x = static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2));
			x < static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x + (dGET_WORLD_PARAM()->WorldSectorSize / 2));
			x += pPEMgr->GetChunkSize())
		{
			RwReal fDeltaX = 1.0f - (static_cast<RwReal>(x + dGET_WORLD_PARAM()->WorldSizeHalf) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
			
			// TEXTURE COLOR
			DWORD *pDest	= pBits + iTexWidth * static_cast<RwInt32>(static_cast<RwReal>(iTexHeight - 1) * fDeltaZ) +	static_cast<RwInt32>(static_cast<RwReal>(iTexWidth - 1) * fDeltaX);
			DWORD ARGB		= *pDest;
			DWORD A			= (DWORD)(((*pDest) & 0xff000000) >> 24);
			DWORD R			= (DWORD)(((*pDest) & 0x00ff0000) >> 16);
			DWORD G			= (DWORD)(((*pDest) & 0x0000ff00) >> 8);
			DWORD B			= (DWORD)(((*pDest) & 0x000000ff) >> 0);

			vCurPos.x = static_cast<RwReal>(x);
			vCurPos.y = -999.0f;
			vCurPos.z = static_cast<RwReal>(z);
			if (R == 255 && G == 255 && B == 255)
			{
				vCurPos.y = RpNtlWorldSectorGetHeight(&pWFMgr->m_pSectors[iSectorIdx], &vCurPos, pvList);
			}

			if (vCurPos.y == -999.0f)
			{
				SetPathEngineTileProperty(vCurPos, RwRealMAXVAL, mapPE);
			}
			else
			{
				SetPathEngineTileProperty(vCurPos, vCurPos.y, mapPE);				
			}
		}
	}

	// TEXTURE UNLOCK
	//RwRasterUnlock(pRwTexture->raster);	
	NTL_ARRAY_DELETE(pvList);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

IMPLEMENT_DYNAMIC(CPathMeshFindDlg, CDialog)

CPathMeshFindDlg::CPathMeshFindDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPathMeshFindDlg::IDD, pParent)
//,m_pRwTexture(NULL)
,m_pRwImage(NULL)
{
}

CPathMeshFindDlg::~CPathMeshFindDlg()
{
	// 	if (m_pRwTexture)
	// 	{
	// 		GetNtlResourceManager()->UnLoadTexture(m_pRwTexture);
	// 		m_pRwTexture = NULL;
	// 	}
	if (m_pRwImage)
	{
		RwImageDestroy(m_pRwImage);
		m_pRwImage = NULL;
	}

}

void CPathMeshFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_LIST, m_listSearch);
	DDX_Control(pDX, IDC_CHECK_NAVI_MESH, m_BtnNaviMesh);
	DDX_Control(pDX, IDC_CHECK_OBS_MESH, m_BtnObsMesh);
}


BEGIN_MESSAGE_MAP(CPathMeshFindDlg, CDialog)
	//ON_BN_CLICKED(IDC_BTN_SEARCH, &CPathMeshFindDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_PORTAL, &CPathMeshFindDlg::OnBnClickedBtnPortal)
	ON_BN_CLICKED(IDC_BTN_SEARCH_ROT, &CPathMeshFindDlg::OnBnClickedBtnSearchRot)
	ON_BN_CLICKED(IDC_BTN_SEARCH_AREA, &CPathMeshFindDlg::OnBnClickedBtnSearchArea)
	ON_BN_CLICKED(IDC_BTN_OBS_EXCEPTION, &CPathMeshFindDlg::OnBnClickedBtnObsException)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CPathMeshFindDlg::OnBnClickedBtnImport)
END_MESSAGE_MAP()


// CFieldSearchDlg 메시지 처리기입니다.

BOOL CPathMeshFindDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

#include <fstream>

BOOL CPathMeshFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listSearch.InsertColumn(ESEARCHLIST_NAME, "Name", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_X, "Pos.X", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_Y, "Pos.Y", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_POS_Z, "Pos.Z", LVCFMT_CENTER, 80);
	m_listSearch.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	char acPath[NTL_MAX_DIR_PATH];
	char acBuffer[NTL_MAX_DIR_PATH];

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acPath);
	// Exception Name
	std::ifstream ifs;
	ifs.open("Tool\\Saber\\Script\\NaviMeshFindExc.txt");
	if (ifs)
	{
		while(!ifs.eof())
		{
			ifs.getline(acBuffer, NTL_MAX_DIR_PATH);
			m_vecExceptionList.push_back(acBuffer);
		}
		ifs.close();
	}
	SetCurrentDirectory(acPath);

	return TRUE;
}

RwBool CPathMeshFindDlg::OnSortData(RwInt32 _Index)
{
	CListCtrl*	pLc			= (CListCtrl*)GetDlgItem(IDC_SEARCH_LIST);
	RwInt32		totItems	= pLc->GetItemCount();

	// Callback Item Setting
	for(RwInt32 i = 0; i < totItems; i++)
	{
		DWORD	dw	= pLc->GetItemData(i);
		CString txt = pLc->GetItemText(i, _Index);

		pLc->SetItemData(i, (DWORD)(new CSortData(txt, dw)));
	}

	if(_Index == 0)
	{
		pLc->SortItems(StringCompare, m_AscendingSortFlags[_Index]);
	}
	else
	{
		pLc->SortItems(NumberCompare, m_AscendingSortFlags[_Index]);
	}

	for(RwInt32 i = 0; i < totItems; ++i)
	{
		CSortData* pItem = (CSortData*)(pLc->GetItemData(i));
		ASSERT(pItem);

		pLc->SetItemData(i, pItem->m_dw);

		delete pItem;
	}

	return TRUE;
}

RwBool CPathMeshFindDlg::IsInString(const char *pSrc, const char *pCmp)
{

	CString strSrc = pSrc;
	CString strCmp = pCmp;

	RwInt32 iLenSrc = strSrc.GetLength();
	RwInt32 iLenCmp = strCmp.GetLength();

	strSrc.MakeUpper();
	strCmp.MakeUpper();

	RwInt32 iPos = strSrc.Find(strCmp.GetBuffer());
	while (iPos != -1 && iPos <= iLenSrc - iLenCmp)
	{
		if (strSrc.Mid(iPos, iLenCmp) == strCmp)
		{
			return TRUE;
		}
		iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
	}

	return FALSE;
}

void CPathMeshFindDlg::AddSearchList(char* pcName, RwV3d* pvPos)
{
	CString strData[ESEARCHLIST_NUM];

	strData[ESEARCHLIST_NAME].Format("%s", pcName);
	strData[ESEARCHLIST_POS_X].Format("%.2f", pvPos->x);
	strData[ESEARCHLIST_POS_Y].Format("%.2f", pvPos->y);
	strData[ESEARCHLIST_POS_Z].Format("%.2f", pvPos->z);

	LVITEM	lvi;
	lvi.mask		= LVIF_TEXT;
	lvi.iItem		= m_listSearch.GetItemCount();
	lvi.iImage		= 0;
	for (int i = 0; i < ESEARCHLIST_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(strData[i].GetBuffer());
		if (i == ESEARCHLIST_NAME)
		{
			m_listSearch.InsertItem(&lvi);
		}
		else
		{
			m_listSearch.SetItem(&lvi);			
		}
	}
}

void CPathMeshFindDlg::OnBnClickedBtnPortal()
{
	RwInt32 iItem = m_listSearch.GetSelectionMark();

	if (iItem != -1)
	{
		RwV3d vPortal;
		vPortal.x = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_X).GetBuffer()));
		vPortal.y = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_Y).GetBuffer()));
		vPortal.z = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_POS_Z).GetBuffer()));

		GetSceneManager()->GetWorld()->SetPortalPosition(vPortal);

		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

		((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteOrnament();
		dGETAPP().Update(0.0f, 0.0f);

		RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
		if (iSectorIdx)
		{
			CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
			for (int i = 0; i < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
			{
				if (m_listSearch.GetItemText(iItem, ESEARCHLIST_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName())
				{
					if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityOrnament[i]->GetPosition())) < 0.01f)
					{
						dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
					}
				}
			}
		}
	}
}

void CPathMeshFindDlg::OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(!OnSortData(pNMLV->iSubItem))
		return;

	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CPathMeshFindDlg::GetWorldObject(RwInt32 iFieldIdx, _CBOBJECT cbObject, void* pData)
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", iFieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	CFileMem	FileMem;
	BYTE*		pFileMemPos = NULL;

	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	pFileMemPos = CNtlWorldFileMemAccessor::GetSPosOfMaterial(FileMem.GetDat());
	for (int iSectorCount = 0; iSectorCount < 4; ++iSectorCount)
	{
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Material(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Heightfield(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Diffuse(pFileMemPos);

		RwInt32	iNum;
		RwChar	cName[128];
		RwV3d	vPos, vScale, vRot;

		CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
		pFileMemPos += sizeof(RwInt32);
		for (int i = 0; i < iNum; ++i)
		{
			// Obj. Name
			CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
			pFileMemPos += sizeof(RwChar) * 128;

			// Scaling
			CopyMemory(&vScale, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			// Rotation
			CopyMemory(&vRot, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			// Translation
			CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
			pFileMemPos += sizeof(RwV3d);

			RwUInt32 uiObjectType;
			CopyMemory(&uiObjectType, pFileMemPos, sizeof(RwUInt32));
			pFileMemPos += sizeof(RwUInt32);

			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				// Object Propperty Range
				RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
					(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
					sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
					(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

				pFileMemPos += iPropertyRange;
			}

			// Trigger Obj Serial Id
			pFileMemPos += sizeof(RwUInt32);
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			// Object serial Id
			pFileMemPos += sizeof(RwUInt32);
#endif

			// Obj. Name Index
			pFileMemPos += sizeof(DWORD);

			// Visibility Distance
			pFileMemPos += sizeof(RwReal);

			// enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
			pFileMemPos += (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool));

			// Project Shadow
			RwInt32 iPSAtomicCnt = 0;
			CopyMemory(&iPSAtomicCnt, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);
			for (RwInt32 j = 0; j < iPSAtomicCnt; ++j)
			{
				// Map name
				pFileMemPos += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				pFileMemPos += (sizeof(RwReal));

				// Light matrices
				pFileMemPos += (sizeof(RwMatrix));

				// Map size
				RwInt32 iSize = 0;
				CopyMemory(&iSize, pFileMemPos, sizeof(RwInt32));
				pFileMemPos += (sizeof(RwInt32));

				// Map
				pFileMemPos += (sizeof(DWORD) * iSize * iSize);
			}

			// milepost
			RwBool bMilePost;
			CopyMemory(&bMilePost, pFileMemPos, sizeof(RwBool));
			pFileMemPos += sizeof(RwBool);

			if(bMilePost)
			{
				pFileMemPos += sizeof(RwUInt32);
			}

			(this->*cbObject)(cName, &vScale, &vRot, &vPos, pData); 
		}

		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Water(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Shadow(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Effects(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_SE(pFileMemPos);
		// CZ-SS
		//pFileMemPos = CNtlWorldFileMemAccessor::Skip_SS(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_SectorCull(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_TileTransparency(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Shoreline(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Decal(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Plant(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_WorldLight(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Occluder(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_HeatHazeObject(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_LightObject(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Dojo(pFileMemPos);
	}

	FileMem.Free();
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CPathMeshFindDlg::OnBnClickedBtnSearchRot()
{
	m_listSearch.DeleteAllItems();

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::SearchRot, NULL);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();
	}
}

void CPathMeshFindDlg::OnBnClickedBtnSearchArea()
{
	m_listSearch.DeleteAllItems();

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);

	char acTemp[256];
	GetDlgItemText(IDC_EDIT_SIZE, acTemp, 256);
	RwReal fSize = static_cast<RwReal>(::atof(acTemp));
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::SearchArea, &fSize);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();		
	}
}

void CPathMeshFindDlg::OnBnClickedBtnObsException()
{
	if (!CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		AfxMessageBox("Sorry... Use PathEngineMode.");
		GetDlgItem(IDC_BTN_OBS_EXCEPTION)->EnableWindow(FALSE);
		return;
	}

	std::map<RwInt32, RwReal*> mapPE;

	CProgressWnd	WndProgress(this, "Search Dialog");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d fields Searching", i, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
		WndProgress.SetText(acText);

		GetWorldObject(i, &CPathMeshFindDlg::ObsException, &mapPE);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();				
	}

	while (!mapPE.empty())
	{
		RwInt32 iIdx	= mapPE.begin()->first;
		RwReal* pList	= mapPE.begin()->second;
		CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(iIdx, pList);
		mapPE.erase(mapPE.begin());
		NTL_ARRAY_DELETE(pList);
	}

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->SetAnotherField();
}

void CPathMeshFindDlg::ObsException(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	CNtlPLObjectProperty* pProperty = (CNtlPLObjectProperty*)CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_OBJECT, pName);
	if (pProperty == NULL/* || !pProperty->IsNaviPress()*/)
	{
		return;
	}

	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	if (vecObsMesh.empty())
	{
		return;
	}

	for (int i = 0; i < (int)vecObsMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecObsMesh.at(i).c_str());

		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				std::vector<RwV3d> vecConvexHull;
				BuildClmupConvexHull(pRpClump, vecConvexHull);

				GenObsException(*((std::map<RwInt32, RwReal*>*)pData), vecConvexHull);
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}	
}

void CPathMeshFindDlg::GenObsException(std::map<RwInt32, RwReal*>& mapPE, std::vector<RwV3d>& vecConvexHull)
{
	RwBBox rwBBox;
	RwBBoxCalculate(&rwBBox, &vecConvexHull.front(), (RwInt32)vecConvexHull.size());

	CNtlWorldFieldManager*		pWFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager* pPEMgr = CNtlWorldPathEngineManager::GetInstance();

	rwBBox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.inf.x + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.inf.z + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.sup.x + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);
	rwBBox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(rwBBox.sup.z + dGET_WORLD_PARAM()->WorldSizeHalf) / pPEMgr->GetChunkSize()) * pPEMgr->GetChunkSize() - dGET_WORLD_PARAM()->WorldSizeHalf) + (pPEMgr->GetChunkSize() / 2);

	for (RwReal z = rwBBox.inf.z; z <= rwBBox.sup.z; z += pPEMgr->GetChunkSize())
	{
		for (RwReal x = rwBBox.inf.x; x <= rwBBox.sup.x; x += pPEMgr->GetChunkSize())
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.y = 0.0f;
			vPos.z = z;
			if (IsInConvexHull(vPos, vecConvexHull))
			{
				SetPathEngineTileProperty(vPos, RwRealMAXVAL, mapPE);
			}
		}
	}
}

void CPathMeshFindDlg::SetPathEngineTileProperty(RwV3d& vPos, RwReal fHeight, std::map<RwInt32, RwReal*>& mapPE)
{
	CNtlWorldFieldManager*		pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager*	pPEMgr		= CNtlWorldPathEngineManager::GetInstance();
	RwInt32						iSectorIdx	= pWFMgr->GetSectorIdx(vPos);

	if (iSectorIdx == -1)
	{
		return;
	}

	RwReal*									pHeightList	= NULL;
	std::map<RwInt32, RwReal*>::iterator	it			= mapPE.find(iSectorIdx);
	if (it == mapPE.end())
	{
		pHeightList = NTL_NEW RwReal [pPEMgr->GetChunkNum() * pPEMgr->GetChunkNum()];
		pPEMgr->LoadSwapFile(iSectorIdx, pHeightList);
		mapPE.insert(std::map<RwInt32, RwReal*>::value_type(iSectorIdx, pHeightList));
		pPEMgr->LoadSwapFile(iSectorIdx, pHeightList);
	}
	else
	{
		pHeightList = it->second;
	}

	RwV3d	vPosS;
	vPosS.x = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
	vPosS.y = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vPosS.z = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

	RwInt32	iMoveCntX	= static_cast<RwInt32>(abs(vPos.x - vPosS.x)) / pPEMgr->GetChunkSize();
	RwInt32	iMoveCntZ	= static_cast<RwInt32>(abs(vPos.z - vPosS.z)) / pPEMgr->GetChunkSize();
	RwInt32 iDataPos	= iMoveCntX + (iMoveCntZ * pPEMgr->GetChunkNum());

	pHeightList[iDataPos] = fHeight;
}

RwBool CPathMeshFindDlg::IsInConvexHull(RwV3d& vPos, std::vector<RwV3d>& vecConvexHull)
{
	for (int i = 0; i < (int)vecConvexHull.size(); ++i)
	{
		RwV3d vP1	= vecConvexHull.at(i);
		RwV3d vP2	= vecConvexHull.at((i + 1 == (int)vecConvexHull.size() ? 0 : i + 1));
		RwV3d vP	= vP2 - vP1;

		RwV3d vN;
		RwV3dCrossProduct(&vN, &vP, &CNtlPLGlobal::m_vYAxisV3);		
		RwV3dNormalize(&vN, &vN);

		RwReal fDist	= RwV3dDotProduct(&vN, &vPos);
		RwReal fD		= - vN.x * vP1.x - vN.y * vP1.y - vN.z * vP1.z;

		if (fDist + fD >= 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CPathMeshFindDlg::BuildClmupConvexHull(RpClump* pRpClump, std::vector<RwV3d>& vecResult)
{
	CNtlConvexHull::vecdef_POS_LIST vecPosList;
	RwLLLink*						pCur	= rwLinkListGetFirstLLLink(&pRpClump->atomicList);
	RwLLLink*						pEnd	= rwLinkListGetTerminator(&pRpClump->atomicList);
	RwLLLink*						pNext	= NULL;

	while(pCur != pEnd)
	{
		RpAtomic*		pCurAtomic		= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
		RpGeometry*		pCurGeometry	= RpAtomicGetGeometry(pCurAtomic);	
		RpMorphTarget*	pCurMorphTarget	= RpGeometryGetMorphTarget(pCurGeometry, 0);
		RpTriangle*		pCurTriangle	= RpGeometryGetTriangles(pCurGeometry);
		RwV3d*			pCurVertices	= RpMorphTargetGetVertices(pCurMorphTarget);
		RwMatrix*		pRwMatrix		= RwFrameGetLTM(RpAtomicGetFrame(pCurAtomic));

		for (int i = 0; i < RpGeometryGetNumTriangles(pCurGeometry); ++i)
		{
			CNtlConvexHull::pairdef_POS pairPos;
			RwV3d						vTemp;

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[0]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[1]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);

			RwV3dTransformPoint(&vTemp, &pCurVertices[pCurTriangle[i].vertIndex[2]], pRwMatrix);
			pairPos.first	= vTemp.x;
			pairPos.second	= vTemp.z;
			vecPosList.push_back(pairPos);
		}		

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	CNtlConvexHull ntlConvexHull(vecPosList); ntlConvexHull.BuildHull();
	CNtlConvexHull::pairdef_POS* pPairPos = ntlConvexHull.BeginHull();
	while (pPairPos)
	{
		RwV3d vTemp;
		vTemp.x = pPairPos->first;
		vTemp.y = 0.0f;
		vTemp.z = pPairPos->second;

		vecResult.push_back(vTemp);
		pPairPos = ntlConvexHull.NextHull();
	}
}

void CPathMeshFindDlg::SearchRot(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	if (m_BtnNaviMesh.GetCheck() && !vecNaviMesh.empty())
	{
		if (pvRot->x > 0.0f || pvRot->z > 0.0f ||
			pvRot->x < 0.0f || pvRot->z < 0.0f)
		{
			AddSearchList(pName, pvPos);
			return;
		}
	}
	if (m_BtnObsMesh.GetCheck() && !vecObsMesh.empty())
	{
		/*for (int i = 0; i < (int)m_vecExceptionList.size(); ++i)
		{
		if (IsInString(pName, m_vecExceptionList.at(i).c_str()))
		{
		return;
		}
		}*/
		if (pvRot->x > 0.0f || pvRot->z > 0.0f ||
			pvRot->x < 0.0f || pvRot->z < 0.0f)
		{
			AddSearchList(pName, pvPos);
			return;
		}
	}
}

void CPathMeshFindDlg::SearchArea(char* pName, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos, void* pData)
{
	RwReal fSize = *((RwReal*)pData);

	std::vector<std::string> vecNaviMesh;
	std::vector<std::string> vecObsMesh;	
	GetPathEngineMeshName(pName, vecNaviMesh, vecObsMesh);

	for (int i = 0; i < (int)vecNaviMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecNaviMesh.at(i).c_str());

		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				RwSphere rwSphere;
				CalcClmupBoundingSphere(pRpClump, &rwSphere);

				RwV3d	vTemp = rwSphere.center;
				vTemp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.x = static_cast<RwReal>(static_cast<RwInt32>(vTemp.x / fSize) * fSize);
				vTemp.z = static_cast<RwReal>(static_cast<RwInt32>(vTemp.z / fSize) * fSize);
				vTemp.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

				if (rwSphere.center.x < vTemp.x + rwSphere.radius ||
					rwSphere.center.z < vTemp.z + rwSphere.radius ||
					rwSphere.center.x > vTemp.x + fSize - rwSphere.radius ||
					rwSphere.center.z > vTemp.z + fSize - rwSphere.radius)
				{
					AddSearchList(pName, pvPos);
				}
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}
	for (int i = 0; i < (int)vecObsMesh.size(); ++i)
	{
		char acTempPath[NTL_MAX_DIR_PATH];
		sprintf_s(acTempPath, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s", dGET_WORLD_PARAM()->CurWorkingFolderName, vecObsMesh.at(i).c_str());

		CNtlPLResource* pRes = GetNtlResourceManager()->LoadClump(acTempPath, "");
		DBO_ASSERT(pRes, "Resource load failed.");		
		if (pRes)
		{
			RpClump* pRpClump = pRes->GetClump();
			if (pRpClump)
			{
				SetClmupTransform(pRpClump, pvScale, pvRot, pvPos);

				RwSphere rwSphere;
				CalcClmupBoundingSphere(pRpClump, &rwSphere);

				RwV3d	vTemp = rwSphere.center;
				vTemp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.x = static_cast<RwReal>(static_cast<RwInt32>(vTemp.x / fSize) * fSize);
				vTemp.z = static_cast<RwReal>(static_cast<RwInt32>(vTemp.z / fSize) * fSize);
				vTemp.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
				vTemp.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

				if (rwSphere.center.x < vTemp.x + rwSphere.radius ||
					rwSphere.center.z < vTemp.z + rwSphere.radius ||
					rwSphere.center.x > vTemp.x + fSize - rwSphere.radius ||
					rwSphere.center.z > vTemp.z + fSize - rwSphere.radius)
				{
					AddSearchList(pName, pvPos);
				}
			}
			GetNtlResourceManager()->UnLoad(pRes);
		}
	}
}

void CPathMeshFindDlg::SetClmupTransform(RpClump* pRpClump, RwV3d* pvScale, RwV3d* pvRot, RwV3d* pvPos)
{
	RwFrame*		pRwFrame	= RpClumpGetFrame(pRpClump); 
	RwMatrix*		pRwMatrix	= RwFrameGetMatrix(pRwFrame);

	RwMatrix		rwMatrix;
	RwV3d			vFront, vLeft, vUp;

	RwMatrixRotate(&rwMatrix, &CNtlPLGlobal::m_vYAxisV3, pvRot->y, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &CNtlPLGlobal::m_vZAxisV3, &rwMatrix);
	RwV3dTransformVector(&vLeft, &CNtlPLGlobal::m_vXAxisV3, &rwMatrix);

	RwMatrixRotate(&rwMatrix, &vLeft, pvRot->x, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &vFront, &rwMatrix);
	RwV3dTransformVector(&vUp, &CNtlPLGlobal::m_vYAxisV3, &rwMatrix);

	RwMatrixScale(pRwMatrix, pvScale, rwCOMBINEREPLACE);
	RwMatrixRotate(pRwMatrix, &CNtlPLGlobal::m_vYAxisV3, pvRot->y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pRwMatrix, &vLeft, pvRot->x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pRwMatrix, &vFront, pvRot->z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(pRwMatrix, pvPos, rwCOMBINEPOSTCONCAT);

	RwFrameUpdateObjects(pRwFrame);
}

void CPathMeshFindDlg::CalcClmupBoundingSphere(RpClump* pRpClump, RwSphere* pRwSphere)
{
	RwSphere*	pCurSphere;
	RwLLLink*	pCur;
	RwLLLink*	pEnd;
	RwLLLink*	pNext;
	RpAtomic*	pCurAtomic;
	RwInt32		NumSphere = 0;
	RpAtomic*	pAtomicOfMaxRadius;
	RwReal		LenMaxRadius;

	pRwSphere->center.x	= 0.0f;
	pRwSphere->center.y	= 0.0f;
	pRwSphere->center.z	= 0.0f;
	pRwSphere->radius	= 0.0f;

	pCur = rwLinkListGetFirstLLLink(&pRpClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pRpClump->atomicList);

	while(pCur != pEnd)
	{
		pCurAtomic	= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
		pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pCurAtomic));

		if(!NumSphere)
		{
			LenMaxRadius		= pCurSphere->radius;
			pAtomicOfMaxRadius	= pCurAtomic;

			CNtlMath::MathRwV3dAssign(&pRwSphere->center, pCurSphere->center.x, pCurSphere->center.y, pCurSphere->center.z);
		}
		else
		{
			if(LenMaxRadius < pCurSphere->radius)
			{
				LenMaxRadius		= pCurSphere->radius;
				pAtomicOfMaxRadius	= pCurAtomic;
			}

			pRwSphere->center.x += pCurSphere->center.x;
			pRwSphere->center.y += pCurSphere->center.y;
			pRwSphere->center.z += pCurSphere->center.z;
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;

		NumSphere++;
	}

	pRwSphere->center.x /= NumSphere;
	pRwSphere->center.y /= NumSphere;
	pRwSphere->center.z /= NumSphere;
	pRwSphere->radius	= LenMaxRadius + sqrtf(
		(pRwSphere->center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) * 
		(pRwSphere->center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) + 
		(pRwSphere->center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) * 
		(pRwSphere->center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) + 
		(pRwSphere->center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z) * 
		(pRwSphere->center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z)
		);
}

void CPathMeshFindDlg::GetPathEngineMeshName(char* pName, std::vector<std::string>& vecNaviMesh, std::vector<std::string>& vecObsMesh)
{
	WIN32_FIND_DATA findData;
	HANDLE			hfindFile;

	// NaviMesh Find
	RwChar acNaviFind[NTL_MAX_DIR_PATH];
	sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
		dGET_WORLD_PARAM()->CurWorkingFolderName, pName);

	hfindFile = ::FindFirstFile(acNaviFind, &findData);
	if(hfindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.')
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					continue;
				}
				else
				{
					vecNaviMesh.push_back(findData.cFileName);
				}
			}
		} while(::FindNextFile(hfindFile, &findData));

		::FindClose(hfindFile);
	}

	RwChar acObstFind[NTL_MAX_DIR_PATH];
	sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
		dGET_WORLD_PARAM()->CurWorkingFolderName, pName);

	hfindFile = ::FindFirstFile(acObstFind, &findData);
	if(hfindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.')
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					continue;
				}
				else
				{								
					vecObsMesh.push_back(findData.cFileName);
				}
			}
		} while(::FindNextFile(hfindFile, &findData));

		::FindClose(hfindFile);
	}
}

void CPathMeshFindDlg::OnBnClickedBtnImport()
{
	if (!CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		AfxMessageBox("Sorry... Use PathEngineMode.");
		GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
		return;
	}

	CFileDialog	FileDlg(TRUE,
		"select Import texture",
		"*.png; *.bmp;",
		OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		"Data Files (*.png;*.bmp;)|*.png;*.bmp||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\Grid Map";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		return;		
	}

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
	{
		return;
	}

	CProgressWnd	WndProgress(this, "Import Image");	
	char			acText[256];

	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum);

	m_pRwImage = RwImageRead(FileDlg.GetPathName().GetBuffer());

	std::map<RwInt32, RwReal*> mapPE;

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
	{
		::sprintf_s(acText, 256, "%d / %d sectors Searching", i, dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum);
		WndProgress.SetText(acText);

		GenPathEngineTilePropertyFromImage(i, m_pRwImage, mapPE);

		WndProgress.StepIt();
		WndProgress.PeekAndPump();	
	}

	while (!mapPE.empty())
	{
		RwInt32 iIdx	= mapPE.begin()->first;
		RwReal* pList	= mapPE.begin()->second;
		CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(iIdx, pList);
		mapPE.erase(mapPE.begin());
		NTL_ARRAY_DELETE(pList);
	}

	if (m_pRwImage)
	{
		RwImageDestroy(m_pRwImage);
		m_pRwImage = NULL;
	}

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->SetAnotherField();
}

void CPathMeshFindDlg::GenPathEngineTilePropertyFromImage(RwInt32 iSectorIdx, RwImage* pRwImage, std::map<RwInt32,RwReal*> &mapPE)
{
	if (pRwImage == NULL || iSectorIdx == -1)
	{
		return;
	}
	CNtlWorldFieldManager*			pWFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldPathEngineManager*		pPEMgr = CNtlWorldPathEngineManager::GetInstance();
	// SECTOR
	RwV3d vCurPos;
	vCurPos.x = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x;
	vCurPos.y = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vCurPos.z = pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z;

	// TEXTURE SIZE
	//RwInt32 iTexWidth	= RwRasterGetWidth(pRwTexture->raster);
	//RwInt32 iTexHeight	= RwRasterGetHeight(pRwTexture->raster);
	RwInt32 iTexWidth	= RwImageGetWidth(pRwImage);
	RwInt32 iTexHeight	= RwImageGetHeight(pRwImage);

	// TEXTURE LOCK
	//DWORD *pBits = (DWORD*)RwRasterLock(pRwTexture->raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKREAD);

	DWORD *pBits = (DWORD*)RwImageGetPixels(pRwImage);

	// SECTOR VLIST
	RwV3d* pvList = NTL_NEW RwV3d [dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum];
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetPosVertList(vCurPos, pvList);

	for (RwInt32 z = static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2));
		z < static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.z + (dGET_WORLD_PARAM()->WorldSectorSize / 2));
		z += pPEMgr->GetChunkSize())
	{
		RwReal fDeltaZ = 1.0f - (static_cast<RwReal>(z + dGET_WORLD_PARAM()->WorldSizeHalf) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
		for (RwInt32 x = static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2));
			x < static_cast<RwInt32>(pWFMgr->m_pSectors[iSectorIdx].DatumPoint.x + (dGET_WORLD_PARAM()->WorldSectorSize / 2));
			x += pPEMgr->GetChunkSize())
		{
			RwReal fDeltaX = 1.0f - (static_cast<RwReal>(x + dGET_WORLD_PARAM()->WorldSizeHalf) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));

			// TEXTURE COLOR
			DWORD *pDest	= pBits + iTexWidth * static_cast<RwInt32>(static_cast<RwReal>(iTexHeight - 1) * fDeltaZ) +	static_cast<RwInt32>(static_cast<RwReal>(iTexWidth - 1) * fDeltaX);
			DWORD ARGB		= *pDest;
			DWORD A			= (DWORD)(((*pDest) & 0xff000000) >> 24);
			DWORD R			= (DWORD)(((*pDest) & 0x00ff0000) >> 16);
			DWORD G			= (DWORD)(((*pDest) & 0x0000ff00) >> 8);
			DWORD B			= (DWORD)(((*pDest) & 0x000000ff) >> 0);

			vCurPos.x = static_cast<RwReal>(x);
			vCurPos.y = -999.0f;
			vCurPos.z = static_cast<RwReal>(z);
			if (R == 255 && G == 255 && B == 255)
			{
				vCurPos.y = RpNtlWorldSectorGetHeight(&pWFMgr->m_pSectors[iSectorIdx], &vCurPos, pvList);
			}

			if (vCurPos.y == -999.0f)
			{
				SetPathEngineTileProperty(vCurPos, RwRealMAXVAL, mapPE);
			}
			else
			{
				SetPathEngineTileProperty(vCurPos, vCurPos.y, mapPE);				
			}
		}
	}

	// TEXTURE UNLOCK
	//RwRasterUnlock(pRwTexture->raster);	
	NTL_ARRAY_DELETE(pvList);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif