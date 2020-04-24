// FieldSearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldInterface.h"

#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWeControlUi.h"

#include "NtlPLWorldEntity.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldSectorinfo.h"

#include "CameraAttrDlg.h"
#include "FieldSearchDlg.h"

#include "ProgressWnd.h"
#include "NtlWEUtil.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

// CFieldSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldSearchDlg, CDialog)

CFieldSearchDlg::CFieldSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldSearchDlg::IDD, pParent)
	, m_strSearch(_T(""))
	, m_strFieldIdx(_T(""))
	, m_bSearchObject(FALSE)
	, m_bSearchEffect(FALSE)
	, m_bSearchSE(FALSE)
	, m_strSelectError(_T(""))
	, m_bSearchFieldAll(FALSE)
	, m_strListCnt(_T(""))
{

}

CFieldSearchDlg::~CFieldSearchDlg()
{
}

void CFieldSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCH_TEXT, m_strSearch);
	DDX_Control(pDX, IDC_SEARCH_LIST, m_listSearch);
	DDX_Text(pDX, IDC_SEARCH_FIELD_IDX, m_strFieldIdx);
	DDX_Check(pDX, IDC_CHECK_OBJECT, m_bSearchObject);
	DDX_Check(pDX, IDC_CHECK_EFFECT, m_bSearchEffect);
	DDX_Check(pDX, IDC_CHECK_SE, m_bSearchSE);
	DDX_Text(pDX, IDC_SEARCH_SELECT_ERROR, m_strSelectError);
	DDX_Check(pDX, IDC_CHECK_FIELD_ALL, m_bSearchFieldAll);
	DDX_Text(pDX, IDC_LIST_CNT_TEXT, m_strListCnt);
}


BEGIN_MESSAGE_MAP(CFieldSearchDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFieldSearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFieldSearchDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SEARCH_LIST, &CFieldSearchDlg::OnLvnColumnClickList)
	ON_BN_CLICKED(IDC_CHECK_OBJECT, &CFieldSearchDlg::OnBnClickedCheckObject)
	ON_BN_CLICKED(IDC_CHECK_EFFECT, &CFieldSearchDlg::OnBnClickedCheckEffect)
	ON_BN_CLICKED(IDC_CHECK_SE, &CFieldSearchDlg::OnBnClickedCheckSe)
	ON_BN_CLICKED(IDC_SEARCH_PORTAL, &CFieldSearchDlg::OnBnClickedSearchPortal)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SEARCH_LIST, &CFieldSearchDlg::OnLvnItemchangedSearchList)
END_MESSAGE_MAP()


// CFieldSearchDlg 메시지 처리기입니다.

BOOL CFieldSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == GetDlgItem(IDC_SEARCH_TEXT)->m_hWnd &&
		pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateSearchList();
		return TRUE;
	}

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

BOOL CFieldSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_INDEX, "Field Idx", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_TYPE, "Type", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_NAME, "Name", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_X, "Pos.X", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_Y, "Pos.Y", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_Z, "Pos.Z", LVCFMT_CENTER, 80);
	m_listSearch.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);

	return TRUE;
}

void CFieldSearchDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CFieldSearchDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CFieldSearchDlg::InitFieldSearch()
{
	UpdateData(TRUE);

	m_listSearch.DeleteAllItems();
	m_vecFieldIdx.clear();

	m_strSearch			= "";
	m_strSelectError	= "0.01";
	
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
		m_vecFieldIdx.push_back(pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
	}
	else
	{
		RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
		RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				m_vecFieldIdx.push_back(pFieldMgr->GetFieldIdx(CurPos));
			}
		}
	}

	m_strFieldIdx.Format("Count(%d)", (int)m_vecFieldIdx.size());
	m_strListCnt.Format("(0/0)");
	
	char cTemp[32];
	for (int i = 0; i < (int)m_vecFieldIdx.size(); ++i)
	{
		m_strFieldIdx += "\r\n";
		m_strFieldIdx += "Field(";
		m_strFieldIdx += itoa(m_vecFieldIdx.at(i), cTemp, 10);
		m_strFieldIdx += ")";
	}

	UpdateData(FALSE);
}

