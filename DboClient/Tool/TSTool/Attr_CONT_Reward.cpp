// Attr_CONT_Reward.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_Reward.h"


// CAttr_CONT_Reward 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_Reward, CAttr_Page, 1)

CAttr_CONT_Reward::CAttr_CONT_Reward()
	: CAttr_Page(CAttr_CONT_Reward::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_dwLTime(0xffffffff)
	, m_uiDesc(0xffffffff)
	, m_dwRewardTbl( 0xffffffff )
	, m_dwDefIndex0(0xffffffff)
	, m_nDefValue0(0)
	, m_dwDefIndex1(0xffffffff)
	, m_nDefValue1(0)
	, m_dwDefIndex2(0xffffffff)
	, m_nDefValue2(0)
	, m_dwDefIndex3(0xffffffff)
	, m_nDefValue3(0)
	, m_dwSelIndex0(0xffffffff)
	, m_nSelValue0(0)
	, m_dwSelIndex1(0xffffffff)
	, m_nSelValue1(0)
	, m_dwSelIndex2(0xffffffff)
	, m_nSelValue2(0)
	, m_dwSelIndex3(0xffffffff)
	, m_nSelValue3(0)
{

}

CAttr_CONT_Reward::~CAttr_CONT_Reward()
{
}

CString CAttr_CONT_Reward::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("rwdtype"), m_ctrRwdType.GetItemData( m_ctrRwdType.GetCurSel() ) );
	strData += PakingPageData( _T("ltime"), m_dwLTime );
	strData += PakingPageData( _T("desc"), m_uiDesc );

	strData += PakingPageData( _T("usetbl"), m_ctrUseRewardTbl.GetCheck() == BST_CHECKED ? 1 : 0 );
	strData += PakingPageData( _T("rwdtbl"), m_dwRewardTbl );

	eREWARD_TYPE eType;

	// 기본 보상
	eType = (eREWARD_TYPE)m_ctrDefRwdType0.GetItemData( m_ctrDefRwdType0.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("dtype0"), (int)eType );
		strData += PakingPageData( _T("didx0"), m_dwDefIndex0 );
		strData += PakingPageData( _T("dval0"), m_nDefValue0 );
	}
	eType = (eREWARD_TYPE)m_ctrDefRwdType1.GetItemData( m_ctrDefRwdType1.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("dtype1"), (int)eType );
		strData += PakingPageData( _T("didx1"), m_dwDefIndex1 );
		strData += PakingPageData( _T("dval1"), m_nDefValue1 );
	}
	eType = (eREWARD_TYPE)m_ctrDefRwdType2.GetItemData( m_ctrDefRwdType2.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("dtype2"), (int)eType );
		strData += PakingPageData( _T("didx2"), m_dwDefIndex2 );
		strData += PakingPageData( _T("dval2"), m_nDefValue2 );
	}
	eType = (eREWARD_TYPE)m_ctrDefRwdType3.GetItemData( m_ctrDefRwdType3.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("dtype3"), (int)eType );
		strData += PakingPageData( _T("didx3"), m_dwDefIndex3 );
		strData += PakingPageData( _T("dval3"), m_nDefValue3 );
	}

	// 선택 보상
	eType = (eREWARD_TYPE)m_ctrSelRwdType0.GetItemData( m_ctrSelRwdType0.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("stype0"), (int)eType );
		strData += PakingPageData( _T("sidx0"), m_dwSelIndex0 );
		strData += PakingPageData( _T("sval0"), m_nSelValue0 );
	}
	eType = (eREWARD_TYPE)m_ctrSelRwdType1.GetItemData( m_ctrSelRwdType1.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("stype1"), (int)eType );
		strData += PakingPageData( _T("sidx1"), m_dwSelIndex1 );
		strData += PakingPageData( _T("sval1"), m_nSelValue1 );
	}
	eType = (eREWARD_TYPE)m_ctrSelRwdType2.GetItemData( m_ctrSelRwdType2.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("stype2"), (int)eType );
		strData += PakingPageData( _T("sidx2"), m_dwSelIndex2 );
		strData += PakingPageData( _T("sval2"), m_nSelValue2 );
	}
	eType = (eREWARD_TYPE)m_ctrSelRwdType3.GetItemData( m_ctrSelRwdType3.GetCurSel() );
	if ( eREWARD_TYPE_INVALID != eType )
	{
		strData += PakingPageData( _T("stype3"), (int)eType );
		strData += PakingPageData( _T("sidx3"), m_dwSelIndex3 );
		strData += PakingPageData( _T("sval3"), m_nSelValue3 );
	}

	return strData;
}

