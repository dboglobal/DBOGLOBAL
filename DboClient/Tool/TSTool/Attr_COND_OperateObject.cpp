// Attr_COND_OperateObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_OperateObject.h"


// CAttr_COND_OperateObject 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_OperateObject, CAttr_Page, 1)

CAttr_COND_OperateObject::CAttr_COND_OperateObject()
	: CAttr_Page(CAttr_COND_OperateObject::IDD)
{

}

CAttr_COND_OperateObject::~CAttr_COND_OperateObject()
{
}

CString CAttr_COND_OperateObject::GetPageData( void )
{
	UpdateData( TRUE );

	return CString(_T(""));
}

void CAttr_COND_OperateObject::UnPakingPageData( CString& strKey, CString& strValue )
{
}

void CAttr_COND_OperateObject::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAttr_COND_OperateObject, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_OperateObject 메시지 처리기입니다.
