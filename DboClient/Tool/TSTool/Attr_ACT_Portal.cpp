// Attr_ACT_Portal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_Portal.h"


// CAttr_ACT_Portal 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_Portal, CAttr_Page, 1)

CAttr_ACT_Portal::CAttr_ACT_Portal()
	: CAttr_Page(CAttr_ACT_Portal::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwWIdx( 0xffffffff )
{

}

CAttr_ACT_Portal::~CAttr_ACT_Portal()
{
}

CString CAttr_ACT_Portal::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("type"), (int)m_ctrPortalType.GetItemData( m_ctrPortalType.GetCurSel() ) );
	strData += PakingPageData( _T("widx"), m_dwWIdx );
	strData += PakingPageData( _T("px"), m_strPosX );
	strData += PakingPageData( _T("py"), m_strPosY );
	strData += PakingPageData( _T("pz"), m_strPosZ );
	strData += PakingPageData( _T("dx"), m_strDirX );
	strData += PakingPageData( _T("dy"), m_strDirY );
	strData += PakingPageData( _T("dz"), m_strDirZ );

	return strData;
}

void CAttr_ACT_Portal::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrPortalType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrPortalType.GetItemData( i ) == nValue )
			{
				m_ctrPortalType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("widx") == strKey )
	{
		m_dwWIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("px") == strKey )
	{
		m_strPosX = strValue;
	}
	else if ( _T("py") == strKey )
	{
		m_strPosY = strValue;
	}
	else if ( _T("pz") == strKey )
	{
		m_strPosZ = strValue;
	}
	else if ( _T("dx") == strKey )
	{
		m_strDirX = strValue;
	}
	else if ( _T("dy") == strKey )
	{
		m_strDirY = strValue;
	}
	else if ( _T("dz") == strKey )
	{
		m_strDirZ = strValue;
	}
}

void CAttr_ACT_Portal::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_PORTAL_TYPE_COMBO, m_ctrPortalType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_WORLD_INDEX_EDITOR, m_dwWIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_POSITION_X_EDITOR, m_strPosX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_POSITION_Y_EDITOR, m_strPosY);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_POSITION_Z_EDITOR, m_strPosZ);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_DIR_X_EDITOR, m_strDirX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_DIR_Y_EDITOR, m_strDirY);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PORTAL_DIR_Z_EDITOR, m_strDirZ);
}

BOOL CAttr_ACT_Portal::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrPortalType.SetItemData( m_ctrPortalType.AddString( _T("Teleport") ), ePORTAL_TYPE_TELEPORT );
	int nIdx = m_ctrPortalType.AddString( _T("Gateway") );
	m_ctrPortalType.SetItemData( nIdx, ePORTAL_TYPE_GATEWAY );
	m_ctrPortalType.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_Portal, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_Portal 메시지 처리기입니다.
