// FinderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "FinderDlg.h"
#include "TSDoc.h"
#include "Finder.h"

// Container
#include "Attr_CONT_GAct.h"
#include "Attr_CONT_GCond.h"
#include "Attr_CONT_UserSel.h"
#include "Attr_CONT_Reward.h"
#include "Attr_CONT_Start.h"
#include "Attr_CONT_End.h"
#include "Attr_CONT_Narration.h"
#include "Attr_CONT_Proposal.h"
#include "Attr_CONT_Switch.h"
#include "Attr_CONT_UnifiedNarration.h"

// Event
#include "Attr_EVT_ColObject.h"
#include "Attr_EVT_ClickObject.h"
#include "Attr_EVT_ClickNPC.h"
#include "Attr_EVT_ItemEquip.h"
#include "Attr_EVT_ItemGet.h"
#include "Attr_EVT_ItemUse.h"
#include "Attr_EVT_ScoutUse.h"
#include "Attr_EVT_RcvSvrEvt.h"
#include "Attr_EVT_SkillUse.h"
#include "Attr_EVT_ColRgn.h"
#include "Attr_EVT_RB.h"
#include "Attr_EVT_ClickMob.h"
#include "Attr_EVT_BindStone.h"
#include "Attr_EVT_SearchQuest.h"

// Condition
#include "Attr_COND_ClrQst.h"
#include "Attr_COND_Item.h"
#include "Attr_COND_Lvl.h"
#include "Attr_COND_PCCls.h"
#include "Attr_COND_PCRace.h"
#include "Attr_COND_QItem.h"
#include "Attr_COND_SSM.h"
#include "Attr_COND_SToCEvt.h"
#include "Attr_COND_WItem.h"
#include "Attr_COND_ObjItem.h"
#include "Attr_COND_InNPC.h"
#include "Attr_COND_ObjState.h"
#include "Attr_COND_InWorld.h"
#include "Attr_COND_OperateObject.h"
#include "Attr_COND_HasCoupon.h"
#include "Attr_COND_NPCDead.h"
#include "Attr_COND_DistNPC.h"
#include "Attr_COND_SToCDelivery.h"
#include "Attr_COND_AttachObj.h"
#include "Attr_COND_ProgQuest.h"
#include "Attr_COND_Reputation.h"
#include "Attr_COND_BudokaiState.h"
#include "Attr_COND_ProgCustomEvt.h"
#include "Attr_COND_AvatarDead.h"
#include "Attr_COND_OutRgn.h"

// Action
#include "Attr_ACT_Dir.h"
#include "Attr_ACT_ETimerS.h"
#include "Attr_ACT_ETimerE.h"
#include "Attr_ACT_InSSM.h"
#include "Attr_ACT_Item.h"
#include "Attr_ACT_NPCConv.h"
#include "Attr_ACT_OPCam.h"
#include "Attr_ACT_QItem.h"
#include "Attr_ACT_RegQInfo.h"
#include "Attr_ACT_SToCEvt.h"
#include "Attr_ACT_TWaitTS.h"
#include "Attr_ACT_TSState.h"
#include "Attr_ACT_Portal.h"
#include "Attr_ACT_ObjState.h"
#include "Attr_ACT_ConcCheck.h"
#include "Attr_ACT_ObjConv.h"
#include "Attr_ACT_Hint.h"
#include "Attr_ACT_TMQStage.h"
#include "Attr_ACT_SendSvrEvt.h"
#include "Attr_ACT_OpenWindow.h"
#include "Attr_ACT_TMQTimeBonus.h"
#include "Attr_ACT_BroadCast.h"
#include "Attr_ACT_DirIndicator.h"
#include "Attr_ACT_OperateObject.h"
#include "Attr_ACT_Drop.h"
#include "Attr_ACT_RmvCoupon.h"
#include "Attr_ACT_Escort.h"
#include "Attr_ACT_TMQInfoShow.h"
#include "Attr_ACT_WorldPlayScript.h"
#include "Attr_ACT_SWProbSF.h"
#include "Attr_ACT_PCConv.h"
#include "Attr_ACT_CustomEvt.h"
#include "Attr_ACT_TelMinorMatch.h"
#include "Attr_ACT_TelMudosa.h"
#include "Attr_ACT_PIDgn.h"
#include "Attr_ACT_TLQ.h"
#include "Attr_ACT_DoSkill.h"
#include "Attr_ACT_TObjFriendly.h"
#include "Attr_ACT_BroadMsg.h"
#include "Attr_ACT_MiniNarration.h"
#include "Attr_ACT_RegCTiming.h"
#include "Attr_ACT_ExcCGroup.h"
#include "Attr_ACT_SkipCont.h"
#include "Attr_ACT_ObjWPS.h"
#include "Attr_ACT_Dojo.h"


// CFinderDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFinderDlg, CDialog)

CFinderDlg::CFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinderDlg::IDD, pParent)
{
	// Container
	m_defKeyValContList[_T("CT_GAct")]				= RUNTIME_CLASS( CAttr_CONT_GAct );
	m_defKeyValContList[_T("CT_GCond")]				= RUNTIME_CLASS( CAttr_CONT_GCond );
	m_defKeyValContList[_T("CT_UsrSel")]			= RUNTIME_CLASS( CAttr_CONT_UserSel );
	m_defKeyValContList[_T("CT_Reward")]			= RUNTIME_CLASS( CAttr_CONT_Reward );
	m_defKeyValContList[_T("CT_Start")]				= RUNTIME_CLASS( CAttr_CONT_Start );
	m_defKeyValContList[_T("CT_End")]				= RUNTIME_CLASS( CAttr_CONT_End );
	m_defKeyValContList[_T("CT_Narration")]			= RUNTIME_CLASS( CAttr_CONT_Narration );
	m_defKeyValContList[_T("CT_Proposal")]			= RUNTIME_CLASS( CAttr_CONT_Proposal );
	m_defKeyValContList[_T("CT_Switch")]			= RUNTIME_CLASS( CAttr_CONT_Switch );
	m_defKeyValContList[_T("CT_UnifiedNarration")]	= RUNTIME_CLASS( CAttr_CONT_UnifiedNarration );

	// Event
	m_defKeyValEventList[_T("E_ColObject")]			= RUNTIME_CLASS( CAttr_EVT_ColObject );
	m_defKeyValEventList[_T("E_ClickObject")]		= RUNTIME_CLASS( CAttr_EVT_ClickObject );
	m_defKeyValEventList[_T("E_ClickNPC")]			= RUNTIME_CLASS( CAttr_EVT_ClickNPC );
	m_defKeyValEventList[_T("E_ItemUse")]			= RUNTIME_CLASS( CAttr_EVT_ItemUse );
	m_defKeyValEventList[_T("E_ItemGet")]			= RUNTIME_CLASS( CAttr_EVT_ItemGet );
	m_defKeyValEventList[_T("E_ItemEquip")]			= RUNTIME_CLASS( CAttr_EVT_ItemEquip );
	m_defKeyValEventList[_T("E_RcvSvrEvt")]			= RUNTIME_CLASS( CAttr_EVT_RcvSvrEvt );
	m_defKeyValEventList[_T("E_ScoutUse")]			= RUNTIME_CLASS( CAttr_EVT_ScoutUse );
	m_defKeyValEventList[_T("E_SkillUse")]			= RUNTIME_CLASS( CAttr_EVT_SkillUse );
	m_defKeyValEventList[_T("E_ColRgn")]			= RUNTIME_CLASS( CAttr_EVT_ColRgn );
	m_defKeyValEventList[_T("E_RB")]				= RUNTIME_CLASS( CAttr_EVT_RB );
	m_defKeyValEventList[_T("E_ClickMob")]			= RUNTIME_CLASS( CAttr_EVT_ClickMob );
	m_defKeyValEventList[_T("E_BindStone")]			= RUNTIME_CLASS( CAttr_EVT_BindStone );
	m_defKeyValEventList[_T("E_SearchQuest")]		= RUNTIME_CLASS( CAttr_EVT_SearchQuest );

	// Condition
	m_defKeyValCondList[_T("C_ClrQst")]				= RUNTIME_CLASS( CAttr_COND_ClrQst );
	m_defKeyValCondList[_T("C_Item")]				= RUNTIME_CLASS( CAttr_COND_Item );
	m_defKeyValCondList[_T("C_Lvl")]				= RUNTIME_CLASS( CAttr_COND_Lvl );
	m_defKeyValCondList[_T("C_PCCls")]				= RUNTIME_CLASS( CAttr_COND_PCCls );
	m_defKeyValCondList[_T("C_PCRace")]				= RUNTIME_CLASS( CAttr_COND_PCRace );
	m_defKeyValCondList[_T("C_QItem")]				= RUNTIME_CLASS( CAttr_COND_QItem );
	m_defKeyValCondList[_T("C_SSM")]				= RUNTIME_CLASS( CAttr_COND_SSM );
	m_defKeyValCondList[_T("C_SToCEvt")]			= RUNTIME_CLASS( CAttr_COND_SToCEvt );
	m_defKeyValCondList[_T("C_WItem")]				= RUNTIME_CLASS( CAttr_COND_WItem );
	m_defKeyValCondList[_T("C_ObjItem")]			= RUNTIME_CLASS( CAttr_COND_ObjItem );
	m_defKeyValCondList[_T("C_InNPC")]				= RUNTIME_CLASS( CAttr_COND_InNPC );
	m_defKeyValCondList[_T("C_CheckObjState")]		= RUNTIME_CLASS( CAttr_COND_ObjState );
	m_defKeyValCondList[_T("C_CheckInWorld")]		= RUNTIME_CLASS( CAttr_COND_InWorld );
	m_defKeyValCondList[_T("C_CheckOPObject")]		= RUNTIME_CLASS( CAttr_COND_OperateObject );
	m_defKeyValCondList[_T("C_CheckHasCoupon")]		= RUNTIME_CLASS( CAttr_COND_HasCoupon );
	m_defKeyValCondList[_T("C_CheckNPCDead")]		= RUNTIME_CLASS( CAttr_COND_NPCDead );
	m_defKeyValCondList[_T("C_CheckDistNPC")]		= RUNTIME_CLASS( CAttr_COND_DistNPC );
	m_defKeyValCondList[_T("C_CheckSToCDelivery")]	= RUNTIME_CLASS( CAttr_COND_SToCDelivery );
	m_defKeyValCondList[_T("C_CheckAttachObj")]		= RUNTIME_CLASS( CAttr_COND_AttachObj );
	m_defKeyValCondList[_T("C_CheckProgQuest")]		= RUNTIME_CLASS( CAttr_COND_ProgQuest );
	m_defKeyValCondList[_T("C_CheckReputation")]	= RUNTIME_CLASS( CAttr_COND_Reputation );
	m_defKeyValCondList[_T("C_CheckBudokaiState")]	= RUNTIME_CLASS( CAttr_COND_BudokaiState );
	m_defKeyValCondList[_T("C_CheckProgCustomEvt")]	= RUNTIME_CLASS( CAttr_COND_ProgCustomEvt );
	m_defKeyValCondList[_T("C_CheckAvatarDead")]	= RUNTIME_CLASS( CAttr_COND_AvatarDead );
	m_defKeyValCondList[_T("C_CheckOutRgn")]		= RUNTIME_CLASS( CAttr_COND_OutRgn );

	// Action
	m_defKeyValActList[_T("A_Dir")]					= RUNTIME_CLASS( CAttr_ACT_Dir );
	m_defKeyValActList[_T("A_ETimerS")]				= RUNTIME_CLASS( CAttr_ACT_ETimerS );
	m_defKeyValActList[_T("A_ETimerE")]				= RUNTIME_CLASS( CAttr_ACT_ETimerE );
	m_defKeyValActList[_T("A_InSSM")]				= RUNTIME_CLASS( CAttr_ACT_InSSM );
	m_defKeyValActList[_T("A_Item")]				= RUNTIME_CLASS( CAttr_ACT_Item );
	m_defKeyValActList[_T("A_NPCConv")]				= RUNTIME_CLASS( CAttr_ACT_NPCConv );
	m_defKeyValActList[_T("A_OPCam")]				= RUNTIME_CLASS( CAttr_ACT_OPCam );
	m_defKeyValActList[_T("A_QItem")]				= RUNTIME_CLASS( CAttr_ACT_QItem );
	m_defKeyValActList[_T("A_RegQInfo")]			= RUNTIME_CLASS( CAttr_ACT_RegQInfo );
	m_defKeyValActList[_T("A_SToCEvt")]				= RUNTIME_CLASS( CAttr_ACT_SToCEvt );
	m_defKeyValActList[_T("A_TWaitTS")]				= RUNTIME_CLASS( CAttr_ACT_TWaitTS );
	m_defKeyValActList[_T("A_TSState")]				= RUNTIME_CLASS( CAttr_ACT_TSState );
	m_defKeyValActList[_T("A_Potal")]				= RUNTIME_CLASS( CAttr_ACT_Portal );
	m_defKeyValActList[_T("A_ObjState")]			= RUNTIME_CLASS( CAttr_ACT_ObjState );
	m_defKeyValActList[_T("A_ConcCheck")]			= RUNTIME_CLASS( CAttr_ACT_ConcCheck );
	m_defKeyValActList[_T("A_ObjConv")]				= RUNTIME_CLASS( CAttr_ACT_ObjConv );
	m_defKeyValActList[_T("A_Hint")]				= RUNTIME_CLASS( CAttr_ACT_Hint );
	m_defKeyValActList[_T("A_SendSvrEvt")]			= RUNTIME_CLASS( CAttr_ACT_SendSvrEvt );
	m_defKeyValActList[_T("A_TMQStage")]			= RUNTIME_CLASS( CAttr_ACT_TMQStage );
	m_defKeyValActList[_T("A_OpenWindow")]			= RUNTIME_CLASS( CAttr_ACT_OpenWindow );
	m_defKeyValActList[_T("A_TMQTimeBonus")]		= RUNTIME_CLASS( CAttr_ACT_TMQTimeBonus );
	m_defKeyValActList[_T("A_BroadCast")]			= RUNTIME_CLASS( CAttr_ACT_BroadCast );
	m_defKeyValActList[_T("A_DirIndicator")]		= RUNTIME_CLASS( CAttr_ACT_DirIndicator );
	m_defKeyValActList[_T("A_OperateObject")]		= RUNTIME_CLASS( CAttr_ACT_OperateObject );
	m_defKeyValActList[_T("A_Drop")]				= RUNTIME_CLASS( CAttr_ACT_Drop );
	m_defKeyValActList[_T("A_RmvCoupon")]			= RUNTIME_CLASS( CAttr_ACT_RmvCoupon );
	m_defKeyValActList[_T("A_Escort")]				= RUNTIME_CLASS( CAttr_ACT_Escort );
	m_defKeyValActList[_T("A_TMQInfoShow")]			= RUNTIME_CLASS( CAttr_ACT_TMQInfoShow );
	m_defKeyValActList[_T("A_WorldPlayScript")]		= RUNTIME_CLASS( CAttr_ACT_WorldPlayScript );
	m_defKeyValActList[_T("A_SWProbSF")]			= RUNTIME_CLASS( CAttr_ACT_SWProbSF );
	m_defKeyValActList[_T("A_PCConv")]				= RUNTIME_CLASS( CAttr_ACT_PCConv );
	m_defKeyValActList[_T("A_CustomEvt")]			= RUNTIME_CLASS( CAttr_ACT_CustomEvt );
	m_defKeyValActList[_T("A_Minormatch")]			= RUNTIME_CLASS( CAttr_ACT_TelMinorMatch );
	m_defKeyValActList[_T("A_Mudosa")]				= RUNTIME_CLASS( CAttr_ACT_TelMudosa );
	m_defKeyValActList[_T("A_PIDgn")]				= RUNTIME_CLASS( CAttr_ACT_PIDgn );
	m_defKeyValActList[_T("A_TLQ")]					= RUNTIME_CLASS( CAttr_ACT_TLQ );
	m_defKeyValActList[_T("A_DoSkill")]				= RUNTIME_CLASS( CAttr_ACT_DoSkill );
	m_defKeyValActList[_T("A_TObjFriendly")]		= RUNTIME_CLASS( CAttr_ACT_TObjFriendly );
	m_defKeyValActList[_T("A_BroadMsg")]			= RUNTIME_CLASS( CAttr_ACT_BroadMsg );
	m_defKeyValActList[_T("A_MiniNarration")]		= RUNTIME_CLASS( CAttr_ACT_MiniNarration );
	m_defKeyValActList[_T("A_RegCTiming")]			= RUNTIME_CLASS( CAttr_ACT_RegCTiming );
	m_defKeyValActList[_T("A_ExcCGroup")]			= RUNTIME_CLASS( CAttr_ACT_ExcCGroup );
	m_defKeyValActList[_T("A_SkipCont")]			= RUNTIME_CLASS( CAttr_ACT_SkipCont );
	m_defKeyValActList[_T("A_ObjWPS")]				= RUNTIME_CLASS( CAttr_ACT_ObjWPS );
	m_defKeyValActList[_T("A_Dojo")]				= RUNTIME_CLASS( CAttr_ACT_Dojo );
}

