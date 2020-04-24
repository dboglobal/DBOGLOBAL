#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "CommandEdit.h"

// CConsoleDialog 대화 상자입니다.

class CConsoleDialog : public CDialog
{
	DECLARE_DYNAMIC(CConsoleDialog)

	CString				m_Command;
public:
	CConsoleDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConsoleDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONSOLE };

	virtual BOOL OnInitDialog();


	const TCHAR * GetCommand( void ) const { return (LPCTSTR)m_Command; }
	void CancelCommand( void ) { OnCancel(); }
	void SetCommand( TCHAR * pCommand ) { m_Command = pCommand; m_Command.AppendChar( '\n' ); OnOK(); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CCommandEdit m_EditCommand;
};
