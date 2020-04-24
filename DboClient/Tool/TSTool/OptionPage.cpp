// OptionPage.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include "OptionPage.h"
#include "OptionSheet.h"

#pragma pack(push, 1)
typedef struct
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;
#pragma pack(pop)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_STATIC inline BOOL IsDialogEx(const DLGTEMPLATE* pTemplate)
{
	return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
}


/////////////////////////////////////////////////////////////////////////////
// COptionPage dialog

IMPLEMENT_DYNAMIC(COptionPage, CDialog);

COptionPage::COptionPage()
{
	//{{AFX_DATA_INIT(COptionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionPage::COptionPage(UINT nIDTemplate, UINT nIDCaption )
: CDialog(nIDTemplate)
{
	CommonConstruct(nIDCaption);
}

COptionPage::COptionPage(LPCTSTR lpszTemplateName, UINT nIDCaption)
: CDialog(lpszTemplateName)
{
	CommonConstruct(nIDCaption);
}

void COptionPage::CommonConstruct(UINT nIDCaption)
{
	if(NULL != GetTemplateName()) {
		BOOL result;
		DLGTEMPLATE* pTemplate;
		CDialogTemplate tmpl;
		LPWORD pw;

		// Load the template
		result = tmpl.Load(GetTemplateName());
		ASSERT(TRUE == result);

		// Get a pointer to the template memory
		pTemplate = (DLGTEMPLATE*)GlobalLock(tmpl.m_hTemplate);
		pw = (LPWORD)pTemplate;

		// Skip it
		if (IsDialogEx(pTemplate)) {
			pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
		} else {
			pw = (WORD*)(pTemplate + 1);
		}

		// Skip menu name string or ordinal
		if (*pw == 0xffff) {
			pw += 2; // WORDs
		} else {
			while(*pw++);
		}

		// Skip class name string or ordinal
		if (*pw == 0xffff) {
			pw += 2; // WORDs
		} else {
			while(*pw++);
		}

		// Get the caption string
		m_Caption = (wchar_t*)pw;

		// Unlock the template memory
		GlobalUnlock(tmpl.m_hTemplate);
	}

	if(0 != nIDCaption) {
		m_Caption.LoadString(nIDCaption);
	}
}













BOOL COptionPage::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialog::Create(lpszTemplateName, pParentWnd);
}


BOOL COptionPage::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	return CDialog::Create(nIDTemplate, pParentWnd);
}


BOOL COptionPage::CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate,
								 CWnd* pParentWnd,
								 void* lpDialogInit)
{
	return CDialog::CreateIndirect(lpDialogTemplate, pParentWnd, lpDialogInit);
}


BOOL COptionPage::CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd)
{
	return CDialog::CreateIndirect(hDialogTemplate, pParentWnd);
}

#define DEFAULT_STYLE	(WS_CHILDWINDOW |	\
						 WS_OVERLAPPED |	\
						 DS_3DLOOK |		\
						 DS_SETFONT |		\
						 DS_CONTROL)

#define DEFAULT_EXSTYLE	(WS_EX_LEFT |			\
						 WS_EX_LTRREADING |		\
						 WS_EX_RIGHTSCROLLBAR |	\
						 WS_EX_WINDOWEDGE |		\
						 WS_EX_CONTROLPARENT)

struct DLGTEMPLATE2 {
	DLGTEMPLATE	tmpl;
	short	stuff[5];
};

BOOL COptionPage::Create(CRect &rect, CWnd *pParentWnd)
{
	ASSERT_VALID(this);
	ASSERT(NULL != pParentWnd);
	ASSERT_KINDOF(CWnd, pParentWnd);


	LPCSTR			pTmplName;
	BOOL			result;
	LPDLGTEMPLATE	pTemplate;
	CDialogTemplate tmpl;
	DLGTEMPLATE2	localTemplate;

	pTmplName = GetTemplateName();

	if(NULL != pTmplName) {
		// Load the template
		result = tmpl.Load(GetTemplateName());
		if(FALSE == result) {
			return FALSE;
		}

		// Fix up the dialog window styles
		pTemplate = (LPDLGTEMPLATE)GlobalLock(tmpl.m_hTemplate);
	} else {
		pTemplate = (DLGTEMPLATE*)&localTemplate;
		memset(&localTemplate, 0, sizeof(localTemplate));
	}

	if(0xffff == ((DLGTEMPLATEEX*)pTemplate)->signature) {
		((DLGTEMPLATEEX*)pTemplate)->style = DEFAULT_STYLE;
		((DLGTEMPLATEEX*)pTemplate)->exStyle = DEFAULT_EXSTYLE;
	} else {
		pTemplate->style = DEFAULT_STYLE;
		pTemplate->dwExtendedStyle = DEFAULT_EXSTYLE;
	}

	if(NULL != pTmplName) {
		GlobalUnlock(tmpl.m_hTemplate);
		// Create a modaless dialog
		CreateIndirect(tmpl.m_hTemplate, pParentWnd);
	} else {
		// Create a modaless dialog
		CreateIndirect(pTemplate, pParentWnd);
	}

	MoveWindow(rect);

	return (NULL != m_hWnd ? TRUE : FALSE);
}



















void COptionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage, CDialog)
	//{{AFX_MSG_MAP(COptionPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPage message handlers
void COptionPage::GetCaption(CString &str)
{
	str = m_Caption;
}

BOOL COptionPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetWindowText(m_Caption);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionPage::SetModified(BOOL bChanged)
{
	COptionSheet *wnd = (COptionSheet*)GetParent();
	ASSERT(NULL != wnd);
	ASSERT_KINDOF(COptionSheet, wnd);
	wnd->SetModified(bChanged);
}


// HANDLED in COptionSheet
BOOL COptionPage::OnApply()
{
	ASSERT_VALID(this);
	OnOK();
	return TRUE;
}


void COptionPage::OnReset()
{
	ASSERT_VALID(this);
	OnCancel();
}


// HANDLED in COptionSheet
void COptionPage::OnOK()
{
	ASSERT_VALID(this);
	//CDialog::OnOK();
}


// HANDLED in COptionSheet
void COptionPage::OnCancel()
{
	ASSERT_VALID(this);
}


// HANDLED in COptionSheet
BOOL COptionPage::OnSetActive()
{
	ASSERT_VALID(this);
	if (!UpdateData(FALSE))
	{
		TRACE0("UpdateData failed during page activation\n");
		return FALSE;
	}
	return TRUE;
}


// HANDLED in COptionSheet
BOOL COptionPage::OnKillActive()
{
	ASSERT_VALID(this);
	if (!UpdateData())
	{
		TRACE0("UpdateData failed during page deactivation\n");
		return FALSE;
	}
	
	return TRUE;
}


// HANDLED in COptionSheet
BOOL COptionPage::OnQueryCancel()
{
	return TRUE;
}


// HANDLED in COptionSheet
LRESULT COptionPage::OnWizardBack()
{
	return 0;
}

// HANDLED in COptionSheet
LRESULT COptionPage::OnWizardNext()
{
	return 0;
}


// HANDLED in COptionSheet
BOOL COptionPage::OnWizardFinish()
{
	return TRUE;
}

UINT COptionPage::GetIconID()
{
	return 0;
}