void CFieldSearchDlg::UpdateSearchList()
{
	/*
	WorldFileFormat - SectorInfo
	WorldEditor에서 Object, SE, Effect를 검색하기 위해 사용되는 부분이다.
	*/

 	UpdateData(TRUE);

	m_listSearch.DeleteAllItems();

// 	if (!m_strSearch.GetLength())
// 	{
// 		return;
// 	}
	CProgressWnd	WndProgress(this, "Search Dialog");
	RwInt32			iRange;
	RwChar			cText[256];

	WndProgress.GoModal();
	if (m_bSearchFieldAll)
	{
		iRange = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;
		WndProgress.SetRange(0, iRange);
		for (int i = 0; i < iRange; ++i)
		{
			::sprintf_s(cText, 256, "%d / %d fields Searching", i, iRange);
			WndProgress.SetText(cText);

			Search(i);

			WndProgress.StepIt();
			WndProgress.PeekAndPump();	
		}
	}
	else
	{
		iRange = (int)m_vecFieldIdx.size();
		for (int i = 0; i < iRange; ++i)
		{
			::sprintf_s(cText, 256, "%d / %d fields Searching", i, iRange);
			WndProgress.SetText(cText);

			Search(m_vecFieldIdx.at(i));

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}
	}
	WndProgress.DestroyWindow();

	if (m_listSearch.GetItemCount())
	{
		for (int i = 0; i < ESEARCHLIST_FIELD_NUM; ++i)
		{
			m_listSearch.SetColumnWidth(i, LVSCW_AUTOSIZE);
		}
	}	
	m_strListCnt.Format("(%d/%d)", m_listSearch.GetSelectedCount(), m_listSearch.GetItemCount());
	
	UpdateData(FALSE);
}

