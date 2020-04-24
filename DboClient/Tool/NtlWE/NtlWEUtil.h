#pragma once


class CWnd;


template <class CModalessDialogs>
CModalessDialogs* CreateModalessDialogs(CWnd* _pDesktopWindow)
{
	CModalessDialogs* _pRet = NULL;

	_pRet = new CModalessDialogs;
	_pRet->Create(CModalessDialogs::IDD, _pDesktopWindow);

	return _pRet;
}


class CNtlWEUtil
{
public:
	CNtlWEUtil(void);
	~CNtlWEUtil(void);

public:
	static VOID		DestroyModalessDialogs(CDialog* _pModalessDialogs);
	static RwReal	GetRwRealVal(CString& _Str);
	static RwInt32	GetRwInt32Val(CString& _Str);
	static CString	GetStr(RwReal& _Num);
	static CString	GetStr(RwInt32& _Num);
};

class CSortData
{
public:
	CSortData(CString str, DWORD dw);
	~CSortData(){};

	CString m_strText;
	DWORD	m_dw;
};

int CALLBACK StringCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

int CALLBACK NumberCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);