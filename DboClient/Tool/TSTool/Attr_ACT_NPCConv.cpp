// Attr_ACT_NPCConv.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_NPCConv.h"


// CAttr_ACT_NPCConv 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_NPCConv, CAttr_Page, 1)

CAttr_ACT_NPCConv::CAttr_ACT_NPCConv()
	: CAttr_Page(CAttr_ACT_NPCConv::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwNPCIdx(0xffffffff)
	, m_uiConv(0xffffffff)
{

}

CAttr_ACT_NPCConv::~CAttr_ACT_NPCConv()
{
}

CString CAttr_ACT_NPCConv::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("ctype"), (int)m_ctrConvType.GetItemData( m_ctrConvType.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_dwNPCIdx );
	strData += PakingPageData( _T("conv"), m_uiConv );

	return strData;
}

void CAttr_ACT_NPCConv::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ctype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrConvType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrConvType.GetItemData( i ) == nValue )
			{
				m_ctrConvType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_dwNPCIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("conv") == strKey )
	{
		m_uiConv = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_NPCConv::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_NPCCONV_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_NPCCONV_NPCCONV_TYPE_COMBO, m_ctrConvType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_NPCCONV_INDEX_EDITOR, m_dwNPCIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_NPCCONV_CONV_EDITOR, m_uiConv);
	DDV_MinMaxUInt(pDX, m_uiConv, 0, 0xffffffff);
}

BOOL CAttr_ACT_NPCConv::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrConvType.SetItemData( m_ctrConvType.AddString( _T("Dialog") ), eNPCCONV_TYPE_DIALOGBOX );
	m_ctrConvType.SetItemData( m_ctrConvType.AddString( _T("Tool tip") ), eNPCCONV_TYPE_TOOLTIP );
	int nIdx = m_ctrConvType.AddString( _T("Invalid") );
	m_ctrConvType.SetItemData( nIdx, eNPCCONV_TYPE_INVALID );
	m_ctrConvType.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_NPCConv, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_NPCConv 메시지 처리기입니다.
