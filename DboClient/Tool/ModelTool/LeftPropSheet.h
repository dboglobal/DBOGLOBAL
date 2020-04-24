#pragma once



// CLeftPropSheet

class CLeftPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CLeftPropSheet)

public:
	CLeftPropSheet();
	CLeftPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CLeftPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CLeftPropSheet();

protected:
	DECLARE_MESSAGE_MAP()
};