void CAttr_CONT_Reward::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	if ( _T("rwdtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrRwdType.GetCount(); ++i )
		{
			if ( m_ctrRwdType.GetItemData( i ) == nValue )
			{
				m_ctrRwdType.SetCurSel( i );
				break;
			}
		}
	}

	if ( _T("ltime") == strKey )
	{
		if ( strValue.GetLength() == 0 )
		{
			m_dwLTime = 0xffffffff;
		}
		else
		{
			m_dwLTime = atoi( strValue.GetBuffer() );
		}
	}

	if ( _T("desc") == strKey )
	{
		m_uiDesc = atoi( strValue.GetBuffer() );
	}

	if ( _T("usetbl") == strKey )
	{
		m_ctrUseRewardTbl.SetCheck( (atoi( strValue.GetBuffer() ) != 0 ? BST_CHECKED : BST_UNCHECKED) );
	}

	if ( _T("rwdtbl") == strKey )
	{
		m_dwRewardTbl = atoi( strValue.GetBuffer() );
	}

	// 기본 보상
	if ( _T("dtype0") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrDefRwdType0.GetCount(); ++i )
		{
			if ( m_ctrDefRwdType0.GetItemData( i ) == nValue )
			{
				m_ctrDefRwdType0.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("didx0") == strKey )
	{
		m_dwDefIndex0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dval0") == strKey )
	{
		m_nDefValue0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dtype1") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrDefRwdType1.GetCount(); ++i )
		{
			if ( m_ctrDefRwdType1.GetItemData( i ) == nValue )
			{
				m_ctrDefRwdType1.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("didx1") == strKey )
	{
		m_dwDefIndex1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dval1") == strKey )
	{
		m_nDefValue1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dtype2") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrDefRwdType2.GetCount(); ++i )
		{
			if ( m_ctrDefRwdType2.GetItemData( i ) == nValue )
			{
				m_ctrDefRwdType2.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("didx2") == strKey )
	{
		m_dwDefIndex2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dval2") == strKey )
	{
		m_nDefValue2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dtype3") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrDefRwdType3.GetCount(); ++i )
		{
			if ( m_ctrDefRwdType3.GetItemData( i ) == nValue )
			{
				m_ctrDefRwdType3.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("didx3") == strKey )
	{
		m_dwDefIndex3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dval3") == strKey )
	{
		m_nDefValue3 = atoi( strValue.GetBuffer() );
	}

	// 선택 보상
	if ( _T("stype0") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrSelRwdType0.GetCount(); ++i )
		{
			if ( m_ctrSelRwdType0.GetItemData( i ) == nValue )
			{
				m_ctrSelRwdType0.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("sidx0") == strKey )
	{
		m_dwSelIndex0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("sval0") == strKey )
	{
		m_nSelValue0 = atoi( strValue.GetBuffer() );
	}
	if ( _T("stype1") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrSelRwdType1.GetCount(); ++i )
		{
			if ( m_ctrSelRwdType1.GetItemData( i ) == nValue )
			{
				m_ctrSelRwdType1.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("sidx1") == strKey )
	{
		m_dwSelIndex1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("sval1") == strKey )
	{
		m_nSelValue1 = atoi( strValue.GetBuffer() );
	}
	if ( _T("stype2") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrSelRwdType2.GetCount(); ++i )
		{
			if ( m_ctrSelRwdType2.GetItemData( i ) == nValue )
			{
				m_ctrSelRwdType2.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("sidx2") == strKey )
	{
		m_dwSelIndex2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("sval2") == strKey )
	{
		m_nSelValue2 = atoi( strValue.GetBuffer() );
	}
	if ( _T("stype3") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < m_ctrSelRwdType3.GetCount(); ++i )
		{
			if ( m_ctrSelRwdType3.GetItemData( i ) == nValue )
			{
				m_ctrSelRwdType3.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("sidx3") == strKey )
	{
		m_dwSelIndex3 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("sval3") == strKey )
	{
		m_nSelValue3 = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_Reward::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_TMQREWARD_COMBO, m_ctrRwdType);

	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_LTIME_EDITOR, m_dwLTime);
	DDV_MinMaxUInt(pDX, m_dwLTime, 0, 0xffffffff);

	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DESC_EDITOR, m_uiDesc);
	DDV_MinMaxUInt(pDX, m_uiDesc, 0, 0xffffffff);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_USE_TABLE_CHECK, m_ctrUseRewardTbl);

	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_RWD_TBL_IDX_EDITOR, m_dwRewardTbl);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_1_TYPE_COMBO, m_ctrDefRwdType0);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_1_INDEX_EDITOR, m_dwDefIndex0);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_1_VARIABLE_EDITOR, m_nDefValue0);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_2_TYPE_COMBO, m_ctrDefRwdType1);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_2_INDEX_EDITOR, m_dwDefIndex1);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_2_VARIABLE_EDITOR, m_nDefValue1);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_3_TYPE_COMBO, m_ctrDefRwdType2);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_3_INDEX_EDITOR, m_dwDefIndex2);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_3_VARIABLE_EDITOR, m_nDefValue2);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_4_TYPE_COMBO, m_ctrDefRwdType3);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_4_INDEX_EDITOR, m_dwDefIndex3);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_DEF_4_VARIABLE_EDITOR, m_nDefValue3);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_1_TYPE_COMBO, m_ctrSelRwdType0);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_1_INDEX_EDITOR, m_dwSelIndex0);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_1_VARIABLE_EDITOR, m_nSelValue0);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_2_TYPE_COMBO, m_ctrSelRwdType1);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_2_INDEX_EDITOR, m_dwSelIndex1);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_2_VARIABLE_EDITOR, m_nSelValue1);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_3_TYPE_COMBO, m_ctrSelRwdType2);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_3_INDEX_EDITOR, m_dwSelIndex2);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_3_VARIABLE_EDITOR, m_nSelValue2);

	DDX_Control(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_4_TYPE_COMBO, m_ctrSelRwdType3);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_REWARD_SEL_4_INDEX_EDITOR, m_dwSelIndex3);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REWARD_SEL_4_VARIABLE_EDITOR, m_nSelValue3);
}

