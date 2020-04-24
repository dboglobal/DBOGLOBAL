// PaletteSpawnSearch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "ProgressWnd.h"
#include "PaletteSpawnSearch.h"
#include "SpawnManager.h"
#include "SpawnContainer.h"
#include "Spawn.h"
#include "SpawnInstNode.h"
#include "NtlWeControlUi.h"
#include "CameraAttrDlg.h"
#include "PaletteSpawnPath.h"
#include "PaletteSpawn.h"
#include "SpawnManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"

#include "NtlSLApi.h"

#include "TableContainer.h"
#include "TextAllTable.h"
#include "MObTable.h"
#include "NPCTable.h"

#include "NtlWEUtil.h"

// CPaletteSpawnSearch 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawnSearch, CDialog)

CPaletteSpawnSearch::CPaletteSpawnSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawnSearch::IDD, pParent)
	, m_StrMinLvl(_T("0"))
	, m_StrMaxLvl(_T("0"))
	, m_StrSpawnIndex(_T("0"))
	, m_StrSpawnCountInfo(_T("0 / 0"))
	, m_strFieldNumber(_T(""))
	, m_strNestCount(_T(""))
	, m_strCoolTime(_T(""))
	, m_strWanderRange(_T(""))
	, m_strMoveRange(_T(""))
	, m_strDelayTime(_T(""))
	, m_strPathIndex(_T(""))
	, m_strSpawnGroupIdx(_T(""))
	, m_strPlayScriptIdx(_T(""))
	, m_strPlayScriptSceneIdx(_T(""))
	, m_strAIScriptIdx(_T(""))
	, m_strAIScriptSceneIdx(_T(""))
{
	for(RwInt32 i = 0; i < ESPAWNLIST_NUM; i++)
	{
		m_AscendingSortFlags[i] = FALSE;
	}
}

CPaletteSpawnSearch::~CPaletteSpawnSearch()
{
}

void CPaletteSpawnSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OPTION, m_SpawnListOptionCombo);
	DDX_Control(pDX, IDC_SPAWN_LIST, m_SpawnList);
	DDX_Text(pDX, IDC_LEVEL_MIN, m_StrMinLvl);
	DDX_Text(pDX, IDC_LEVEL_MAX, m_StrMaxLvl);
	DDX_Text(pDX, IDC_SPAWN_INDEX, m_StrSpawnIndex);
	DDX_Text(pDX, IDC_SPAWN_COUNT_INFO, m_StrSpawnCountInfo);
	DDX_Text(pDX, IDC_FIELD_NUMBER, m_strFieldNumber);
	DDX_Text(pDX, IDC_INDEX, m_strIndex);
	DDX_Text(pDX, IDC_NEST_COUNT, m_strNestCount);
	DDX_Text(pDX, IDC_COOL_TIME, m_strCoolTime);
	DDX_Text(pDX, IDC_WANDER_RANGE, m_strWanderRange);
	DDX_Text(pDX, IDC_MOVE_RANGE, m_strMoveRange);
	DDX_Text(pDX, IDC_DELAY_TIME, m_strDelayTime);
	DDX_Text(pDX, IDC_PATH_INDEX, m_strPathIndex);
	DDX_Text(pDX, IDC_SPAWN_GROUP_IDX, m_strSpawnGroupIdx);
	DDX_Text(pDX, IDC_PLAYSCRIPT_IDX, m_strPlayScriptIdx);
	DDX_Text(pDX, IDC_PLAYSCRIPT_SCENE_IDX, m_strPlayScriptSceneIdx);
	DDX_Control(pDX, IDC_COMBO1, m_cbMoveType);
	DDX_Text(pDX, IDC_AISCRIPT_IDX, m_strAIScriptIdx);
	DDX_Text(pDX, IDC_AISCRIPT_SCENE_IDX, m_strAIScriptSceneIdx);
}


BEGIN_MESSAGE_MAP(CPaletteSpawnSearch, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHLIST, &CPaletteSpawnSearch::OnLvnItemchangedPathlist)
	ON_CBN_SELCHANGE(IDC_LIST_OPTION, &CPaletteSpawnSearch::OnCbnSelchangeListOption)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SPAWN_LIST, &CPaletteSpawnSearch::OnLvnItemchangedSpawnList)
	ON_BN_CLICKED(IDC_CREATE, &CPaletteSpawnSearch::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_DELETE, &CPaletteSpawnSearch::OnBnClickedDelete)
	ON_BN_CLICKED(IDCANCEL, &CPaletteSpawnSearch::OnBnClickedCancel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SPAWN_LIST, &CPaletteSpawnSearch::OnLvnColumnclickSpawnList)
	ON_BN_CLICKED(IDC_PORTAL, &CPaletteSpawnSearch::OnBnClickedPortal)
	ON_BN_CLICKED(IDC_INDEX_OK, &CPaletteSpawnSearch::OnBnClickedIndexOk)
	ON_BN_CLICKED(IDC_NEST_COUNT_OK, &CPaletteSpawnSearch::OnBnClickedNestCountOk)
	ON_BN_CLICKED(IDC_COOL_TIME_OK, &CPaletteSpawnSearch::OnBnClickedCoolTimeOk)
	ON_BN_CLICKED(IDC_MOVE_TYPE_OK, &CPaletteSpawnSearch::OnBnClickedMoveTypeOk)
	ON_BN_CLICKED(IDC_WANDER_RANGE_OK, &CPaletteSpawnSearch::OnBnClickedWanderRangeOk)
	ON_BN_CLICKED(IDC_MOVE_RANGE_OK, &CPaletteSpawnSearch::OnBnClickedMoveRangeOk)
	ON_BN_CLICKED(IDC_DELAY_TIME_OK, &CPaletteSpawnSearch::OnBnClickedDelayTimeOk)
	ON_BN_CLICKED(IDC_PATH_INDEX_OK, &CPaletteSpawnSearch::OnBnClickedPathIndexOk)
	ON_BN_CLICKED(IDC_SPAWN_GROUP_IDX_OK, &CPaletteSpawnSearch::OnBnClickedSpawnGroupIdxOk)
	ON_BN_CLICKED(IDC_PLAYSCRIPT_IDX_OK, &CPaletteSpawnSearch::OnBnClickedPlayscriptIdxOk)
	ON_BN_CLICKED(IDC_PLAYSCRIPT_SCENE_IDX_OK, &CPaletteSpawnSearch::OnBnClickedPlayscriptSceneIdxOk)
	ON_BN_CLICKED(IDC_ALL_OK, &CPaletteSpawnSearch::OnBnClickedAllOk)
	ON_BN_CLICKED(IDC_AISCRIPT_IDX_OK, &CPaletteSpawnSearch::OnBnClickedAiscriptIdxOk)
	ON_BN_CLICKED(IDC_AISCRIPT_SCENE_IDX_OK, &CPaletteSpawnSearch::OnBnClickedAiscriptSceneIdxOk)
	ON_BN_CLICKED(IDC_EXPORT_SPAWN_LIST, &CPaletteSpawnSearch::OnBnClickedExportSpawnList)
	ON_BN_CLICKED(IDC_EXPORT_RULE_OUT, &CPaletteSpawnSearch::OnBnClickedExportRuleOut)
