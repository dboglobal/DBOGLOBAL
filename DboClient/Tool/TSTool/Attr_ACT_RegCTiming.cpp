// Attr_ACT_RegCTiming.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_RegCTiming.h"


// CAttr_ACT_RegCTiming 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_RegCTiming, CAttr_Page, 1)

CAttr_ACT_RegCTiming::CAttr_ACT_RegCTiming()
	: CAttr_Page(CAttr_ACT_RegCTiming::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwMobGTblIdx( 0xffffffff )
	, m_dwNpcTblIdx( 0xffffffff )
	, m_dwOCWorldTblIdx( 0xffffffff )
	, m_dwObjTblIdx( 0xffffffff )
	, m_dwERWorldTblIdx( 0xffffffff )
	, m_fMinX( FLT_MAX )
	, m_fMinZ( FLT_MAX )
	, m_fMaxX( FLT_MAX )
	, m_fMaxZ( FLT_MAX )
	, m_dwQItemTblIdx( 0xffffffff )
	, m_dwNarTblIdx( 0xffffffff )
{
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_TIMING_GROUP );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_MIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_MIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_NIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_NIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_WTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_WTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_OBJTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_OBJTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_WTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_WTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINZ_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINZ_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXZ_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXZ_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_QITBLIDX_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_QITBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_QTITLE_CLICK_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ACCEPT_QUEST_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_SUCCESS_QUEST_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_POST_REWARD_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_PRE_REWARD_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_EXC_GROUP );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_BTN );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_STATIC );
	m_defMapList[eREG_TYPE_INVALID].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_EDITOR );

	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_TIMING_GROUP );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_MIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_MIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_NIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_NIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_WTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_WTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_OBJTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_OBJTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_WTBLIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_WTBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINZ_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINZ_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXZ_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXZ_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_QITBLIDX_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_QITBLIDX_EDITOR );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_QTITLE_CLICK_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ACCEPT_QUEST_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_SUCCESS_QUEST_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_PRE_REWARD_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_POST_REWARD_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_EXC_GROUP );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_BTN );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_STATIC );
	m_defMapList[eREG_TYPE_REG].push_back( IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_EDITOR );
}

CAttr_ACT_RegCTiming::~CAttr_ACT_RegCTiming()
{
}

void CAttr_ACT_RegCTiming::InitData( void )
{
	m_nCurSelIndex = -1;
	m_dwMobGTblIdx = 0xffffffff;
	m_dwNpcTblIdx = 0xffffffff;
	m_dwOCWorldTblIdx = 0xffffffff;
	m_dwObjTblIdx = 0xffffffff;
	m_dwERWorldTblIdx = 0xffffffff;
	m_fMinX = FLT_MAX;
	m_fMinZ = FLT_MAX;
	m_fMaxX = FLT_MAX;
	m_fMaxZ = FLT_MAX;
	m_dwQItemTblIdx = 0xffffffff;
	m_dwNarTblIdx = 0xffffffff;

	UpdateData( FALSE );
}