CFinderDlg::~CFinderDlg()
{
}

void CFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FINDER_CONT_COMBO, m_ctrCont);
	DDX_Control(pDX, IDC_FINDER_EVENT_COMBO, m_ctrEvent);
	DDX_Control(pDX, IDC_FINDER_COND_COMBO, m_ctrCond);
	DDX_Control(pDX, IDC_FINDER_ACTION_COMBO, m_ctrAct);
	DDX_Control(pDX, IDC_FINDER_OUTPUT_LIST, m_ctrResultOutput);
}

BEGIN_MESSAGE_MAP(CFinderDlg, CDialog)
	ON_BN_CLICKED(IDC_FINDER_FIND_BTN, &CFinderDlg::OnBnClickedFinderFindBtn)
	ON_BN_CLICKED(IDC_FINDER_FIND_RESULT_SAVE_BTN, &CFinderDlg::OnBnClickedFinderFindResultSaveBtn)
END_MESSAGE_MAP()


// CFinderDlg 메시지 처리기입니다.

void CFinderDlg::OnBnClickedFinderFindBtn()
{
	if ( CTSDoc::GetInstance()->IsModified() ) CTSDoc::GetInstance()->DoFileSave();

	m_ctrResultOutput.ResetContent();

	CTSProject* pTSProj = CTSDoc::GetInstance()->GetProjectObject();

	CArray<CString, const CString&>& arTrigList = pTSProj->GetTriggerList();

	for ( int i = 0; i < arTrigList.GetCount(); ++i )
	{
		CString strPath = pTSProj->GetProjectPath();
		strPath += _T("\\");
		strPath += arTrigList[i];

		CTSDoc::GetInstance()->SetPathName( strPath );
		CTSDoc::GetInstance()->OnOpenDocument( strPath );

		CFinder clFinder( &pTSProj->GetTrigger() );

		int nCurSel;

		{
			nCurSel = m_ctrCont.GetCurSel();
			if ( CB_ERR != nCurSel )
			{
				CString* pKeyVal = (CString*)m_ctrCont.GetItemData( nCurSel );
				if ( pKeyVal )
				{
					clFinder.AddFindEntity( *pKeyVal );
				}
			}
		}

		{
			nCurSel = m_ctrEvent.GetCurSel();
			if ( CB_ERR != nCurSel )
			{
				CString* pKeyVal = (CString*)m_ctrEvent.GetItemData( nCurSel );
				if ( pKeyVal )
				{
					clFinder.AddFindEntity( *pKeyVal );
				}
			}
		}

		{
			nCurSel = m_ctrCond.GetCurSel();
			if ( CB_ERR != nCurSel )
			{
				CString* pKeyVal = (CString*)m_ctrCond.GetItemData( nCurSel );
				if ( pKeyVal )
				{
					clFinder.AddFindEntity( *pKeyVal );
				}
			}
		}

		{
			nCurSel = m_ctrAct.GetCurSel();
			if ( CB_ERR != nCurSel )
			{
				CString* pKeyVal = (CString*)m_ctrAct.GetItemData( nCurSel );
				if ( pKeyVal )
				{
					clFinder.AddFindEntity( *pKeyVal );
				}
			}
		}

		clFinder.Run();

		CFinder::vecdef_FindResultList* pList = clFinder.GetFindResult();

		int nListCnt = (int)pList->size();
		for ( int i = 0; i < nListCnt; ++i )
		{
			CFinder::sFIND_ENTITY& sFindEntity = (*pList)[i];

			CString strFormat;
			strFormat.Format( _T("%d \t %d \t %d \t %d \t %s \t %s"), sFindEntity.tId, sFindEntity.tgId, sFindEntity.tcId, sFindEntity.taId, sFindEntity.strType, sFindEntity.strAttr );

			m_ctrResultOutput.AddString( strFormat );
		}
	}

}