void CFieldSearchDlg::Search(RwInt32 iFieldIdx)
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

		// Doodads
		if (m_bSearchObject)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);
			for (int i = 0; i < iNum; ++i)
			{
				// Obj. Name
				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
					pFileMemPos += sizeof(RwChar) * 128;
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiLength = 0;
					CopyMemory(&uiLength, pFileMemPos, sizeof(RwUInt32));
					pFileMemPos += sizeof(RwUInt32);
					
					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * uiLength);
					pFileMemPos += (sizeof(RwChar) * uiLength);
					cName[uiLength] = '\0';
				}				

				// Scaling
				pFileMemPos += sizeof(RwV3d);

				// Rotation
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
					pFileMemPos = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemPos);
				}

				// Trigger Obj Serial Id
				pFileMemPos += sizeof(RwUInt32);

				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
				{
					pFileMemPos += (sizeof(RwUInt32));		// Object  serial id	
				}

				// Obj. Name Index
				pFileMemPos += sizeof(DWORD);

				// Visibility Distance
				pFileMemPos += sizeof(RwReal);

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					// enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
					pFileMemPos += (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool)) + (sizeof(RwBool));
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					pFileMemPos += (sizeof(RwUInt32));
				}

				// Project Shadow
				pFileMemPos = CNtlPLObject::SkipPSMapToFileMem(pFileMemPos);
				

				// milepost
				RwBool bMilePost;
				CopyMemory(&bMilePost, pFileMemPos, sizeof(RwBool));
				pFileMemPos += sizeof(RwBool);

				if(bMilePost)
				{
					pFileMemPos += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemPos = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemPos);

				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
				{
					pFileMemPos = CNtlPLObject::SkipLightmapToFileMem(pFileMemPos);
				}

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Object", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pFileMemPos, AW_HEGITHFIELD);

		}

		pFileMemPos = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemPos);
		pFileMemPos = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemPos);
		
		// Effect
		if (m_bSearchEffect)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// Effect. Name
				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
					pFileMemPos += sizeof(RwChar) * 128;
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiLength = 0;

					CopyMemory(&uiLength, pFileMemPos, sizeof(RwUInt32));
					pFileMemPos += sizeof(RwUInt32);

					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * uiLength);
					cName[uiLength] = '\0';
					pFileMemPos += sizeof(RwChar) * uiLength;
					
				}

				// Scaling
				pFileMemPos += sizeof(RwV3d);

				// Rotation
				pFileMemPos += sizeof(RwV3d);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				// Visibility Distance
				pFileMemPos += sizeof(RwReal);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Effect", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemPos);
		}

		// SE
		if (m_bSearchSE)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// Name
				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 32);
					pFileMemPos += sizeof(RwChar) * 32;
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiLength = 0;

					CopyMemory(&uiLength, pFileMemPos, sizeof(RwUInt32));
					pFileMemPos += sizeof(RwUInt32);

					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * uiLength);
					cName[uiLength] = '\0';
					pFileMemPos += sizeof(RwChar) * uiLength;
				}				

				// Vol, Radius Min, Max
				pFileMemPos += sizeof(RwReal) + sizeof(RwReal) + sizeof(RwReal);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Sound", cName, &vPos);
				}
			}

			// CZ-BGM
			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// Name
				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 32);
					pFileMemPos += sizeof(RwChar) * 32;
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiLength = 0;

					CopyMemory(&uiLength, pFileMemPos, sizeof(RwUInt32));
					pFileMemPos += sizeof(RwUInt32);

					CopyMemory(cName, pFileMemPos, sizeof(RwChar) * uiLength);
					cName[uiLength] = '\0';
					pFileMemPos += sizeof(RwChar) * uiLength;
				}

				// Vol
				//pFileMemPos += sizeof(RwReal);
				// VRadius
				pFileMemPos += sizeof(RwReal);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Sound", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemPos);
		}			

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

void CFieldSearchDlg::AddSearchList(RwInt32 iFieldIdx, char* pcType, char* pcName, RwV3d* pvPos)
{
	CString strData[ESEARCHLIST_FIELD_NUM];

	strData[ESEARCHLIST_FIELD_INDEX].Format("%d", iFieldIdx);
	strData[ESEARCHLIST_FIELD_TYPE].Format("%s", pcType);
	strData[ESEARCHLIST_FIELD_NAME].Format("%s", pcName);
	strData[ESEARCHLIST_FIELD_POS_X].Format("%.2f", pvPos->x);
	strData[ESEARCHLIST_FIELD_POS_Y].Format("%.2f", pvPos->y);
	strData[ESEARCHLIST_FIELD_POS_Z].Format("%.2f", pvPos->z);
	
	LVITEM	lvi;
	lvi.mask		= LVIF_TEXT;
	lvi.iItem		= m_listSearch.GetItemCount();
	lvi.iImage		= 0;
	for (int i = 0; i < ESEARCHLIST_FIELD_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(strData[i].GetBuffer());
		if (i == ESEARCHLIST_FIELD_INDEX)
		{
			m_listSearch.InsertItem(&lvi);
		}
		else
		{
			m_listSearch.SetItem(&lvi);			
		}
	}
}

RwBool CFieldSearchDlg::IsInString(char *pSrc, char *pCmp)
{

	CString strSrc = pSrc;
	CString strCmp = pCmp;

	RwInt32 iLenSrc = strSrc.GetLength();
	RwInt32 iLenCmp = strCmp.GetLength();

// 	if (iLenSrc >= iLenCmp)
// 	{
// 		strSrc.MakeUpper();
// 		strCmp.MakeUpper();
// 		for (char* pC = strSrc.GetBuffer(); pC <= strSrc.GetBuffer() + iLenSrc - iLenCmp; ++pC)
// 		{
// 			if (!strncmp(pC, strCmp.GetBuffer(), iLenCmp))
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}