CString CAttr_ACT_RegCTiming::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	strData += PakingPageData( _T("rtype"), (int)m_ctrRegType.GetItemData( m_ctrRegType.GetCurSel() ) );

	if ( (int)m_ctrRegType.GetItemData( m_ctrRegType.GetCurSel() ) == eREG_TYPE_REG )
	{
		DWORD dwTimingFlags = 0;

		dwTimingFlags |= ( m_ctrMobClick.GetCheck() == BST_CHECKED ? eREG_C_TIMING_MOB_CLICK_FLAG : 0 );
		dwTimingFlags |= ( m_ctrNpcClick.GetCheck() == BST_CHECKED ? eREG_C_TIMING_NPC_CLICK_FLAG : 0 );
		dwTimingFlags |= ( m_ctrObjClick.GetCheck() == BST_CHECKED ? eREG_C_TIMING_OBJ_CLICK_FLAG : 0 );
		dwTimingFlags |= ( m_ctrEnterRgn.GetCheck() == BST_CHECKED ? eREG_C_TIMING_ENTER_RGN_FLAG : 0 );
		dwTimingFlags |= ( m_ctrGetQItem.GetCheck() == BST_CHECKED ? eREG_C_TIMING_GET_QITEM_FLAG : 0 );
		dwTimingFlags |= ( m_ctrQTitleClick.GetCheck() == BST_CHECKED ? eREG_C_TIMING_QLISTWND_QTITLE_CLICK_FLAG : 0 );
		dwTimingFlags |= ( m_ctrAcceptQuest.GetCheck() == BST_CHECKED ? eREG_C_TIMING_ACCEPT_QUEST_FLAG : 0 );
		dwTimingFlags |= ( m_ctrSuccessQuest.GetCheck() == BST_CHECKED ? eREG_C_TIMING_SUCCESS_QUEST_FLAG : 0 );
		dwTimingFlags |= ( m_ctrPreReward.GetCheck() == BST_CHECKED ? eREG_C_TIMING_PRE_REWARD_FLAG : 0 );
		dwTimingFlags |= ( m_ctrPostReward.GetCheck() == BST_CHECKED ? eREG_C_TIMING_POST_REWARD_FLAG : 0 );

		strData += PakingPageData( _T("flags"), dwTimingFlags );

		if ( dwTimingFlags & eREG_C_TIMING_MOB_CLICK_FLAG )
		{
			strData += PakingPageData( _T("0_midx"), m_dwMobGTblIdx );
		}
		if ( dwTimingFlags & eREG_C_TIMING_NPC_CLICK_FLAG )
		{
			strData += PakingPageData( _T("1_nidx"), m_dwNpcTblIdx );
		}
		if ( dwTimingFlags & eREG_C_TIMING_OBJ_CLICK_FLAG )
		{
			strData += PakingPageData( _T("2_widx"), m_dwOCWorldTblIdx );
			strData += PakingPageData( _T("2_oidx"), m_dwObjTblIdx );
		}
		if ( dwTimingFlags & eREG_C_TIMING_ENTER_RGN_FLAG )
		{
			strData += PakingPageData( _T("3_widx"), m_dwERWorldTblIdx );
			strData += PakingPageData( _T("3_minx"), m_fMinX );
			strData += PakingPageData( _T("3_minz"), m_fMinZ );
			strData += PakingPageData( _T("3_maxx"), m_fMaxX );
			strData += PakingPageData( _T("3_maxz"), m_fMaxZ );
		}
		if ( dwTimingFlags & eREG_C_TIMING_GET_QITEM_FLAG )
		{
			strData += PakingPageData( _T("4_qidx"), m_dwQItemTblIdx );
		}

		if ( m_ctrMiniNarration.GetCheck() == BST_CHECKED )
		{
			strData += PakingPageData( _T("etype"), eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION );
			strData += PakingPageData( _T("ntidx"), m_dwNarTblIdx );
		}
	}

	return strData;
}

void CAttr_ACT_RegCTiming::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrRegType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrRegType.GetItemData( i ) == nValue )
			{
				m_ctrRegType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("flags") == strKey )
	{
		DWORD dwTimingFlags = atoi( strValue.GetBuffer() );

		m_ctrMobClick.SetCheck( (dwTimingFlags & eREG_C_TIMING_MOB_CLICK_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrNpcClick.SetCheck( (dwTimingFlags & eREG_C_TIMING_NPC_CLICK_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrObjClick.SetCheck( (dwTimingFlags & eREG_C_TIMING_OBJ_CLICK_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrEnterRgn.SetCheck( (dwTimingFlags & eREG_C_TIMING_ENTER_RGN_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrGetQItem.SetCheck( (dwTimingFlags & eREG_C_TIMING_GET_QITEM_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrQTitleClick.SetCheck( (dwTimingFlags & eREG_C_TIMING_QLISTWND_QTITLE_CLICK_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrAcceptQuest.SetCheck( (dwTimingFlags & eREG_C_TIMING_ACCEPT_QUEST_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrSuccessQuest.SetCheck( (dwTimingFlags & eREG_C_TIMING_SUCCESS_QUEST_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrPreReward.SetCheck( (dwTimingFlags & eREG_C_TIMING_PRE_REWARD_FLAG) ? BST_CHECKED : BST_UNCHECKED );
		m_ctrPostReward.SetCheck( (dwTimingFlags & eREG_C_TIMING_POST_REWARD_FLAG) ? BST_CHECKED : BST_UNCHECKED );
	}
	else if ( _T("0_midx") == strKey )
	{
		m_dwMobGTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("1_nidx") == strKey )
	{
		m_dwNpcTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("2_widx") == strKey )
	{
		m_dwOCWorldTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("2_oidx") == strKey )
	{
		m_dwObjTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("3_widx") == strKey )
	{
		m_dwERWorldTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("3_minx") == strKey )
	{
		m_fMinX = atof( strValue.GetBuffer() );
	}
	else if ( _T("3_minz") == strKey )
	{
		m_fMinZ = atof( strValue.GetBuffer() );
	}
	else if ( _T("3_maxx") == strKey )
	{
		m_fMaxX = atof( strValue.GetBuffer() );
	}
	else if ( _T("3_maxz") == strKey )
	{
		m_fMaxZ = atof( strValue.GetBuffer() );
	}
	else if ( _T("4_qidx") == strKey )
	{
		m_dwQItemTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("etype") == strKey )
	{
		if ( eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION == atoi( strValue.GetBuffer() ) )
		{
			m_ctrMiniNarration.SetCheck( BST_CHECKED );
		}
		else
		{
			m_ctrMiniNarration.SetCheck( BST_UNCHECKED );
		}
	}
	else if ( _T("ntidx") == strKey )
	{
		m_dwNarTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_RegCTiming::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REG_C_TIMING_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TYPE_COMBO, m_ctrRegType);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_BTN, m_ctrMobClick);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_MOB_CLICK_MIDX_EDITOR, m_dwMobGTblIdx);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_BTN, m_ctrNpcClick);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_NPC_CLICK_NIDX_EDITOR, m_dwNpcTblIdx);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_BTN, m_ctrObjClick);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_WTBLIDX_EDITOR, m_dwOCWorldTblIdx);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_OBJ_CLICK_OBJTBLIDX_EDITOR, m_dwObjTblIdx);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_BTN, m_ctrEnterRgn);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_WTBLIDX_EDITOR, m_dwERWorldTblIdx);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINX_EDITOR, m_fMinX);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MINZ_EDITOR, m_fMinZ);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXX_EDITOR, m_fMaxX);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ENTER_RGN_MAXZ_EDITOR, m_fMaxZ);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_BTN, m_ctrGetQItem);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_GET_QITEM_QITBLIDX_EDITOR, m_dwQItemTblIdx);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_QTITLE_CLICK_BTN, m_ctrQTitleClick);
	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_ACCEPT_QUEST_BTN, m_ctrAcceptQuest);
	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_SUCCESS_QUEST_BTN, m_ctrSuccessQuest);
	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_POST_REWARD_BTN, m_ctrPreReward);
	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_TYPE_PRE_REWARD_BTN, m_ctrPostReward);

	DDX_Control(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_BTN, m_ctrMiniNarration);
	DDX_Text(pDX, IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_EDITOR, m_dwNarTblIdx);
}

