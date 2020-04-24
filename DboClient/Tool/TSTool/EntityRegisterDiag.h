#pragma once


#include "OptionSheet.h"


class CAttr_Page;


// CEntityRegisterDiag 대화 상자입니다.

class CEntityRegisterDiag : public COptionSheet
{
	DECLARE_DYNAMIC(CEntityRegisterDiag)

protected:
	CAttr_Page*		m_pEditedPage;

public:
	CEntityRegisterDiag(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEntityRegisterDiag();

	CAttr_Page*		GetEditedAttrPage( void );
	void			AddAttrPage( CAttr_Page* pPage );
	void			ClearAttrPage( void );

	virtual INT_PTR DoModal();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
