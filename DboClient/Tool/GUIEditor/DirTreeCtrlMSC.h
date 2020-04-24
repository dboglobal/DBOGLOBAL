//-----------------------------------------------------------------------------
// Name: class CDirTreeCtrlMSC
// Desc: 단지 메세지만을 오버라이드하기 위함.
//
// 2006.02.6 Peessi@hitel.net  
//-----------------------------------------------------------------------------
#pragma once
#include "DirTreeCtrl.h"
#include "ExtFilter.h"

class CDirTreeCtrlMSC :	public CDirTreeCtrl
{
public:
	CDirTreeCtrlMSC(void);
	~CDirTreeCtrlMSC(void);

	CString	GetFullDir( CString& strFullPath );

	void	LoadDevILImage( CString& strFullPath, CString& strSubPath );
	void	LoadRsr( CString& strFullPath, CString& strSubPath );
	void	LoadSrf( CString& strFullPath, CString& strSubPath );

protected:
	CExtFilter	m_extFilter;
	CString		m_strCurrentPath;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
};