// 	if (iLenSrc >= iLenCmp)
// 	{
// 		strSrc.MakeUpper();
// 		strCmp.MakeUpper();
// 
// 		for (int i = 0; i < iLenSrc - iLenCmp; ++i)
// 		{
// 			if (strSrc.Mid(i, iLenCmp) == strCmp)
// 			{
// 				return TRUE;
// 			}
// 		}	
// 	}

// 	if (iLenSrc >= iLenCmp)
// 	{
// 		strSrc.MakeUpper();
// 		strCmp.MakeUpper();
// 
// 		RwInt32 iPos = 0;
// 		do 
// 		{
// 			iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
// 			if (strSrc.Mid(iPos, iLenCmp) == strCmp)
// 			{
// 				return TRUE;
// 			}
// 		} while (iPos != -1 && iPos <= iLenSrc - iLenCmp);
// 	}


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

RwBool CFieldSearchDlg::OnSortData(RwInt32 _Index)
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

	if(_Index == 1 || _Index == 2)
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

void CFieldSearchDlg::OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(!OnSortData(pNMLV->iSubItem))
		return;

	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CFieldSearchDlg::OnBnClickedCheckObject()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedCheckEffect()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedCheckSe()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedSearchPortal()
{
	RwInt32 iItem = m_listSearch.GetSelectionMark();

	if (iItem != -1)
	{
		RwV3d vPortal;
		vPortal.x = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_X).GetBuffer()));
		vPortal.y = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_Y).GetBuffer()));
		vPortal.z = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_Z).GetBuffer()));

		GetSceneManager()->GetWorld()->SetPortalPosition(vPortal);

		// force the camera to be free
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

		RwReal fSelectError = static_cast<RwReal>(atof(m_strSelectError.GetBuffer()));
		
		if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Object")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteOrnament();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityOrnament[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
						}
					}

					/*if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName() &&
						static_cast<RwInt32>(vPortal.x) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().x) &&
						static_cast<RwInt32>(vPortal.y) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().y) &&
						static_cast<RwInt32>(vPortal.z) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().z))
					{
						dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
						break;
					}*/
				}
			}
		}
		else if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Effect")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteEffect();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntityEffect.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityEffect[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityEffect[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedEffect(pSector->m_vecNtlPLEntityEffect[i]);
						}
					}

					/*if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityEffect[i]->GetName() &&
						static_cast<RwInt32>(vPortal.x) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().x) &&
						static_cast<RwInt32>(vPortal.y) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().y) &&
						static_cast<RwInt32>(vPortal.z) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().z))
					{
						dGETHDL()->AddSelectedEffect(pSector->m_vecNtlPLEntityEffect[i]);
						break;
					}*/
				}
			}
		}
		else if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Sound")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteSE();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntitySound.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntitySound[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntitySound[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedSE(pSector->m_vecNtlPLEntitySound[i]);
						}
					}
				}

				for (int i = 0; i < (int)pSector->m_vecNtlPLEntitySoundBGM.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntitySoundBGM[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntitySoundBGM[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedSE(pSector->m_vecNtlPLEntitySoundBGM[i]);
						}
					}
				}
			}
		}
	}
}
void CFieldSearchDlg::OnLvnItemchangedSearchList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	UpdateData(TRUE);
	m_strListCnt.Format("(%d/%d)", m_listSearch.GetSelectedCount(), m_listSearch.GetItemCount());
	UpdateData(FALSE);
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

// CFieldSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldSearchDlg, CDialog)

CFieldSearchDlg::CFieldSearchDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFieldSearchDlg::IDD, pParent)
, m_strSearch(_T(""))
, m_strFieldIdx(_T(""))
, m_bSearchObject(FALSE)
, m_bSearchEffect(FALSE)
, m_bSearchSE(FALSE)
, m_strSelectError(_T(""))
, m_bSearchFieldAll(FALSE)
, m_strListCnt(_T(""))
{

}

