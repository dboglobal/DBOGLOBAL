// EntityRegisterDiag.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "EntityRegisterDiag.h"
#include "Attr_Page.h"


// CEntityRegisterDiag 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEntityRegisterDiag, COptionSheet)

CEntityRegisterDiag::CEntityRegisterDiag(CWnd* pParent /*=NULL*/)
	: COptionSheet(_T("Entity Register Dialog"), pParent)
{

}

CEntityRegisterDiag::~CEntityRegisterDiag()
{
}

CAttr_Page* CEntityRegisterDiag::GetEditedAttrPage( void )
{
	return m_pEditedPage;
}

void CEntityRegisterDiag::AddAttrPage( CAttr_Page* pPage )
{
	AddGroup( pPage );
}

void CEntityRegisterDiag::ClearAttrPage( void )
{
	m_Pages.RemoveAll();
}

INT_PTR CEntityRegisterDiag::DoModal()
{
	return COptionSheet::DoModal();
}

void CEntityRegisterDiag::DoDataExchange(CDataExchange* pDX)
{
	COptionSheet::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEntityRegisterDiag, COptionSheet)
END_MESSAGE_MAP()


// CEntityRegisterDiag 메시지 처리기입니다.

BOOL CEntityRegisterDiag::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( IDOK == LOWORD( wParam ) && NULL != lParam )
	{
		m_pEditedPage = DYNAMIC_DOWNCAST( CAttr_Page, GetActivePage() );
	}
	else
	{
		m_pEditedPage = NULL;
	}

	return COptionSheet::OnCommand(wParam, lParam);
}
