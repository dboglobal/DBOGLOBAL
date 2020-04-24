// OptionSheet.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include "OptionSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define DEFAULT_DLG_STYLE	(WS_POPUPWINDOW |	\
							 WS_VISIBLE |		\
							 WS_CLIPSIBLINGS |	\
							 WS_DLGFRAME |		\
							 WS_OVERLAPPED |	\
							 DS_MODALFRAME)

#define DEFAULT_DLG_EXSTYLE	(WS_EX_LEFT |			\
							 WS_EX_LTRREADING |		\
							 WS_EX_RIGHTSCROLLBAR |	\
							 WS_EX_DLGMODALFRAME |	\
							 WS_EX_WINDOWEDGE |		\
							 WS_EX_CONTROLPARENT)

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

// DLGTEMPLATEEX taken from afximpl.h
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


COptionSheet::COptionList::COptionList()
{
}

COptionSheet::COptionList::~COptionList()
{
}

BOOL COptionSheet::COptionList::NotifyParentOfSelChange()
{
	// TODO: Add your control notification handler code here
	CWnd *wnd;
	NMHDR hdr;

	// Get the control window
	wnd = GetControlWnd();
	ASSERT_KINDOF(CWnd, wnd);

	// Set up the notification message object
	hdr.code = OSN_SELCHANGE;
	hdr.idFrom = ::GetWindowLong(wnd->GetSafeHwnd(), GWL_ID);
	hdr.hwndFrom = wnd->GetSafeHwnd();

	// Get the parent
	wnd = wnd->GetParent();
	ASSERT_KINDOF(CWnd, wnd);

	// Send the message
	return wnd->SendMessage(WM_NOTIFY, hdr.idFrom, (DWORD)&hdr);
}


BOOL COptionSheet::COptionList::NotifyParentOfSelChanging()
{
	// TODO: Add your control notification handler code here
	CWnd *wnd;
	NMHDR hdr;
	
	// Get the control window
	wnd = GetControlWnd();
	ASSERT_KINDOF(CWnd, wnd);

	// Set up the notification message object
	hdr.code = OSN_SELCHANGING;
	hdr.idFrom = ::GetWindowLong(wnd->GetSafeHwnd(), GWL_ID);
	hdr.hwndFrom = wnd->GetSafeHwnd();

	// Get the parent
	wnd = wnd->GetParent();
	ASSERT_KINDOF(CWnd, wnd);

	// Send the message
	return (BOOL)wnd->SendMessage(WM_NOTIFY, hdr.idFrom, (DWORD)&hdr);
}


// Taken from afximpl.h (AfxHelpEnabled)
static BOOL AFXAPI IsHelpEnabled()
{
	if (AfxGetApp() == NULL)
		return FALSE;

	// help is enabled if the app has a handler for ID_HELP
	AFX_CMDHANDLERINFO info;

	// check main window first
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && pWnd->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info))
		return TRUE;

	// check app last
	return AfxGetApp()->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info);
}




IMPLEMENT_DYNAMIC(COptionSheet, CDialog);

