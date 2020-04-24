// Attr_COND_AvatarDead.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_AvatarDead.h"


// CAttr_COND_AvatarDead 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_AvatarDead, CAttr_Page, 1)

CAttr_COND_AvatarDead::CAttr_COND_AvatarDead()
	: CAttr_Page(CAttr_COND_AvatarDead::IDD)
{

}

CAttr_COND_AvatarDead::~CAttr_COND_AvatarDead()
{
}

CString CAttr_COND_AvatarDead::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	return strData;
}

void CAttr_COND_AvatarDead::UnPakingPageData( CString& strKey, CString& strValue )
{
}

void CAttr_COND_AvatarDead::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAttr_COND_AvatarDead, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_AvatarDead 메시지 처리기입니다.