void CFinderDlg::OnBnClickedFinderFindResultSaveBtn()
{
	CFileDialog clFile( TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.*", NULL, 0 );
	if ( IDOK == clFile.DoModal() )
	{
		CString strPath = clFile.GetPathName();

		FILE* pFile;
		fopen_s( &pFile, strPath.GetBuffer(), "w" );

		if ( pFile )
		{
			CString strString;

			int nCnt = m_ctrResultOutput.GetCount();
			for ( int i = 0; i < nCnt; ++i )
			{
				m_ctrResultOutput.GetText( i, strString );
				fprintf( pFile, "%s\n", strString.GetBuffer() );
			}

			fclose( pFile );
		}
	}
}

BOOL CFinderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strCaption;

	// Container
	{
		mapdef_KeyValList::iterator it = m_defKeyValContList.begin();
		for ( ; it != m_defKeyValContList.end(); ++it )
		{
			CAttr_Page* pPage = (CAttr_Page*)it->second->CreateObject();

			pPage->GetCaption( strCaption );

			m_ctrCont.SetItemData( m_ctrCont.AddString( strCaption ), (DWORD_PTR)&it->first );

			delete pPage;
		}
	}

	// Event
	{
		mapdef_KeyValList::iterator it = m_defKeyValEventList.begin();
		for ( ; it != m_defKeyValEventList.end(); ++it )
		{
			CAttr_Page* pPage = (CAttr_Page*)it->second->CreateObject();

			pPage->GetCaption( strCaption );

			m_ctrEvent.SetItemData( m_ctrEvent.AddString( strCaption ), (DWORD_PTR)&it->first );

			delete pPage;
		}
	}

	// Condition
	{
		mapdef_KeyValList::iterator it = m_defKeyValCondList.begin();
		for ( ; it != m_defKeyValCondList.end(); ++it )
		{
			CAttr_Page* pPage = (CAttr_Page*)it->second->CreateObject();

			pPage->GetCaption( strCaption );

			m_ctrCond.SetItemData( m_ctrCond.AddString( strCaption ), (DWORD_PTR)&it->first );

			delete pPage;
		}
	}

	// Action
	{
		mapdef_KeyValList::iterator it = m_defKeyValActList.begin();
		for ( ; it != m_defKeyValActList.end(); ++it )
		{
			CAttr_Page* pPage = (CAttr_Page*)it->second->CreateObject();

			pPage->GetCaption( strCaption );

			m_ctrAct.SetItemData( m_ctrAct.AddString( strCaption ), (DWORD_PTR)&it->first );

			delete pPage;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
