// Attr_ACT_OperateObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_OperateObject.h"
#include "NtlSharedType.h"
#include "NtlDirection.h"


// CAttr_ACT_OperateObject 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_OperateObject, CAttr_Page, 1)

CAttr_ACT_OperateObject::CAttr_ACT_OperateObject()
	: CAttr_Page(CAttr_ACT_OperateObject::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwOperateTime(0)
	, m_dwDirectionTblIdx(0xffffffff)
{
}

CAttr_ACT_OperateObject::~CAttr_ACT_OperateObject()
{
}

CString CAttr_ACT_OperateObject::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("apply"), (int)m_ctrTypeCtrl.GetItemData( m_ctrTypeCtrl.GetCurSel() ) );
	strData += PakingPageData( _T("time"), m_dwOperateTime );
	strData += PakingPageData( _T("tblidx"), m_dwDirectionTblIdx );

	return strData;
}

void CAttr_ACT_OperateObject::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("apply") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrTypeCtrl.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrTypeCtrl.GetItemData( i ) == nValue )
			{
				m_ctrTypeCtrl.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("time") == strKey )
	{
		m_dwOperateTime = atoi( strValue.GetBuffer() );
	}
	else if ( _T("tblidx") == strKey )
	{
		m_dwDirectionTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_OperateObject::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPERATEOBJECT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OPERATEOBJECT_TYPE_COMBO, m_ctrTypeCtrl);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPERATEOBJECT_TIME_EDITOR, m_dwOperateTime);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPERATEOBJECT_TBLIDX_EDITOR, m_dwDirectionTblIdx);
}

BOOL CAttr_ACT_OperateObject::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrTypeCtrl.SetItemData( m_ctrTypeCtrl.AddString( _T("종료") ), eOPERATEOBJECT_TYPE_END );
	int nIdx = m_ctrTypeCtrl.AddString( _T("시작") );
	m_ctrTypeCtrl.SetItemData( nIdx, eOPERATEOBJECT_TYPE_START );
	m_ctrTypeCtrl.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_OperateObject, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_OperateObject 메시지 처리기입니다.