CFieldSearchDlg::~CFieldSearchDlg()
{
}

void CFieldSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCH_TEXT, m_strSearch);
	DDX_Control(pDX, IDC_SEARCH_LIST, m_listSearch);
	DDX_Text(pDX, IDC_SEARCH_FIELD_IDX, m_strFieldIdx);
	DDX_Check(pDX, IDC_CHECK_OBJECT, m_bSearchObject);
	DDX_Check(pDX, IDC_CHECK_EFFECT, m_bSearchEffect);
	DDX_Check(pDX, IDC_CHECK_SE, m_bSearchSE);
	DDX_Text(pDX, IDC_SEARCH_SELECT_ERROR, m_strSelectError);
	DDX_Check(pDX, IDC_CHECK_FIELD_ALL, m_bSearchFieldAll);
	DDX_Text(pDX, IDC_LIST_CNT_TEXT, m_strListCnt);
}


BEGIN_MESSAGE_MAP(CFieldSearchDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFieldSearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFieldSearchDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SEARCH_LIST, &CFieldSearchDlg::OnLvnColumnClickList)
	ON_BN_CLICKED(IDC_CHECK_OBJECT, &CFieldSearchDlg::OnBnClickedCheckObject)
	ON_BN_CLICKED(IDC_CHECK_EFFECT, &CFieldSearchDlg::OnBnClickedCheckEffect)
	ON_BN_CLICKED(IDC_CHECK_SE, &CFieldSearchDlg::OnBnClickedCheckSe)
	ON_BN_CLICKED(IDC_SEARCH_PORTAL, &CFieldSearchDlg::OnBnClickedSearchPortal)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SEARCH_LIST, &CFieldSearchDlg::OnLvnItemchangedSearchList)
END_MESSAGE_MAP()


// CFieldSearchDlg 메시지 처리기입니다.

BOOL CFieldSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == GetDlgItem(IDC_SEARCH_TEXT)->m_hWnd &&
		pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateSearchList();
		return TRUE;
	}

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

BOOL CFieldSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_INDEX, "Field Idx", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_TYPE, "Type", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_NAME, "Name", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_X, "Pos.X", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_Y, "Pos.Y", LVCFMT_CENTER, 80);
	m_listSearch.InsertColumn(ESEARCHLIST_FIELD_POS_Z, "Pos.Z", LVCFMT_CENTER, 80);
	m_listSearch.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);

	return TRUE;
}

void CFieldSearchDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CFieldSearchDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CFieldSearchDlg::InitFieldSearch()
{
	UpdateData(TRUE);

	m_listSearch.DeleteAllItems();
	m_vecFieldIdx.clear();

	m_strSearch			= "";
	m_strSelectError	= "0.01";

	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
		m_vecFieldIdx.push_back(pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
	}
	else
	{
		RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
		RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				m_vecFieldIdx.push_back(pFieldMgr->GetFieldIdx(CurPos));
			}
		}
	}

	m_strFieldIdx.Format("Count(%d)", (int)m_vecFieldIdx.size());
	m_strListCnt.Format("(0/0)");

	char cTemp[32];
	for (int i = 0; i < (int)m_vecFieldIdx.size(); ++i)
	{
		m_strFieldIdx += "\r\n";
		m_strFieldIdx += "Field(";
		m_strFieldIdx += itoa(m_vecFieldIdx.at(i), cTemp, 10);
		m_strFieldIdx += ")";
	}

	UpdateData(FALSE);
}