BOOL CAttr_ACT_RegCTiming::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrRegType.SetItemData( m_ctrRegType.AddString( _T("해제") ), eREG_TYPE_UNREG );
	int nIdx = m_ctrRegType.AddString( _T("등록") );
	m_ctrRegType.SetItemData( nIdx, eREG_TYPE_REG );
	m_ctrRegType.SetCurSel( nIdx );

	m_ctrMobClick.SetCheck( BST_UNCHECKED );
	m_ctrNpcClick.SetCheck( BST_UNCHECKED );
	m_ctrObjClick.SetCheck( BST_UNCHECKED );
	m_ctrEnterRgn.SetCheck( BST_UNCHECKED );
	m_ctrGetQItem.SetCheck( BST_UNCHECKED );
	m_ctrQTitleClick.SetCheck( BST_UNCHECKED );
	m_ctrAcceptQuest.SetCheck( BST_UNCHECKED );
	m_ctrSuccessQuest.SetCheck( BST_UNCHECKED );
	m_ctrPreReward.SetCheck( BST_UNCHECKED );
	m_ctrPostReward.SetCheck( BST_UNCHECKED );

	m_ctrMiniNarration.SetCheck( BST_CHECKED );

	vecdef_ControlList::iterator it = m_defMapList[eREG_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eREG_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrRegCTimingTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_RegCTiming, CAttr_Page)
	ON_CBN_SELCHANGE(IDD_TS_ACT_REG_C_TIMING_TYPE_COMBO, &CAttr_ACT_RegCTiming::OnCbnSelchangeTsActAttrRegCTimingTypeCombo)
	ON_BN_CLICKED(IDD_TS_ACT_REG_C_TIMING_TIMING_EXC_MINNARRATION_BTN, &CAttr_ACT_RegCTiming::OnBnClickedTsActRegCTimingTimingExcMinnarrationBtn)
END_MESSAGE_MAP()


// CAttr_ACT_RegCTiming 메시지 처리기입니다.

void CAttr_ACT_RegCTiming::OnCbnSelchangeTsActAttrRegCTimingTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrRegType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitData();
	}

	m_nCurSelIndex = m_ctrRegType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrRegType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}
		}
		else
		{
			m_nCurSelIndex = -1;
		}
	}
	else
	{
		m_nCurSelIndex = -1;
	}
}

void CAttr_ACT_RegCTiming::OnBnClickedTsActRegCTimingTimingExcMinnarrationBtn()
{
	m_ctrMiniNarration.SetCheck( BST_CHECKED );
}