END_MESSAGE_MAP()

// CPaletteSpawnSearch 메시지 처리기입니다.
BOOL CPaletteSpawnSearch::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->wParam)
	{
	case VK_ESCAPE:
		{
			return true;
		}
	case VK_RETURN:
		{
			return true;
		}
	default:
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPaletteSpawnSearch::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);

	m_SpawnListOptionCombo.SetCurSel(0);

	m_SpawnList.InsertColumn(ESPAWNLIST_FIELD_NUMBER, "Field Number", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_INDEX, "Index", LVCFMT_CENTER, 40);
	m_SpawnList.InsertColumn(ESPAWNLIST_NAME, "Name", LVCFMT_CENTER, 40);
	m_SpawnList.InsertColumn(ESPAWNLIST_LEVEL, "Level", LVCFMT_CENTER, 40);
	m_SpawnList.InsertColumn(ESPAWNLIST_POSITION, "Position", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_NEST_COUNT, "Nest Count", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_COOL_TIME, "Cool Time", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_MOVE_TYPE, "Move Type", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_WANDER_RANGE, "Wander Range", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_MOVE_RANGE, "Move Range", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_DELAY_TIME, "Delay Time", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_PATH_INDEX, "Path Index", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_SPAWN_GROUP_INDEX, "Spawn Group Idx", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_PLAY_SCRIPT_INDEX, "PlayScript Idx", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX, "PlayScript Scene Idx", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_AI_SCRIPT_INDEX, "AIScript Idx", LVCFMT_CENTER, 80);
	m_SpawnList.InsertColumn(ESPAWNLIST_AI_SCRIPT_SCENE_INDEX, "AIScript Scene Idx", LVCFMT_CENTER, 80);
	m_SpawnList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPaletteSpawnSearch::OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CPaletteSpawnSearch::OnCbnSelchangeListOption()
{
}

void CPaletteSpawnSearch::OnLvnItemchangedSpawnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED | LVIS_SELECTED)))
	{
		RefreshSpawnCount();
		ReFreshModification();
	}

	*pResult = 0;
}

void CPaletteSpawnSearch::OnBnClickedCreate()
{
	RefreshSpawnList();
}

void CPaletteSpawnSearch::OnBnClickedDelete()
{
	RwUInt32				SelCount		= m_SpawnList.GetSelectedCount();
	RwInt32					FirstItemSymbol	= -1;

	if(SelCount > 0)
	{
		for(RwUInt32 i = 0; i < SelCount; i++)
		{
			FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
			if(FirstItemSymbol != -1)
			{
				CString _StrField	= m_SpawnList.GetItemText(FirstItemSymbol, 0);
				CString _StrIndex	= m_SpawnList.GetItemText(FirstItemSymbol, 1);
				CString _StrName	= m_SpawnList.GetItemText(FirstItemSymbol, 2);
				CString _StrLevel	= m_SpawnList.GetItemText(FirstItemSymbol, 3);
				CString _StrPos		= m_SpawnList.GetItemText(FirstItemSymbol, 4);
				CString _StrCNM		= m_SpawnList.GetItemText(FirstItemSymbol, 5);

				RwInt32	_ByField	= static_cast<RwInt32>(atoi(_StrField.GetBuffer(0)));
				RwInt32	_ByIndex	= static_cast<RwInt32>(atoi(_StrIndex.GetBuffer(0)));
				RwInt32	_ByLevel	= static_cast<RwInt32>(atoi(_StrLevel.GetBuffer(0)));
				RwInt32	_ByCNM		= static_cast<RwInt32>(atoi(_StrCNM.GetBuffer(0)));
				RwV3d	_ByPos;

				RwInt32 TmpIndex = _StrPos.Find(',');
				CString _StrX = _StrPos.Left(TmpIndex);	_StrPos.Delete(0, TmpIndex + 2);

				TmpIndex = _StrPos.Find(',');
				CString _StrY = _StrPos.Left(TmpIndex);	_StrPos.Delete(0, TmpIndex + 2);

				CString _StrZ = _StrPos;

				_ByPos.x = static_cast<RwReal>(atof(_StrX.GetBuffer(0)));
				_ByPos.y = static_cast<RwReal>(atof(_StrY.GetBuffer(0))); 
				_ByPos.z = static_cast<RwReal>(atof(_StrZ.GetBuffer(0))); 

				//GetSpawnInstance()->OnDeleteSpawn(_ByField, _ByIndex, _StrName.GetBuffer(0), _ByLevel, _ByPos, _ByCNM);
				GetSpawnInstance()->OnDeleteSpawn(_ByPos);
			}
		}
	}

	RefreshSpawnList();
}