void CFieldSearchDlg::UpdateSearchList()
{
	/*
	WorldFileFormat - SectorInfo
	WorldEditor에서 Object, SE, Effect를 검색하기 위해 사용되는 부분이다.
	*/

	UpdateData(TRUE);

	m_listSearch.DeleteAllItems();

	// 	if (!m_strSearch.GetLength())
	// 	{
	// 		return;
	// 	}
	CProgressWnd	WndProgress(this, "Search Dialog");
	RwInt32			iRange;
	RwChar			cText[256];

	WndProgress.GoModal();
	if (m_bSearchFieldAll)
	{
		iRange = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;
		WndProgress.SetRange(0, iRange);
		for (int i = 0; i < iRange; ++i)
		{
			::sprintf_s(cText, 256, "%d / %d fields Searching", i, iRange);
			WndProgress.SetText(cText);

			Search(i);

			WndProgress.StepIt();
			WndProgress.PeekAndPump();	
		}
	}
	else
	{
		iRange = (int)m_vecFieldIdx.size();
		for (int i = 0; i < iRange; ++i)
		{
			::sprintf_s(cText, 256, "%d / %d fields Searching", i, iRange);
			WndProgress.SetText(cText);

			Search(m_vecFieldIdx.at(i));

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}
	}
	WndProgress.DestroyWindow();

	if (m_listSearch.GetItemCount())
	{
		for (int i = 0; i < ESEARCHLIST_FIELD_NUM; ++i)
		{
			m_listSearch.SetColumnWidth(i, LVSCW_AUTOSIZE);
		}
	}	
	m_strListCnt.Format("(%d/%d)", m_listSearch.GetSelectedCount(), m_listSearch.GetItemCount());

	UpdateData(FALSE);
}

void CFieldSearchDlg::Search(RwInt32 iFieldIdx)
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

		// Doodads
		if (m_bSearchObject)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);
			for (int i = 0; i < iNum; ++i)
			{
				// Obj. Name
				CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
				pFileMemPos += sizeof(RwChar) * 128;

				// Scaling
				pFileMemPos += sizeof(RwV3d);

				// Rotation
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
				// Object Serial Id
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

				// attach light object skip
				RwInt32 iLightObjectNum;
				CopyMemory(&iLightObjectNum, pFileMemPos, sizeof(RwInt32));
				pFileMemPos += sizeof(RwInt32);

				for (int j = 0; j < iLightObjectNum; ++j)
				{
					pFileMemPos += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

					pFileMemPos += sizeof(RpLightType);
					pFileMemPos += sizeof(RwBool);

					pFileMemPos += sizeof(RwV3d);

					pFileMemPos += sizeof(RwV3d);
					pFileMemPos += sizeof(RwRGBAReal);
					pFileMemPos += sizeof(RwReal);
					pFileMemPos += sizeof(RwReal);

					pFileMemPos += sizeof(RwBool);
					pFileMemPos += sizeof(RwBool);
				}

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Object", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = CNtlWorldFileMemAccessor::Skip_Doodads(pFileMemPos);
		}

		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Water(pFileMemPos);
		pFileMemPos = CNtlWorldFileMemAccessor::Skip_Shadow(pFileMemPos);

		// Effect
		if (m_bSearchEffect)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// Effect. Name
				CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 128);
				pFileMemPos += sizeof(RwChar) * 128;

				// Scaling
				pFileMemPos += sizeof(RwV3d);

				// Rotation
				pFileMemPos += sizeof(RwV3d);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				// Visibility Distance
				pFileMemPos += sizeof(RwReal);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Effect", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = CNtlWorldFileMemAccessor::Skip_Effects(pFileMemPos);
		}

		// SE
		if (m_bSearchSE)
		{
			RwInt32	iNum;
			RwChar	cName[128];
			RwV3d	vPos;

			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// Effect. Name
				CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 32);
				pFileMemPos += sizeof(RwChar) * 32;

				// Vol, Radius Min, Max
				pFileMemPos += sizeof(RwReal) + sizeof(RwReal) + sizeof(RwReal);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Sound", cName, &vPos);
				}
			}

			// CZ-BGM
			CopyMemory(&iNum, pFileMemPos, sizeof(RwInt32));
			pFileMemPos += sizeof(RwInt32);

			for (int i = 0; i < iNum; ++i)
			{
				// BGM. Name
				CopyMemory(cName, pFileMemPos, sizeof(RwChar) * 32);
				pFileMemPos += sizeof(RwChar) * 32;

				// Vol
				//pFileMemPos += sizeof(RwReal);
				// VRadius
				pFileMemPos += sizeof(RwReal);

				// Translation
				CopyMemory(&vPos, pFileMemPos, sizeof(RwV3d));
				pFileMemPos += sizeof(RwV3d);

				if (IsInString(cName, m_strSearch.GetBuffer()))
				{
					AddSearchList(iFieldIdx, "Sound", cName, &vPos);
				}
			}
		}
		else
		{
			pFileMemPos = CNtlWorldFileMemAccessor::Skip_SE(pFileMemPos);
		}			

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