BOOL CAttr_CONT_Reward::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	int nIdx;

	// TMQ 보상
	m_ctrRwdType.SetItemData( m_ctrRwdType.AddString( _T("TMQ") ), eREWARD_CONTAINER_TYPE_TMQ );
	nIdx = m_ctrRwdType.AddString( _T("Quest") ); m_ctrRwdType.SetItemData( nIdx, eREWARD_CONTAINER_TYPE_QUEST );
	m_ctrRwdType.SetCurSel( nIdx );

	// 기본 보상
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrDefRwdType0.SetItemData( m_ctrDefRwdType0.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrDefRwdType0.AddString( _T(" Invalid") );
	m_ctrDefRwdType0.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrDefRwdType0.SetCurSel( nIdx );

	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrDefRwdType1.SetItemData( m_ctrDefRwdType1.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrDefRwdType1.AddString( _T(" Invalid") );
	m_ctrDefRwdType1.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrDefRwdType1.SetCurSel( nIdx );

	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrDefRwdType2.SetItemData( m_ctrDefRwdType2.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrDefRwdType2.AddString( _T(" Invalid") );
	m_ctrDefRwdType2.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrDefRwdType2.SetCurSel( nIdx );

	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrDefRwdType3.SetItemData( m_ctrDefRwdType3.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrDefRwdType3.AddString( _T(" Invalid") );
	m_ctrDefRwdType3.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrDefRwdType3.SetCurSel( nIdx );

	// 선택 보상
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrSelRwdType0.SetItemData( m_ctrSelRwdType0.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrSelRwdType0.AddString( _T(" Invalid") );
	m_ctrSelRwdType0.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrSelRwdType0.SetCurSel( nIdx );

	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrSelRwdType1.SetItemData( m_ctrSelRwdType1.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrSelRwdType1.AddString( _T(" Invalid") );
	m_ctrSelRwdType1.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrSelRwdType1.SetCurSel( nIdx );

	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrSelRwdType2.SetItemData( m_ctrSelRwdType2.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrSelRwdType2.AddString( _T(" Invalid") );
	m_ctrSelRwdType2.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrSelRwdType2.SetCurSel( nIdx );

	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Normal item") ), eREWARD_TYPE_NORMAL_ITEM );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Quest item") ), eREWARD_TYPE_QUEST_ITEM );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Exp") ), eREWARD_TYPE_EXP );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Skill") ), eREWARD_TYPE_SKILL );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Zeny") ), eREWARD_TYPE_ZENY );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Change class") ), eREWARD_TYPE_CHANGE_CLASS );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Buff") ), eREWARD_TYPE_BUFF );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Probability") ), eREWARD_TYPE_PROBABILITY );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Reputation") ), eREWARD_TYPE_REPUTATION );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Change adult") ), eREWARD_TYPE_CHANGE_ADULT );
	m_ctrSelRwdType3.SetItemData( m_ctrSelRwdType3.AddString( _T("Convert class right") ), eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT );
	nIdx = m_ctrSelRwdType3.AddString( _T(" Invalid") );
	m_ctrSelRwdType3.SetItemData( nIdx, eREWARD_TYPE_INVALID );
	m_ctrSelRwdType3.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_CONT_Reward, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_Reward 메시지 처리기입니다.