void CPaletteSpawnSearch::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

VOID CPaletteSpawnSearch::InsertNode(RwInt32 _FieldNum, RwInt32 _Index, string& Name, RwInt32 _SpawnLvl, RwV3d& _Pos, void* pData)
{
	LVITEM	lvi;
 	CString SpawnData[ESPAWNLIST_NUM];

	CString Tmp		= Name.c_str();
	RwInt32 Idx		= static_cast<RwInt32>(Tmp.Find('('));
	CSpawn* pSpawn	= static_cast<CSpawn*>(pData);


	SpawnData[ESPAWNLIST_FIELD_NUMBER].Format("%d", _FieldNum);
	SpawnData[ESPAWNLIST_INDEX].Format("%d", _Index);
	SpawnData[ESPAWNLIST_NAME] = Tmp.Left(Idx);
	SpawnData[ESPAWNLIST_LEVEL].Format("%d", _SpawnLvl);
	SpawnData[ESPAWNLIST_POSITION].Format("%.2f, %.2f, %.2f", _Pos.x, _Pos.y, _Pos.z);

	SpawnData[ESPAWNLIST_NEST_COUNT].Format("%d", pSpawn->GetNestMemberCnt());
	SpawnData[ESPAWNLIST_COOL_TIME].Format("%d", pSpawn->GetSpawnMacro().m_CoolTime);
	SpawnData[ESPAWNLIST_MOVE_TYPE].Format("%d", pSpawn->GetSpawnMacro().m_MoveType);
	SpawnData[ESPAWNLIST_WANDER_RANGE].Format("%.2f", pSpawn->GetSpawnMacro().m_WanderRange);
	SpawnData[ESPAWNLIST_MOVE_RANGE].Format("%.2f", pSpawn->GetSpawnMacro().m_MoveRange);
	SpawnData[ESPAWNLIST_DELAY_TIME].Format("%.2f", pSpawn->GetSpawnMacro().m_DelayTime);
	SpawnData[ESPAWNLIST_PATH_INDEX].Format("%d", pSpawn->GetPathIndex());
	SpawnData[ESPAWNLIST_SPAWN_GROUP_INDEX].Format("%u", pSpawn->GetGroupIdx());
	SpawnData[ESPAWNLIST_PLAY_SCRIPT_INDEX].Format("%u", pSpawn->GetPlayScriptIdx());
	SpawnData[ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX].Format("%u", pSpawn->GetPlayScriptSceneIdx());
	SpawnData[ESPAWNLIST_AI_SCRIPT_INDEX].Format("%u", pSpawn->GetAIScriptIdx());
	SpawnData[ESPAWNLIST_AI_SCRIPT_SCENE_INDEX].Format("%u", pSpawn->GetAIScriptSceneIdx());

	lvi.mask		= LVIF_TEXT;
	lvi.iItem		= m_SpawnList.GetItemCount();
	lvi.iImage		= 0;
	for (int i = 0; i < ESPAWNLIST_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(SpawnData[i].GetBuffer(0));
		if (i == ESPAWNLIST_FIELD_NUMBER)
		{
			m_SpawnList.SetItemData(m_SpawnList.InsertItem(&lvi), (DWORD_PTR)pData);
		}
		else
		{
			m_SpawnList.SetItem(&lvi);			
		}
	}

	for (int i = 0; i < ESPAWNLIST_NUM; ++i)
	{
		m_SpawnList.SetColumnWidth(i, LVSCW_AUTOSIZE);
	}
}

VOID CPaletteSpawnSearch::RefreshSpawnListItem(RwInt32 iItem, CSpawn* pSpawn)
{
	RwInt32 Idx;
	CString Tmp		= pSpawn->m_Name.c_str();
	RwInt32 IdxF	= static_cast<RwInt32>(Tmp.Find('('));
	RwInt32 IdxR	= static_cast<RwInt32>(Tmp.Find("레"));
	RwInt32 IdxDiff	= IdxR - IdxF;
	CString Level	= Tmp;

	Level.Delete(0, IdxF + 1);

	RwInt32 CntNumber	= static_cast<RwInt32>(Level.Find("레"));
	Level				= Level.Left(CntNumber);
	Tmp					= pSpawn->m_Name.c_str();
	Idx					= static_cast<RwInt32>(Tmp.Find('('));

	CString SpawnData[ESPAWNLIST_NUM];

	SpawnData[ESPAWNLIST_FIELD_NUMBER].Format("%d", dGETMGR()->GetFieldIdx(pSpawn->m_T));
	SpawnData[ESPAWNLIST_INDEX].Format("%u", pSpawn->m_MdlGUID);
	SpawnData[ESPAWNLIST_NAME] = Tmp.Left(Idx);
	SpawnData[ESPAWNLIST_LEVEL] = Level;
	SpawnData[ESPAWNLIST_POSITION].Format("%.2f, %.2f, %.2f", pSpawn->m_T.x, pSpawn->m_T.y, pSpawn->m_T.z);
	SpawnData[ESPAWNLIST_NEST_COUNT].Format("%d", pSpawn->GetNestMemberCnt());
	SpawnData[ESPAWNLIST_COOL_TIME].Format("%d", pSpawn->GetSpawnMacro().m_CoolTime);
	SpawnData[ESPAWNLIST_MOVE_TYPE].Format("%d", pSpawn->GetSpawnMacro().m_MoveType);
	SpawnData[ESPAWNLIST_WANDER_RANGE].Format("%.2f", pSpawn->GetSpawnMacro().m_WanderRange);
	SpawnData[ESPAWNLIST_MOVE_RANGE].Format("%.2f", pSpawn->GetSpawnMacro().m_MoveRange);
	SpawnData[ESPAWNLIST_DELAY_TIME].Format("%.2f", pSpawn->GetSpawnMacro().m_DelayTime);
	SpawnData[ESPAWNLIST_PATH_INDEX].Format("%d", pSpawn->GetPathIndex());
	SpawnData[ESPAWNLIST_SPAWN_GROUP_INDEX].Format("%u", pSpawn->GetGroupIdx());
	SpawnData[ESPAWNLIST_PLAY_SCRIPT_INDEX].Format("%u", pSpawn->GetPlayScriptIdx());
	SpawnData[ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX].Format("%u", pSpawn->GetPlayScriptSceneIdx());
	SpawnData[ESPAWNLIST_AI_SCRIPT_INDEX].Format("%u", pSpawn->GetAIScriptIdx());
	SpawnData[ESPAWNLIST_AI_SCRIPT_SCENE_INDEX].Format("%u", pSpawn->GetAIScriptSceneIdx());

	for (int i = 0; i < ESPAWNLIST_NUM; ++i)
	{
		m_SpawnList.SetItemText(iItem, i, (LPSTR)(SpawnData[i].GetBuffer(0)));
	}
}