void CFieldSearchDlg::AddSearchList(RwInt32 iFieldIdx, char* pcType, char* pcName, RwV3d* pvPos)
{
	CString strData[ESEARCHLIST_FIELD_NUM];

	strData[ESEARCHLIST_FIELD_INDEX].Format("%d", iFieldIdx);
	strData[ESEARCHLIST_FIELD_TYPE].Format("%s", pcType);
	strData[ESEARCHLIST_FIELD_NAME].Format("%s", pcName);
	strData[ESEARCHLIST_FIELD_POS_X].Format("%.2f", pvPos->x);
	strData[ESEARCHLIST_FIELD_POS_Y].Format("%.2f", pvPos->y);
	strData[ESEARCHLIST_FIELD_POS_Z].Format("%.2f", pvPos->z);

	LVITEM	lvi;
	lvi.mask		= LVIF_TEXT;
	lvi.iItem		= m_listSearch.GetItemCount();
	lvi.iImage		= 0;
	for (int i = 0; i < ESEARCHLIST_FIELD_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(strData[i].GetBuffer());
		if (i == ESEARCHLIST_FIELD_INDEX)
		{
			m_listSearch.InsertItem(&lvi);
		}
		else
		{
			m_listSearch.SetItem(&lvi);			
		}
	}
}

