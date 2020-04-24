#pragma once


// CNewAssetOpen

class CNewAssetOpen : public CFileDialog
{
	DECLARE_DYNAMIC(CNewAssetOpen)

public:
	CNewAssetOpen(BOOL bOpenFileDialog, // FileOpen은 TRUE, FileSaveAs는 FALSE입니다.
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CNewAssetOpen();

protected:
	DECLARE_MESSAGE_MAP()
};