BEGIN_MESSAGE_MAP(COptionSheet, CDialog)
	//{{AFX_MSG_MAP(COptionSheet)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(OSN_SELCHANGE, IDC_PAGELIST, ListSelectionChanged)
	ON_NOTIFY(OSN_SELCHANGING, IDC_PAGELIST, ListSelectionChanging)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(ID_WIZFINISH, OnFinish)
	ON_COMMAND(ID_WIZNEXT, OnNext)
	ON_COMMAND(ID_WIZBACK, OnBack)
	ON_COMMAND(ID_APPLY_NOW, OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet
void COptionSheet::CommonConstruct(CWnd *pParentWnd, UINT iSelectPage)
{
	ASSERT(iSelectPage >= 0);

	memset(&m_psh, 0, sizeof(m_psh));

	m_ListWindow = NULL;
	m_StartPageIndex = iSelectPage;
	m_psh.dwFlags = OSH_PROPSHEETPAGE;
	m_ButtonYLocOffset = 0;
	m_ButtonWidth = 0;
	m_AllButtonsWidth = 0;
	m_YBorder = 0;
	m_XBorder = 0;
	m_ButtonSpacing = 0;
	m_ButtonHeight = 0;
	m_CurrentPage = NULL;
	m_DlgTemplate = NULL;
	m_ListWidth = 200;

	if (FALSE != IsHelpEnabled()) {
		m_psh.dwFlags |= OSH_HASHELP;
	}
}


COptionSheet::COptionSheet() : CDialog()
{
	m_strCaption = _T("");
	CommonConstruct(NULL, 0);
}

COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CDialog()
{
	ASSERT(NULL != pszCaption);

	m_strCaption = pszCaption;
	CommonConstruct(pParentWnd, iSelectPage);
}

COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CDialog()
{
	ASSERT(nIDCaption >= 0);

	VERIFY(m_strCaption.LoadString(nIDCaption));
	CommonConstruct(pParentWnd, iSelectPage);
}


void COptionSheet::Construct(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
{
	ASSERT(0 != nIDCaption);

	m_StartPageIndex = iSelectPage;
	m_pParentWnd = pParentWnd;
	m_strCaption.LoadString(nIDCaption);
}


void COptionSheet::Construct(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
{
	ASSERT(NULL != pszCaption);

	m_StartPageIndex = iSelectPage;
	m_pParentWnd = pParentWnd;
	m_strCaption = pszCaption;
}


COptionSheet::~COptionSheet()
{
	// FIXME Remove option item containers
	if(NULL != m_DlgTemplate) {
		free(m_DlgTemplate);
		m_DlgTemplate = NULL;
	}

	// Get rid of the font
	if(NULL != m_Font.GetSafeHandle()) {
		m_Font.DeleteObject();
	}

	// Remove buttons
	while(FALSE == m_Buttons.IsEmpty()) {
		delete m_Buttons.RemoveHead();
	}

	// Remove all pages
	int i;
	for(i = 0; i < m_Pages.GetSize(); i++) {
		delete m_Pages[i];
		m_Pages[i] = NULL;
	}

}



//////////////////////////////////////////////////////////////////////////////
//
//	Internal helper methods
//
//////////////////////////////////////////////////////////////////////////////
DLGTEMPLATE* COptionSheet::CreateDlgTemplate(DWORD dwStyle, DWORD dwExStyle)
{
	int size = sizeof(DLGTEMPLATE) + 6;
	DLGTEMPLATE *tmpl = (DLGTEMPLATE *)malloc(size);

	memset(tmpl, 0, size);

	// Check the default style
	if(dwStyle == (DWORD)-1) {
		tmpl->style = DEFAULT_DLG_STYLE;
	} else {
		tmpl->style = dwStyle;
	}

	if(dwExStyle == 0) {
		tmpl->dwExtendedStyle = DEFAULT_DLG_EXSTYLE;
	} else {
		tmpl->dwExtendedStyle = dwExStyle;
	}

	// Check for using context help
	if(m_psh.dwFlags & OSH_WIZARDCONTEXTHELP) {
		tmpl->dwExtendedStyle |= WS_EX_CONTEXTHELP;
	}

	// Set up default params
	tmpl->cdit = 0;
	tmpl->x = 0;
	tmpl->y = 0;
	tmpl->cx = 100;
	tmpl->cy = 100;

	return tmpl;
}


BOOL COptionSheet::IsWizardMode()
{
	return (m_psh.dwFlags & OSH_WIZARD ? TRUE : FALSE);
}


void COptionSheet::ClearFlags(DWORD flags)
{
	// Clear the specified flags
	m_psh.dwFlags &= flags;
}

void COptionSheet::SetFlags(DWORD flags)
{
	// Set the specified flags
	m_psh.dwFlags |= flags;
}


void COptionSheet::SetModified(BOOL bModified)
{
	ASSERT(TRUE == bModified || FALSE == bModified);

	// Set the pages are modified and update the buttons
	m_Modified = bModified;

	UpdateAllButtonsState();

}


BOOL COptionSheet::IsModified()
{
	return m_Modified;
}



//////////////////////////////////////////////////////////////////////////////
//
//	Options and settings methods
//
//////////////////////////////////////////////////////////////////////////////
void COptionSheet::SetListControl(COptionList *listCtrl)
{
	m_ListWindow = listCtrl;
}

CWnd *COptionSheet::GetListControlWnd()
{
	// Get the child list control window
	ASSERT(NULL != m_ListWindow);
	CWnd *wnd = m_ListWindow->GetControlWnd();
	ASSERT_KINDOF(CWnd, wnd);
	return wnd;
}



void COptionSheet::SetListWidth(int nWidth)
{
	ASSERT(nWidth >= 0);
	m_ListWidth = nWidth;
}


int COptionSheet::GetActiveIndex() const
{
	if(NULL == m_CurrentPage) {
		return -1;
	}

	return GetPageIndex(m_CurrentPage);
}


int COptionSheet::GetPageIndex(COptionPage *page) const
{
	ASSERT(NULL != page);

	for(int i = 0; i < m_Pages.GetSize(); i++) {
		if(GetPage(i) == page) {
			return i;
		}
	}

	return - 1;
}


int COptionSheet::GetPageCount()
{
	return m_Pages.GetSize();

}


COptionPage *COptionSheet::GetPage(int nPage) const
{
	ASSERT(nPage >= 0 && nPage < m_Pages.GetSize());
	return m_Pages[nPage]->m_Page;
}


COptionPage *COptionSheet::GetPageGroup(int nPage) const
{
	ASSERT(nPage >= 0 && nPage < m_Pages.GetSize());
	return m_Pages[nPage]->m_GroupPage;
}


COptionPage* COptionSheet::GetActivePage() const
{
	return m_CurrentPage;
}


//FIXME - must check to not set a new active page!
BOOL COptionSheet::SetActivePage(COptionPage *newPage)
{
	ASSERT(NULL != newPage);
	ASSERT_KINDOF(COptionPage, newPage);

	BOOL result;
	COptionPage *oldPage;

	// Save the current item
	oldPage = m_CurrentPage;

	// Destroy the old item
	if(NULL != oldPage) {
		result = oldPage->OnKillActive();	// Let it know it is gonna die
		if(FALSE == result) {
			return FALSE;
		}
		oldPage->OnOK();
		oldPage->DestroyWindow();
	}

	// Create the new page and tell it it's active
	CreatePage(newPage);
	newPage->OnSetActive();
	newPage->ShowWindow(SW_SHOW);

	// Find the index
	for(int i = 0; i < m_Pages.GetSize(); i++) {
		if(newPage == GetPage(i)) {
			m_PageIndex = i;
			break;
		}
	}

	// Set the current page
	m_CurrentPage = newPage;

	// Set the current selection
	if(NULL != m_ListWindow) {
		m_ListWindow->SetSelection(m_CurrentPage);
	}

	// Update all button states
	UpdateAllButtonsState();

	return TRUE;
}


BOOL COptionSheet::SetActivePage(int nPage)
{
	ASSERT(nPage < GetPageCount());

	COptionPage	*newPage;

	// Get the new page
	if(nPage >= 0) {
		newPage = GetPage(nPage);
	} else {
		newPage = NULL;
	}

	return SetActivePage(newPage);
}


void COptionSheet::SetTitle(LPCTSTR lpszText, UINT nStyle)
{
	ASSERT(NULL != lpszText);

	// Reset the internal title
	m_strCaption = _T("");

	if(nStyle & OSH_OPTIONTITLE) {
		m_strCaption = CString("Options for ");
	}

	m_strCaption += lpszText;

	if(NULL != GetSafeHwnd()) {
		SetWindowText(m_strCaption);
	}
}


void COptionSheet::SetTitle(UINT nID, UINT nStyle)
{
	BOOL	result;
	CString str;

	result = str.LoadString(nID);
	ASSERT(FALSE != result);

	SetTitle(str, nStyle);
}


void COptionSheet::SetFinishText(LPCSTR lpszText)
{
	ASSERT(NULL != lpszText);

	if(TRUE == IsWizardMode()) {
		CWnd *wnd;

		wnd = GetDlgItem(ID_WIZFINISH);
		if(NULL != wnd) {
			wnd->SetWindowText(lpszText);
			wnd->ShowWindow(SW_SHOW);
		}

		wnd = GetDlgItem(ID_WIZBACK);
		if(NULL != wnd) {
			wnd->ShowWindow(SW_HIDE);
		}

		wnd = GetDlgItem(ID_WIZNEXT);
		if(NULL != wnd) {
			wnd->ShowWindow(SW_HIDE);
		}
	}

}


void COptionSheet::SetWizardButtons(DWORD dwFlags)
{
	m_WizButFlags = dwFlags;
}


void COptionSheet::SetWizardMode(BOOL bFullPage)
{
	DWORD flags;

	flags = OSH_WIZARD;
	if(TRUE == bFullPage) {
		flags |= OSH_WIZARDFULL;
	}
	SetWizardButtons(OSWIZB_BACK | OSWIZB_NEXT | OSWIZB_FINISH);
	SetFlags(flags);
}








//////////////////////////////////////////////////////////////////////////////
//
//	Operations
//
//////////////////////////////////////////////////////////////////////////////
BOOL COptionSheet::DoModal()
{
	BOOL result;
	int oldWidth;

	// Save the old list control width;
	oldWidth = m_ListWidth;
	if(TRUE == IsWizardMode()) {
		m_ListWidth = 0;
	}

	// Create the template
	m_DlgTemplate = CreateDlgTemplate(-1, 0);


	// Initialize the modal dialog template
	CDialog::InitModalIndirect(m_DlgTemplate, m_pParentWnd);

	// Enter the modal loop
	result = CDialog::DoModal();

	// Retore!
	m_hDialogTemplate = NULL;
	m_pParentWnd = NULL;

	// Restore the old list control width
	m_ListWidth = oldWidth;

	return result;
}



BOOL COptionSheet::Create(CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle)
{
	BOOL result;

	// We do not go into wizard mode here since we cannot statically save the 
	// list control width
	ASSERT(TRUE != IsWizardMode());

	// Make sure we are a popup
	dwStyle |= WS_POPUP;

	m_DlgTemplate = CreateDlgTemplate(dwStyle, dwExStyle);
	
	result = CDialog::CreateIndirect(m_DlgTemplate, pParentWnd);

	m_hDialogTemplate = NULL;
	m_pParentWnd = NULL;

	return result;
}


void COptionSheet::AddPage(COptionPage *page, COptionPage *pGroupPage)
{
	ASSERT(NULL != page);
	ASSERT_KINDOF(COptionPage, page);
	if(NULL != pGroupPage) {
		ASSERT_KINDOF(COptionPage, pGroupPage);
	}

	PageInfo *info;

	info = new PageInfo;
	info->m_Page = page;
	info->m_GroupPage = pGroupPage;
	info->m_IsGroup = FALSE;

	m_Pages.Add(info);

}

void COptionSheet::AddGroup(COptionPage *page)
{
	ASSERT(page != NULL);
	ASSERT_KINDOF(COptionPage, page);

	PageInfo *info;

	info = new PageInfo;
	info->m_Page = page;
	info->m_GroupPage = NULL;
	info->m_IsGroup = TRUE;

	m_Pages.Add(info);
}


void COptionSheet::RemovePage(COptionPage *pPage)
{
	// Not implemented yet
	ASSERT(0);
}


void COptionSheet::RemovePage(int nPage)
{
	// Not implemented yet
	ASSERT(0);
}


BOOL COptionSheet::PressButton(int nButton)
{

	BOOL result = FALSE;

	switch(nButton) {
	case OSBTN_BACK:
		if(TRUE == IsWizardMode()) {
			result = SendMessage(WM_COMMAND, ID_WIZBACK, 0);
		}
		break;

	case OSBTN_NEXT:
		if(TRUE == IsWizardMode()) {
			result = SendMessage(WM_COMMAND, ID_WIZNEXT, 0);
		}
		break;

	case OSBTN_FINISH:
		if(TRUE == IsWizardMode()) {
			result = SendMessage(WM_COMMAND, ID_WIZFINISH, 0);
		}
		break;

	case OSBTN_OK:
		result = SendMessage(WM_COMMAND, IDOK, 0);
		break;

	case OSBTN_APPLYNOW:
		result = SendMessage(WM_COMMAND, ID_APPLY_NOW, 0);
		break;

	case OSBTN_CANCEL:
		result = SendMessage(WM_COMMAND, IDCANCEL, 0);
		break;

	case OSBTN_HELP:
		result = SendMessage(WM_COMMAND, IDHELP, 0);
		break;

	default:
		ASSERT(0);
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////////
//
//	Initialization methods
//
//////////////////////////////////////////////////////////////////////////////

void COptionSheet::CalcTreeRect(CRect &rect)
{
	GetClientRect(rect);
	rect.left = PAGE_SPACING;
	rect.right = rect.left + m_ListWidth;
	rect.top = PAGE_SPACING;
	rect.bottom -= (PAGE_SPACING + m_ButtonSpacing);
}



void COptionSheet::AddButton(char *txt, int x, int w, UINT id, BOOL showButton)
{
	CButton *btn;
	CRect rect;
	CRect rcClient;

	GetClientRect(rcClient);

	rect.left = x;
	rect.right = x + w;
	rect.top = rcClient.Height() - (PAGE_SPACING2 + m_ButtonHeight);
	rect.bottom = rect.top + m_ButtonHeight;

	btn = new CButton();

	DWORD style = WS_TABSTOP | BS_DEFPUSHBUTTON;

	if(TRUE == showButton) {
		style |= WS_VISIBLE;
	}

	btn->Create(txt, style, rect, this, id);
	btn->SetFont(&m_Font);
	m_Buttons.AddTail(btn);
}

int COptionSheet::GetButtonWidth(UINT id)
{
	int retval = 0;

	switch(id) {
	case IDOK:
	case IDCANCEL:
		retval = m_ButtonWidth;
		break;
	case IDHELP:
		if(m_psh.dwFlags & OSH_HASHELP) {
			retval = m_ButtonWidth;
		}
		break;
	default:
		if(TRUE == IsWizardMode()) {
			switch(id) {
			case ID_WIZBACK:
			case ID_WIZNEXT:
				retval = m_ButtonWidth;
			case ID_WIZFINISH:
				if(m_psh.dwFlags & OSH_WIZARDHASFINISH) {
					retval = m_ButtonWidth;
				}
				break;
			}
		} else {
			ASSERT(0);
		}
		break;
	}

	return 0;
}

int COptionSheet::AddButtons(BOOL addBut)
{
	int		x;
	CRect	rcClient;

	// Add the standard buttons
	GetClientRect(rcClient);

	if(TRUE == addBut) {
		x = rcClient.Width() - (PAGE_SPACING2 + m_AllButtonsWidth);
	} else {
		x = 0;
	}

	if(TRUE == IsWizardMode()) {

		// Add the back button
		if(TRUE == addBut) {
			AddButton("< Back", x, m_ButtonWidth, ID_WIZBACK);
		}
		x += m_ButtonWidth;

		// Add the next button
		if(TRUE == addBut) {
			AddButton("Next >", x, m_ButtonWidth, ID_WIZNEXT);
		}

		// Add in the finish button
		if(m_psh.dwFlags & OSH_WIZARDHASFINISH) {
			x += (m_ButtonWidth + BUTTON_SPACING);
			if(TRUE == addBut) {
				AddButton("Finish", x, m_ButtonWidth, ID_WIZFINISH);
			}
		} else {
			if(TRUE == addBut) {
				AddButton("Finish", x, m_ButtonWidth, ID_WIZFINISH, FALSE);
			}
		}

		// Add the finish button
		x += m_ButtonWidth;

	} else {

		// Add the OK button
		if(TRUE == addBut) {
			AddButton("OK", x, m_ButtonWidth, IDOK);
		}
		x += m_ButtonWidth;

		// Add the Apply button
		if(!(m_psh.dwFlags & OSH_NOAPPLYNOW)) {
			if(TRUE == addBut) {
				AddButton("Apply", x, m_ButtonWidth, ID_APPLY_NOW);
			}
			x += m_ButtonWidth;
		}

	}

	// Add the cancel button
	x += BUTTON_SPACING;
	if(TRUE == addBut) {
		AddButton("Cancel", x, m_ButtonWidth, IDCANCEL);
	}
	x += m_ButtonWidth;

	// Add the help button
	if(m_psh.dwFlags & OSH_HASHELP) {
		x += BUTTON_SPACING;
		if(TRUE == addBut) {
			AddButton("Help", x, m_ButtonWidth, IDHELP);
		}
		x += m_ButtonWidth;
	}
	return x;
}


BOOL COptionSheet::CalcSpacing(CFont &buttonFont)
{
	TEXTMETRIC	txtMetric;
	CRect		rcClient;
	CRect		rcWindow;
	COptionPage *page;
	CRect		rect;
	
	GetClientRect(rcClient);
	GetWindowRect(rcWindow);

	// Get the client area used by the window
	m_XBorder = rcWindow.Width() - rcClient.Width();
	m_YBorder = rcWindow.Height() - rcClient.Height();

	// Get the rect metrics of the font used on the buttons
	CDC *dc = GetDC();
	dc->SelectObject(buttonFont);
	dc->GetTextMetrics(&txtMetric);
	ReleaseDC(dc);

	// Calculate the height of the buttons and the button spacing
	m_ButtonHeight = (txtMetric.tmHeight + txtMetric.tmExternalLeading) + 10;
	m_ButtonYLocOffset = PAGE_SPACING2 + m_ButtonHeight;
	m_ButtonSpacing = m_ButtonHeight + PAGE_SPACING;
	m_ButtonWidth = BUTTON_WIDTH;

	// Adjust the button spacing if in full page wizard mode
	if(m_psh.dwFlags & OSH_WIZARDFULL) {
		m_ButtonSpacing += PAGE_SPACING;
	}

	// Calculate the space needed for the buttons
	m_AllButtonsWidth = 0;

	m_AllButtonsWidth = AddButtons(FALSE);

	// Calculate the sizes of the option pages
	m_MaxChildSize.cx = 0;
	m_MaxChildSize.cy = 0;

	for(int i = 0; i < GetPageCount(); i++) {
		BOOL result;
		CDialogTemplate tmpl;
		SIZE size;

		page = GetPage(i);

		// FIXME
		if(NULL != page->GetTemplateName()) {
			// Create the child dialog and get the rect
			result = tmpl.Load(page->GetTemplateName());
			ASSERT(TRUE == result);

			tmpl.GetSizeInPixels(&size);

			// Calc the size
			if(size.cx > m_MaxChildSize.cx) {
				m_MaxChildSize.cx = size.cx;
			}

			if(size.cy > m_MaxChildSize.cy) {
				m_MaxChildSize.cy = size.cy;
			}
		}
	}

	return TRUE;
}


BOOL COptionSheet::IsPageAGroup(int nPage)
{
	ASSERT(nPage >= 0 && nPage < m_Pages.GetSize());
	return m_Pages[nPage]->m_IsGroup;
}


void COptionSheet::BuildList()
{
	ASSERT(NULL != m_ListWindow);

	CWnd *wnd;
	int i;

	// Get the list control window
	wnd = GetListControlWnd();

	// Create the list control
	m_ListWindow->Create(this, IDC_PAGELIST);

	// Set the controls font
	wnd->SetFont(&m_Font);

	// Add the groups
	for(i = 0; i < GetPageCount(); i++) {
		if(TRUE == IsPageAGroup(i)) {
			m_ListWindow->AddGroup(GetPage(i));
		}
	}

	// Add the pages
	for(i = 0; i < GetPageCount(); i++) {
		if(FALSE == IsPageAGroup(i)) {
			m_ListWindow->AddPage(GetPage(i), GetPageGroup(i));
		}
	}

	wnd->ShowWindow(SW_SHOW);
}


BOOL COptionSheet::OnInitDialog() 
{
	BOOL		result;
	CRect		rcClient;
	int			width;
	int			height;

	// Call the default behaviour
	CDialog::OnInitDialog();

	m_CurrentPage = NULL;

	// Create out button font
	if(NULL != m_Font.GetSafeHandle()) {
		m_Font.DeleteObject();
	}

	m_Font.Attach(GetStockObject(DEFAULT_GUI_FONT));

	// Set the icon
	if(m_psh.dwFlags & OSH_USEHICON && NULL != m_psh.hIcon) {
		SetIcon(m_psh.hIcon, FALSE);
	}

	// Calculate spacing needed by all items
	result = CalcSpacing(m_Font);
	if(FALSE == result) {
		ASSERT(0);
		DestroyWindow();
		return TRUE;
	}

	// Adjust our X position to compensate for the biggest child dialog
	width = m_MaxChildSize.cx;
	if(m_psh.dwFlags & OSH_WIZARDFULL) {
		width += (FULLPAGE_SPACING * 2);
	} else {
		width += (PAGE_SPACING * 2);
	}
	if(0 != m_ListWidth) {
		width += m_ListWidth + PAGE_SPACING;
	}

	if(width < m_AllButtonsWidth) {
		width = m_AllButtonsWidth;
	}

	width += m_XBorder;


	// Adjust our X position to compensate for the biggest child dialog
	height = m_YBorder + m_MaxChildSize.cy + m_ButtonSpacing;
	if(!(m_psh.dwFlags & OSH_WIZARDFULL)) {
		height += (PAGE_SPACING * 2);
	}
	
	// Set the new window position
	SetWindowPos(&wndTop,
				 0,
				 0,
				 width, 
				 height,
				 SWP_NOMOVE | SWP_NOZORDER);

	// Set the title
	if(0 != m_strCaption.GetLength()) {
		SetWindowText(m_strCaption);
	}


	// Add the buttons
	AddButtons();

	// IF we are not in wizard mode, select the first root tree item
	// otherwise, display the first page in the list.
	if(FALSE == IsWizardMode()) {
		CRect rect;
		// Select the default item
		BuildList();
		CalcTreeRect(rect);
		GetListControlWnd()->MoveWindow(rect);
	}

	SetActivePage(m_StartPageIndex);

	SetModified(FALSE);

	// We are done!
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void COptionSheet::OnPaint() 
{
	CPaintDC	dc(this); // device context for painting

	COLORREF	clrDark;
	COLORREF	clrLite;

	// Create gfx items
	clrDark = GetSysColor(COLOR_3DSHADOW);
	clrLite = GetSysColor(COLOR_3DHILIGHT);

	if(m_psh.dwFlags & OSH_WIZARDFULL) {
		CRect rect;
		// Determine the X and Y position of the dialog in dialog units
		rect.top = FULLPAGE_SPACING;
		rect.left = FULLPAGE_SPACING;
		rect.right = rect.left + m_MaxChildSize.cx;
		rect.bottom = rect.top + m_MaxChildSize.cy;


		rect.InflateRect(1, 1, 1, 1);
		dc.Draw3dRect(rect, clrLite, clrDark);
		rect.InflateRect(1, 1, 1, 1);
		dc.Draw3dRect(rect, clrDark, clrLite);
	} else {
		CRect		rcClient;
		int			centerX;
		CPen		penDark;
		CPen		penLite;

		penDark.CreatePen(PS_SOLID, 1, clrDark);
		penLite.CreatePen(PS_SOLID, 1, clrLite);

		// Get the client area and adjust to the area we will draw to
		GetClientRect(rcClient);

		centerX = rcClient.left + PAGE_SPACING + PAGE_SPACING2 + m_ListWidth;

		rcClient.InflateRect(-PAGE_SPACING2,
							 -PAGE_SPACING2,
							 -PAGE_SPACING2,
							 -(PAGE_SPACING2 + m_ButtonSpacing));

		if(0 != m_ListWidth) {
			// Draw the center divider
			dc.SelectObject(penDark);
			dc.MoveTo(centerX, rcClient.top);
			dc.LineTo(centerX, rcClient.bottom);

			dc.SelectObject(penLite);
			dc.MoveTo(centerX + 1, rcClient.top);
			dc.LineTo(centerX + 1, rcClient.bottom);
		}

		// Draw the group box around the child dialogs and the tree control
		dc.Draw3dRect(rcClient, clrDark, clrLite);
		rcClient.InflateRect(-1, -1, -1, -1);
		dc.Draw3dRect(rcClient, clrLite, clrDark);
	}
	
}

void COptionSheet::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// Adjust the tree control
	if(NULL != m_ListWindow) {
		CRect rect;
		CWnd *wnd;

		CalcTreeRect(rect);

		wnd = GetListControlWnd();
		if(FALSE != ::IsWindow(wnd->m_hWnd)) {
			ASSERT_KINDOF(CWnd, wnd);
			GetListControlWnd()->MoveWindow(rect);
		}
	}
}


void COptionSheet::CreatePage(COptionPage *page)
{
	ASSERT(NULL != page);

	BOOL			result;
	CRect			rect;

	// Determine the X and Y position of the dialog in dialog units
	if(m_psh.dwFlags & OSH_WIZARDFULL) {
		rect.top = FULLPAGE_SPACING;
		rect.left = FULLPAGE_SPACING;
	} else {
		rect.top = PAGE_SPACING;
		rect.left = PAGE_SPACING;
	}

	if(0 != m_ListWidth) {
		rect.left += (m_ListWidth + PAGE_SPACING);
	}
	rect.right = rect.left + m_MaxChildSize.cx;
	rect.bottom = rect.top + m_MaxChildSize.cy;

	// Create a modaless dialog
	result = page->Create(rect, this);
	ASSERT(FALSE != result);
}




void COptionSheet::UpdateAllButtonsState()
{
	// If in wizard mode, process the next/back buttons
	if(TRUE == IsWizardMode()) {
		CWnd *wnd;

		BOOL bNextState;
		BOOL bBackState;

		if(NULL == m_CurrentPage) {
			bNextState = FALSE;
			bBackState = FALSE;
		} else if(0 == m_PageIndex) {
			bNextState = TRUE;
			bBackState = FALSE;
		} else if(m_PageIndex == (GetPageCount() - 1)) {
			bNextState = FALSE;
			bBackState = TRUE;
		} else {
			bNextState = TRUE;
			bBackState = TRUE;
		}

		// Update the "Next" button
		wnd = GetDlgItem(ID_WIZNEXT);
		if(NULL != wnd) {
			wnd->EnableWindow(bNextState);
		}

		// Update the "Back" button
		wnd = GetDlgItem(ID_WIZBACK);
		if(NULL != wnd) {
			wnd->EnableWindow(bBackState);
		}

		// Update the "Finish" button
		wnd = GetDlgItem(ID_WIZFINISH);
		if(NULL != wnd) {
			wnd->EnableWindow((0 == (m_WizButFlags & OSWIZB_DISABLEDFINISH)) ? TRUE : FALSE);
		}
	}

	if(FALSE == IsWizardMode()) {
		CWnd *wnd = GetDlgItem(ID_APPLY_NOW);
		if(NULL != wnd) {
			wnd->EnableWindow(m_Modified);
		}
	}

}

void COptionSheet::ListSelectionChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
/*
	COptionPage *page;

	page = m_ListWindow->GetSelection();
	if(NULL != page) {
		ASSERT_KINDOF(COptionPage, page);
	}
	*pResult = SetActivePage(page);
*/
}



void COptionSheet::ListSelectionChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	COptionPage *page;

	page = m_ListWindow->GetSelection();
	if(NULL != page) {
		ASSERT_KINDOF(COptionPage, page);
	}
	*pResult = SetActivePage(page);
}






void COptionSheet::OnOK()
{
	// lleo52
	// Active 페이지에 한해서 오케이 명령을 내려준다
//	for(int i = 0; i < GetPageCount(); i++) {
//		GetPage(i)->OnOK();
//	}
	if ( GetActivePage() )
	{
		GetActivePage()->OnOK();
	}

	// Ok, time to end it
	EndDialog(IDOK);
}


void COptionSheet::OnFinish()
{
	for(int i = 0; i < GetPageCount(); i++) {
		BOOL result = GetPage(i)->OnWizardFinish();
		if(FALSE == result) {
			return;
		}
	}

	// Ok, time to end it
	EndDialog(ID_WIZFINISH);
}


void COptionSheet::OnApply()
{
	if(TRUE == IsModified()) {
		for(int i = 0; i < GetPageCount(); i++) {
			BOOL result = GetPage(i)->OnApply();
			if(FALSE == result) {
				return;
			}
		}
		SetModified(FALSE);
	}
}


void COptionSheet::OnCancel()
{
	int i;

	for(i = 0; i < GetPageCount(); i++) {
		BOOL result = GetPage(i)->OnQueryCancel();
		if(FALSE == result) {
			return;
		}
	}

	// Go reset and cancel everything
	for(i = 0; i < GetPageCount(); i++) {
		GetPage(i)->OnReset();
		GetPage(i)->OnCancel();
	}

	// Ok, time to end it
	EndDialog(IDCANCEL);
}


void COptionSheet::OnNext(void)
{
	LRESULT result;

	// Make sure there is a current item it has a page
	ASSERT(NULL != m_CurrentPage);

	// Make sure the Next button is enabled
	CWnd *wnd = GetDlgItem(ID_WIZNEXT);
	if(NULL == wnd) {
		return;
	}

	if(FALSE == wnd->IsWindowEnabled()) {
		return;
	}

	// Check to see if we can move to the next page
	result = GetPage(m_PageIndex)->OnWizardNext();

	switch(result) {
	case -1:
		break;
	case 0:	
		SetActivePage(++m_PageIndex);
		break;
	default:
		if(result > 0) {
			SetActivePage(result);
		}
		break;
	}

	return;
}


void COptionSheet::OnBack()
{
	LRESULT result;
	
	// Make sure there is a current item it has a page
	ASSERT(NULL != m_CurrentPage);

	// Make sure the Next button is enabled
	CWnd *wnd = GetDlgItem(ID_WIZBACK);
	if(NULL == wnd) {
		return;
	}

	if(FALSE == wnd->IsWindowEnabled()) {
		return;
	}

	// Check to see if we can move to the next page
	result = GetPage(m_PageIndex)->OnWizardNext();

	switch(result) {
	case -1:
		break;
	case 0:
		SetActivePage(--m_PageIndex);
		break;
	default:
		if(result > 0) {
			SetActivePage(result);
		}
		break;
	}

	return;
}


void COptionSheet::EndDialog(int nEndID)
{
	CDialog::EndDialog(nEndID);
}