RwBool CFieldSearchDlg::IsInString(char *pSrc, char *pCmp)
{

	CString strSrc = pSrc;
	CString strCmp = pCmp;

	RwInt32 iLenSrc = strSrc.GetLength();
	RwInt32 iLenCmp = strCmp.GetLength();

	// 	if (iLenSrc >= iLenCmp)
	// 	{
	// 		strSrc.MakeUpper();
	// 		strCmp.MakeUpper();
	// 		for (char* pC = strSrc.GetBuffer(); pC <= strSrc.GetBuffer() + iLenSrc - iLenCmp; ++pC)
	// 		{
	// 			if (!strncmp(pC, strCmp.GetBuffer(), iLenCmp))
	// 			{
	// 				return TRUE;
	// 			}
	// 		}
	// 	}

	// 	if (iLenSrc >= iLenCmp)
	// 	{
	// 		strSrc.MakeUpper();
	// 		strCmp.MakeUpper();
	// 
	// 		for (int i = 0; i < iLenSrc - iLenCmp; ++i)
	// 		{
	// 			if (strSrc.Mid(i, iLenCmp) == strCmp)
	// 			{
	// 				return TRUE;
	// 			}
	// 		}	
	// 	}

	// 	if (iLenSrc >= iLenCmp)
	// 	{
	// 		strSrc.MakeUpper();
	// 		strCmp.MakeUpper();
	// 
	// 		RwInt32 iPos = 0;
	// 		do 
	// 		{
	// 			iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
	// 			if (strSrc.Mid(iPos, iLenCmp) == strCmp)
	// 			{
	// 				return TRUE;
	// 			}
	// 		} while (iPos != -1 && iPos <= iLenSrc - iLenCmp);
	// 	}


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

RwBool CFieldSearchDlg::OnSortData(RwInt32 _Index)
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

	if(_Index == 1 || _Index == 2)
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

void CFieldSearchDlg::OnLvnColumnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(!OnSortData(pNMLV->iSubItem))
		return;

	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CFieldSearchDlg::OnBnClickedCheckObject()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedCheckEffect()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedCheckSe()
{
	//UpdateSearchList();
}

void CFieldSearchDlg::OnBnClickedSearchPortal()
{
	RwInt32 iItem = m_listSearch.GetSelectionMark();

	if (iItem != -1)
	{
		RwV3d vPortal;
		vPortal.x = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_X).GetBuffer()));
		vPortal.y = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_Y).GetBuffer()));
		vPortal.z = static_cast<RwReal>(atof(m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_POS_Z).GetBuffer()));

		GetSceneManager()->GetWorld()->SetPortalPosition(vPortal);

		// force the camera to be free
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

		RwReal fSelectError = static_cast<RwReal>(atof(m_strSelectError.GetBuffer()));

		if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Object")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteOrnament();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityOrnament[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
						}
					}

					/*if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityOrnament[i]->GetName() &&
					static_cast<RwInt32>(vPortal.x) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().x) &&
					static_cast<RwInt32>(vPortal.y) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().y) &&
					static_cast<RwInt32>(vPortal.z) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament[i]->GetPosition().z))
					{
					dGETHDL()->AddSelectedObject(pSector->m_vecNtlPLEntityOrnament[i]);
					break;
					}*/
				}
			}
		}
		else if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Effect")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteEffect();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntityEffect.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityEffect[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntityEffect[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedEffect(pSector->m_vecNtlPLEntityEffect[i]);
						}
					}

					/*if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntityEffect[i]->GetName() &&
					static_cast<RwInt32>(vPortal.x) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().x) &&
					static_cast<RwInt32>(vPortal.y) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().y) &&
					static_cast<RwInt32>(vPortal.z) == static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect[i]->GetPosition().z))
					{
					dGETHDL()->AddSelectedEffect(pSector->m_vecNtlPLEntityEffect[i]);
					break;
					}*/
				}
			}
		}
		else if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_TYPE) == "Sound")
		{
			((CNtlWEFrm*)AfxGetMainWnd())->OnPaletteSE();
			dGETAPP().Update(0.0f, 0.0f);

			RwInt32 iSectorIdx = dGETMGR()->GetSectorIdx(vPortal);
			if (iSectorIdx)
			{
				CNtlWorldSector* pSector = &dGETMGR()->m_pSectors[iSectorIdx];
				for (int i = 0; i < (int)pSector->m_vecNtlPLEntitySound.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntitySound[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntitySound[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedSE(pSector->m_vecNtlPLEntitySound[i]);
						}
					}
				}

				for (int i = 0; i < (int)pSector->m_vecNtlPLEntitySoundBGM.size(); ++i)
				{
					if (m_listSearch.GetItemText(iItem, ESEARCHLIST_FIELD_NAME) == pSector->m_vecNtlPLEntitySoundBGM[i]->GetName())
					{
						if (RwV3dLength(&(vPortal - pSector->m_vecNtlPLEntitySoundBGM[i]->GetPosition())) < fSelectError)
						{
							dGETHDL()->AddSelectedSE(pSector->m_vecNtlPLEntitySoundBGM[i]);
						}
					}
				}
			}
		}
	}
}
void CFieldSearchDlg::OnLvnItemchangedSearchList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	UpdateData(TRUE);
	m_strListCnt.Format("(%d/%d)", m_listSearch.GetSelectedCount(), m_listSearch.GetItemCount());
	UpdateData(FALSE);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif