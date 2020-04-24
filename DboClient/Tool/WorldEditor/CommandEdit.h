#pragma once

class CConsoleDialog;

#include "BaseType.h"

// CCommandEdit

class CCommandEdit : public CEdit
{
	DECLARE_DYNAMIC(CCommandEdit)

private:

	CConsoleDialog * m_pParent;

public:

	void SetParent( CConsoleDialog * pParent ) { m_pParent = pParent; }

public:
	CCommandEdit();
	virtual ~CCommandEdit();
	BOOL PreTranslateMessage( MSG* pMsg );

protected:
	DECLARE_MESSAGE_MAP()
};