RwBool CPaletteSpawnSearch::RefreshSpawnList()
{
	UpdateData(TRUE);

	m_SpawnList.DeleteAllItems();

	RwInt32 DatumMode	= m_SpawnListOptionCombo.GetCurSel();
	RwInt32 MinLvl		= atoi(m_StrMinLvl.GetBuffer(0));
	RwInt32 MaxLvl		= atoi(m_StrMaxLvl.GetBuffer(0));
	RwInt32	SpawnIdx	= atoi(m_StrSpawnIndex.GetBuffer(0));

	// by level
	if(DatumMode == 0)
	{
		if(MinLvl < 0 || !MinLvl || MaxLvl < 0 || !MaxLvl)
		{
			AfxMessageBox("Min Level, Max Level can't be negative values or zero.");
			return FALSE;
		}
	}
	// by index
	else if(DatumMode == 1)
	{
		if(!SpawnIdx)
		{
			AfxMessageBox("MOB or NPC index can't be zero.");
			return FALSE;
		}
	}

	CSpawnContainer*						pSC		= GetSpawnInstance()->GetSpawnContainer();
	CSpawnContainer::SPAWN_INST_NODE_ITER	The		= pSC->SIN_BEGIN();
	RwInt32									iNum	= 0;
	RwInt32									iIndex	= 0;

	while(The != pSC->SIN_END())
	{
		CSpawnInstNode*				pSIN		= The->second;
		vector<CSpawn*>::iterator	TheSpawn	= pSIN->BEGIN();

		while(TheSpawn != pSIN->END())
		{
			++iNum;
			++TheSpawn;
		}
		++The;
	}

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Spawn");
	WndProgress.GoModal();
	WndProgress.SetRange(0, iNum);	

	The	= pSC->SIN_BEGIN();
	while(The != pSC->SIN_END())
	{
		CSpawnInstNode*				pSIN		= The->second;
		vector<CSpawn*>::iterator	TheSpawn	= pSIN->BEGIN();

		while(TheSpawn != pSIN->END())
		{
			CSpawn* pCurSpawn = (*TheSpawn);

			// get a current level
			CString Tmp		= pCurSpawn->m_Name.c_str();
			RwInt32 IdxF	= static_cast<RwInt32>(Tmp.Find('('));
			RwInt32 IdxR	= static_cast<RwInt32>(Tmp.Find("레"));
			RwInt32 IdxDiff	= IdxR - IdxF;
			CString Level	= Tmp;

			Level.Delete(0, IdxF + 1);
			RwInt32 CntNumber = static_cast<RwInt32>(Level.Find("레"));
			Level = Level.Left(CntNumber);

			RwInt32 CurLvl	= static_cast<RwInt32>(atoi(Level.GetBuffer(0)));

			if(DatumMode == 0)
			{
				if(CurLvl >= MinLvl && CurLvl <= MaxLvl)
				{
					InsertNode(The->first, pCurSpawn->m_MdlGUID, pCurSpawn->m_Name, CurLvl, pCurSpawn->m_T, pCurSpawn);
				}
			}
			else if(DatumMode == 1)
			{
				if(static_cast<RwInt32>(pCurSpawn->m_MdlGUID) == SpawnIdx)
				{
					InsertNode(The->first, pCurSpawn->m_MdlGUID, pCurSpawn->m_Name, CurLvl, pCurSpawn->m_T, pCurSpawn);
				}
			}
			else if(DatumMode == 2)
			{
				if (pCurSpawn->GetSpawnType() == eSPAWN_NPC)
				{
					RwInt32 CheckIdx = static_cast<RwInt32>(pCurSpawn->m_MdlGUID);
					RwInt32	CheckCnt = 0;

					CSpawnContainer*						pSCCheck = GetSpawnInstance()->GetSpawnContainer();
					CSpawnContainer::SPAWN_INST_NODE_ITER	TheCheck = pSCCheck->SIN_BEGIN();
					while(TheCheck != pSCCheck->SIN_END())
					{
						CSpawnInstNode*				pSINCheck		= TheCheck->second;
						vector<CSpawn*>::iterator	TheSpawnCheck	= pSINCheck->BEGIN();
						while(TheSpawnCheck != pSINCheck->END())
						{
							CSpawn* pCurSpawnCheck = (*TheSpawnCheck);
							if (pCurSpawn->GetSpawnType() == eSPAWN_NPC)
							{
								if (CheckIdx == static_cast<RwInt32>(pCurSpawnCheck->m_MdlGUID))
								{
									if (++CheckCnt > 1)
									{
										break;
									}
								}
							}
							++TheSpawnCheck;
						}
						if (CheckCnt > 1)
						{
							break;
						}
						++TheCheck;
					}

					if (CheckCnt > 1)
					{
						InsertNode(The->first, pCurSpawn->m_MdlGUID, pCurSpawn->m_Name, CurLvl, pCurSpawn->m_T, pCurSpawn);
					}
				}
			}
			else if (DatumMode == 3)
			{
				if (pCurSpawn->GetSpawnType() == eSPAWN_MOB)
				{
					CMobTable*		pMobTbl		= static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
					sMOB_TBLDAT*	pTlbData	= static_cast<sMOB_TBLDAT*>(pMobTbl->FindData(pCurSpawn->m_MdlGUID));

					if (pTlbData && pTlbData->byGrade > 0)
					{
						InsertNode(The->first, pCurSpawn->m_MdlGUID, pCurSpawn->m_Name, CurLvl, pCurSpawn->m_T, pCurSpawn);
					}				
				}
			}

			RwChar cText[64];
			::sprintf_s(cText, 64, "%d / %d Searching", iIndex++, iNum);
			WndProgress.SetText(cText);
			WndProgress.StepIt();
			WndProgress.PeekAndPump();

			++TheSpawn;
		}

		++The;
	}

	WndProgress.SetText("Done.");

	RefreshSpawnCount();

	UpdateData(FALSE);

	return TRUE;
}

VOID CPaletteSpawnSearch::RefreshSpawnCount()
{
	RwUInt32	SelCount			= m_SpawnList.GetSelectedCount();
	RwInt32		FirstItemSymbol		= -1;
	RwInt32		NumAllNestMemberCnt	= 0;
	RwInt32		NumSelNestMemberCnt	= 0;
	CString		strNMC;

	// count all nest members
	for(RwInt32 i = 0; i < m_SpawnList.GetItemCount(); i++)
	{
		strNMC				=	m_SpawnList.GetItemText(i, 5);
		NumAllNestMemberCnt	+=	static_cast<RwInt32>(atoi(strNMC.GetBuffer(0)));
	}

	// count the nest member selected
	if(SelCount > 0)
	{
		for(RwUInt32 i = 0; i < SelCount; i++)
		{
			FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
			if(FirstItemSymbol != -1)
			{
				strNMC = m_SpawnList.GetItemText(FirstItemSymbol, 5);
				NumSelNestMemberCnt += static_cast<RwInt32>(atoi(strNMC.GetBuffer(0)));
			}
		}
	}

	m_StrSpawnCountInfo.Format("(%d / %d)", NumSelNestMemberCnt, NumAllNestMemberCnt);

	UpdateData(FALSE);
}

VOID CPaletteSpawnSearch::ReFreshModification()
{
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	if (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			m_strFieldNumber.Format("%d", dGETMGR()->GetFieldIdx(pSpawn->m_T));
			m_strIndex.Format("%u",pSpawn->m_MdlGUID);
			m_strNestCount.Format("%d", pSpawn->GetNestMemberCnt());
			m_strCoolTime.Format("%d", pSpawn->GetSpawnMacro().m_CoolTime);
			m_cbMoveType.SetCurSel(pSpawn->GetSpawnMacro().m_MoveType);
			m_strWanderRange.Format("%.2f", pSpawn->GetSpawnMacro().m_WanderRange);
			m_strMoveRange.Format("%.2f", pSpawn->GetSpawnMacro().m_MoveRange);
			m_strDelayTime.Format("%.2f", pSpawn->GetSpawnMacro().m_DelayTime);
			m_strPathIndex.Format("%d", pSpawn->GetPathIndex());
			m_strSpawnGroupIdx.Format("%u", pSpawn->GetGroupIdx());
			m_strPlayScriptIdx.Format("%u", pSpawn->GetPlayScriptIdx());
			m_strPlayScriptSceneIdx.Format("%u", pSpawn->GetPlayScriptSceneIdx());			
			m_strAIScriptIdx.Format("%u", pSpawn->GetAIScriptIdx());
			m_strAIScriptSceneIdx.Format("%u", pSpawn->GetAIScriptSceneIdx());
		}
		//FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

RwBool CPaletteSpawnSearch::OnSortData(RwInt32 _Index)
{
	CListCtrl*	pLc			= (CListCtrl*)GetDlgItem(IDC_SPAWN_LIST);
	RwInt32		totItems	= pLc->GetItemCount();

	// Callback Item Setting
	for(RwInt32 i = 0; i < totItems; i++)
	{
		DWORD	dw	= pLc->GetItemData(i);
		CString txt = pLc->GetItemText(i, _Index);

		pLc->SetItemData(i, (DWORD)(new CSortData(txt, dw)));
	}

	if(_Index == 2 || _Index == 4)
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

void CPaletteSpawnSearch::OnLvnColumnclickSpawnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if(!OnSortData(pNMLV->iSubItem))
		return;

	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CPaletteSpawnSearch::OnBnClickedPortal()
{
	RwUInt32 SelCount = m_SpawnList.GetSelectedCount();

	if(SelCount > 1)
	{
		AfxMessageBox("can't portal to multi path points please choose one path point.");
		return;
	}

	if(!SelCount)
	{
		AfxMessageBox("please choose a path point.");
		return;
	}

	RwInt32 SelIndex = m_SpawnList.GetSelectionMark();
	RwInt32	FirstItemSymbol	= -1;

	FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	if(FirstItemSymbol != -1)
	{
		CString _StrPos	 = m_SpawnList.GetItemText(FirstItemSymbol, 4);
		RwV3d	_ByPos;

		RwInt32 TmpIndex	= _StrPos.Find(',');
		CString _StrX		= _StrPos.Left(TmpIndex);
		_StrPos.Delete(0, TmpIndex + 2);

		TmpIndex = _StrPos.Find(',');
		CString _StrY = _StrPos.Left(TmpIndex);
		_StrPos.Delete(0, TmpIndex + 2);

		CString _StrZ = _StrPos;

		_ByPos.x = static_cast<RwReal>(atof(_StrX.GetBuffer(0)));
		_ByPos.y = static_cast<RwReal>(atof(_StrY.GetBuffer(0))); 
		_ByPos.z = static_cast<RwReal>(atof(_StrZ.GetBuffer(0))); 

		GetSpawnInstance()->OnPortalTo(_ByPos);

		// force the camera to be free
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
	}
}

LRESULT CPaletteSpawnSearch::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			break;
		}
	case WM_CLOSE:
		{
			dGETFRM()->m_pSpawnPalette->ShowWindow(SW_SHOW);
			dGETFRM()->m_pSpawnPalette->m_PathDialogOpend = FALSE;
			dGETFRM()->m_pSpawnPalette->ShowWindow(SW_SHOW);
			dGETFRM()->m_pSpawnPalette->m_NavigatorDialogOpend = FALSE;
			GetSpawnInstance()->SetPathModeActive(FALSE);
			GetSpawnInstance()->SetNavigatorModeActive(FALSE);
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
void CPaletteSpawnSearch::OnBnClickedIndexOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnModel(pSpawn, atoi(m_strIndex));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedNestCountOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnNestCount(pSpawn, atoi(m_strNestCount));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedCoolTimeOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnCoolTime(pSpawn, atoi(m_strCoolTime));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedMoveTypeOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnMoveType(pSpawn, m_cbMoveType.GetCurSel());
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}

	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedWanderRangeOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnWanderRange(pSpawn, static_cast<RwReal>(atof(m_strWanderRange)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedMoveRangeOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnMoveRange(pSpawn,  static_cast<RwReal>(atof(m_strMoveRange)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedDelayTimeOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnDelayTime(pSpawn,  static_cast<RwReal>(atof(m_strDelayTime)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedPathIndexOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnPathIndex(pSpawn, atoi(m_strPathIndex));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedSpawnGroupIdxOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnGroupIndex(pSpawn, static_cast<DWORD>(atoi(m_strSpawnGroupIdx)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedPlayscriptIdxOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnPlayScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strPlayScriptIdx)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedPlayscriptSceneIdxOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnPlayScriptSceneIndex(pSpawn,  static_cast<DWORD>(atoi(m_strPlayScriptSceneIdx)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedAllOk()
{
	UpdateData(TRUE);
	if (AfxMessageBox("Do you want modify data?", MB_YESNO) == IDYES)
	{
		int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
		while (FirstItemSymbol != -1)
		{
			CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
			if (pSpawn != NULL)
			{
				GetSpawnInstance()->ChangeSpawnModel(pSpawn, atoi(m_strIndex));
				GetSpawnInstance()->ChangeSpawnNestCount(pSpawn, atoi(m_strNestCount));
				GetSpawnInstance()->ChangeSpawnCoolTime(pSpawn, atoi(m_strCoolTime));
				GetSpawnInstance()->ChangeSpawnMoveType(pSpawn, m_cbMoveType.GetCurSel());
				GetSpawnInstance()->ChangeSpawnWanderRange(pSpawn, static_cast<RwReal>(atof(m_strWanderRange)));
				GetSpawnInstance()->ChangeSpawnMoveRange(pSpawn,  static_cast<RwReal>(atof(m_strMoveRange)));
				GetSpawnInstance()->ChangeSpawnDelayTime(pSpawn,  static_cast<RwReal>(atof(m_strDelayTime)));
				GetSpawnInstance()->ChangeSpawnPathIndex(pSpawn, atoi(m_strPathIndex));
				GetSpawnInstance()->ChangeSpawnGroupIndex(pSpawn, static_cast<DWORD>(atoi(m_strSpawnGroupIdx)));
				GetSpawnInstance()->ChangeSpawnPlayScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strPlayScriptIdx)));
				GetSpawnInstance()->ChangeSpawnPlayScriptSceneIndex(pSpawn,  static_cast<DWORD>(atoi(m_strPlayScriptSceneIdx)));
				GetSpawnInstance()->ChangeSpawnAIScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strAIScriptIdx)));
				GetSpawnInstance()->ChangeSpawnAIScriptSceneIndex(pSpawn,  static_cast<DWORD>(atoi(m_strAIScriptSceneIdx)));
				RefreshSpawnListItem(FirstItemSymbol, pSpawn);
			}
			FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
		}
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedAiscriptIdxOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnAIScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strAIScriptIdx)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedAiscriptSceneIdxOk()
{
	UpdateData(TRUE);
	int FirstItemSymbol = m_SpawnList.GetNextItem(-1, LVNI_SELECTED);
	while (FirstItemSymbol != -1)
	{
		CSpawn* pSpawn = (CSpawn*)m_SpawnList.GetItemData(FirstItemSymbol);
		if (pSpawn != NULL)
		{
			GetSpawnInstance()->ChangeSpawnAIScriptSceneIndex(pSpawn, static_cast<DWORD>(atoi(m_strAIScriptSceneIdx)));
			RefreshSpawnListItem(FirstItemSymbol, pSpawn);
		}
		FirstItemSymbol = m_SpawnList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
	}
	UpdateData(FALSE);
}

void CPaletteSpawnSearch::OnBnClickedExportSpawnList()
{
	if(AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
	{
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "export files", "*.txt", OFN_HIDEREADONLY, "Export Files (*.txt)|*.txt||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// tool
	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetPathName().GetBuffer(0);

	if(::fopen_s(&pFile, FileTitle.c_str(), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}

	RwInt32 iNum = m_SpawnList.GetItemCount();

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Export");
	WndProgress.GoModal();
	WndProgress.SetRange(0, iNum);	

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		for (int iField = 0; iField < ESPAWNLIST_NUM; ++iField)
		{
			CString str = m_SpawnList.GetItemText(i, iField);
			fprintf_s(pFile, "%s\t", str.GetBuffer(0));
		}
		fprintf_s(pFile, "\r\n");

		RwChar cText[64];
		::sprintf_s(cText, 64, "%d / %d export", i, iNum);
		WndProgress.SetText(cText);
		WndProgress.StepIt();
		WndProgress.PeekAndPump();
	}
	fclose(pFile);

	::SetCurrentDirectory(dirBackup);

	WndProgress.SetText("Done.");
}

void CPaletteSpawnSearch::OnBnClickedExportRuleOut()
{
	if(AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
	{
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "export files", "*.txt", OFN_HIDEREADONLY, "Export Files (*.txt)|*.txt||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// tool
	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetPathName().GetBuffer(0);

	if(::fopen_s(&pFile, FileTitle.c_str(), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << FileTitle.c_str() << ")");
		::SetCurrentDirectory(dirBackup);
		return;
	}
	::SetCurrentDirectory(dirBackup);

	RwV3d vOldPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());

	CSpawnContainer*						pSC		= GetSpawnInstance()->GetSpawnContainer();
	CSpawnContainer::SPAWN_INST_NODE_ITER	The		= pSC->SIN_BEGIN();

	RwInt32 iCnt = 0;
	RwInt32 iNum = 0;
	while (The != pSC->SIN_END())
	{
		++iNum;
		++The;
	}

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Export");
	WndProgress.GoModal();
	WndProgress.SetRange(0, iNum);	

	The	= pSC->SIN_BEGIN();
	while (The != pSC->SIN_END())
	{
		CSpawnInstNode*				pSIN		= The->second;
		vector<CSpawn*>::iterator	TheSpawn	= pSIN->BEGIN();

		RwInt32 iFieldIdx	= The->first;
		RwV3d	MPos		= pFields[iFieldIdx].GetSPos();

		// move to new field area and update(camera, app.)
		MPos.x	+=	dGET_WORLD_PARAM()->WorldSectorSize;
		MPos.z	+=	dGET_WORLD_PARAM()->WorldSectorSize;
		MPos.y	=	(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))).y;

		pFieldMgr->SetAnotherField(TRUE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(MPos);
		CNtlWeControlUi::GetInstance().Update();
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.OnIdle();

// 		while(TheSpawn != pSIN->END())
// 		{
// 			CSpawn* pCurSpawn = (*TheSpawn);
// 
// 			CNtlPLGlobal::m_pWHEntity	= NULL;
// 			RwV3d vTestPos				= pCurSpawn->m_T; vTestPos.y += 512.0f;
// 			if (GetSceneManager()->GetWorldHeight(&vTestPos, vTestPos.y, NULL))
// 			{
// 				if (vTestPos.y > pCurSpawn->m_T.y)
// 				{
// 					CString Tmp		= pCurSpawn->m_Name.c_str();
// 					RwInt32 IdxF	= static_cast<RwInt32>(Tmp.Find('('));
// 					RwInt32 IdxR	= static_cast<RwInt32>(Tmp.Find("레"));
// 					RwInt32 IdxDiff	= IdxR - IdxF;
// 					CString Level	= Tmp;
// 
// 					Level.Delete(0, IdxF + 1);
// 					RwInt32 CntNumber = static_cast<RwInt32>(Level.Find("레"));
// 					Level = Level.Left(CntNumber);					
// 
// 					RwInt32 CurLvl	= static_cast<RwInt32>(atoi(Level.GetBuffer(0)));
// 
// 					CString Tmp2	= pCurSpawn->m_Name.c_str();
// 					RwInt32 Idx		= static_cast<RwInt32>(Tmp.Find('('));
// 
// 					CString SpawnData[ESPAWNLIST_NUM];
// 
// 					SpawnData[ESPAWNLIST_FIELD_NUMBER].Format("%d", iFieldIdx);
// 					SpawnData[ESPAWNLIST_INDEX].Format("%d", pCurSpawn->m_MdlGUID);
// 					SpawnData[ESPAWNLIST_NAME] = Tmp2.Left(Idx);
// 					SpawnData[ESPAWNLIST_LEVEL].Format("%d", CurLvl);
// 					SpawnData[ESPAWNLIST_POSITION].Format("%.2f, %.2f, %.2f", pCurSpawn->m_T.x, pCurSpawn->m_T.y, pCurSpawn->m_T.z);
// 
// 					SpawnData[ESPAWNLIST_NEST_COUNT].Format("%d", pCurSpawn->GetNestMemberCnt());
// 					SpawnData[ESPAWNLIST_COOL_TIME].Format("%d", pCurSpawn->GetSpawnMacro().m_CoolTime);
// 					SpawnData[ESPAWNLIST_MOVE_TYPE].Format("%d", pCurSpawn->GetSpawnMacro().m_MoveType);
// 					SpawnData[ESPAWNLIST_WANDER_RANGE].Format("%.2f", pCurSpawn->GetSpawnMacro().m_WanderRange);
// 					SpawnData[ESPAWNLIST_MOVE_RANGE].Format("%.2f", pCurSpawn->GetSpawnMacro().m_MoveRange);
// 					SpawnData[ESPAWNLIST_DELAY_TIME].Format("%.2f", pCurSpawn->GetSpawnMacro().m_DelayTime);
// 					SpawnData[ESPAWNLIST_PATH_INDEX].Format("%d", pCurSpawn->GetPathIndex());
// 					SpawnData[ESPAWNLIST_SPAWN_GROUP_INDEX].Format("%u", pCurSpawn->GetGroupIdx());
// 					SpawnData[ESPAWNLIST_PLAY_SCRIPT_INDEX].Format("%u", pCurSpawn->GetPlayScriptIdx());
// 					SpawnData[ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX].Format("%u", pCurSpawn->GetPlayScriptSceneIdx());
// 					SpawnData[ESPAWNLIST_AI_SCRIPT_INDEX].Format("%u", pCurSpawn->GetAIScriptIdx());
// 					SpawnData[ESPAWNLIST_AI_SCRIPT_SCENE_INDEX].Format("%u", pCurSpawn->GetAIScriptSceneIdx());
// 
// 					for (RwInt32 iDataFields = 0; iDataFields < ESPAWNLIST_NUM; ++iDataFields)
// 					{
// 						fprintf_s(pFile, "%s\t", SpawnData[iDataFields].GetBuffer(0));
// 					}
// 					if (CNtlPLGlobal::m_pWHEntity)
// 					{
// 						fprintf_s(pFile, "%s\t", CNtlPLGlobal::m_pWHEntity->GetName());
// 					}
// 					fprintf_s(pFile, "\r\n");
// 				}
// 			}
// 
// 			++TheSpawn;
// 		}

		while(TheSpawn != pSIN->END())
		{
			CSpawn* pCurSpawn = (*TheSpawn);
			RwReal	fHeight;
			
			if (GetSceneManager()->GetTerrainHeight(&pCurSpawn->m_T, fHeight))
			{
				if (fHeight > pCurSpawn->m_T.y)
				{
					CString Tmp		= pCurSpawn->m_Name.c_str();
					RwInt32 IdxF	= static_cast<RwInt32>(Tmp.Find('('));
					RwInt32 IdxR	= static_cast<RwInt32>(Tmp.Find("레"));
					RwInt32 IdxDiff	= IdxR - IdxF;
					CString Level	= Tmp;

					Level.Delete(0, IdxF + 1);
					RwInt32 CntNumber = static_cast<RwInt32>(Level.Find("레"));
					Level = Level.Left(CntNumber);					

					RwInt32 CurLvl	= static_cast<RwInt32>(atoi(Level.GetBuffer(0)));

					CString Tmp2	= pCurSpawn->m_Name.c_str();
					RwInt32 Idx		= static_cast<RwInt32>(Tmp.Find('('));

					CString SpawnData[ESPAWNLIST_NUM];

					SpawnData[ESPAWNLIST_FIELD_NUMBER].Format("%d", iFieldIdx);
					SpawnData[ESPAWNLIST_INDEX].Format("%d", pCurSpawn->m_MdlGUID);
					SpawnData[ESPAWNLIST_NAME] = Tmp2.Left(Idx);
					SpawnData[ESPAWNLIST_LEVEL].Format("%d", CurLvl);
					SpawnData[ESPAWNLIST_POSITION].Format("%.2f, %.2f, %.2f", pCurSpawn->m_T.x, pCurSpawn->m_T.y, pCurSpawn->m_T.z);

					SpawnData[ESPAWNLIST_NEST_COUNT].Format("%d", pCurSpawn->GetNestMemberCnt());
					SpawnData[ESPAWNLIST_COOL_TIME].Format("%d", pCurSpawn->GetSpawnMacro().m_CoolTime);
					SpawnData[ESPAWNLIST_MOVE_TYPE].Format("%d", pCurSpawn->GetSpawnMacro().m_MoveType);
					SpawnData[ESPAWNLIST_WANDER_RANGE].Format("%.2f", pCurSpawn->GetSpawnMacro().m_WanderRange);
					SpawnData[ESPAWNLIST_MOVE_RANGE].Format("%.2f", pCurSpawn->GetSpawnMacro().m_MoveRange);
					SpawnData[ESPAWNLIST_DELAY_TIME].Format("%.2f", pCurSpawn->GetSpawnMacro().m_DelayTime);
					SpawnData[ESPAWNLIST_PATH_INDEX].Format("%d", pCurSpawn->GetPathIndex());
					SpawnData[ESPAWNLIST_SPAWN_GROUP_INDEX].Format("%u", pCurSpawn->GetGroupIdx());
					SpawnData[ESPAWNLIST_PLAY_SCRIPT_INDEX].Format("%u", pCurSpawn->GetPlayScriptIdx());
					SpawnData[ESPAWNLIST_PLAY_SCRIPT_SCENE_INDEX].Format("%u", pCurSpawn->GetPlayScriptSceneIdx());
					SpawnData[ESPAWNLIST_AI_SCRIPT_INDEX].Format("%u", pCurSpawn->GetAIScriptIdx());
					SpawnData[ESPAWNLIST_AI_SCRIPT_SCENE_INDEX].Format("%u", pCurSpawn->GetAIScriptSceneIdx());

					fprintf_s(pFile, "%.2f\t", (fHeight - pCurSpawn->m_T.y));
					for (RwInt32 iDataFields = 0; iDataFields < ESPAWNLIST_NUM; ++iDataFields)
					{
						fprintf_s(pFile, "%s\t", SpawnData[iDataFields].GetBuffer(0));
					}
					fprintf_s(pFile, "\r\n");
				}
			}

			++TheSpawn;
		}

		RwChar cText[64];
		::sprintf_s(cText, 64, "%d / %d export", iCnt++, iNum);
		WndProgress.SetText(cText);
		WndProgress.StepIt();
		WndProgress.PeekAndPump();

		++The;
	}

	WndProgress.SetText("Done.");

	fclose(pFile);

	pFieldMgr->SetAnotherField(TRUE);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(vOldPos);
	CNtlWeControlUi::GetInstance().Update();
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.OnIdle();	
}
