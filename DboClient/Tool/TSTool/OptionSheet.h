/*!
	\file
*/
#if !defined(AFX_OPTIONSHEET_H__05EA0917_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_OPTIONSHEET_H__05EA0917_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSheet window
#include "OptionPage.h"


#define OSBTN_BACK              0	//!<	Use when calling PressButton to simulate pressing the Back button.
#define OSBTN_NEXT              1	//!<	Use when calling PressButton to simulate pressing the Next button.
#define OSBTN_FINISH            2	//!<	Use when calling PressButton to simulate pressing the Finish button.
#define OSBTN_OK                3	//!<	Use when calling PressButton to simulate pressing the OK button.
#define OSBTN_APPLYNOW          4	//!<	Use when calling PressButton to simulate pressing the Apply Now button.
#define OSBTN_CANCEL            5	//!<	Use when calling PressButton to simulate pressing the Cancel button.
#define OSBTN_HELP              6	//!<	Use when calling PressButton to simulate pressing the Help button. 
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define OSBTN_MAX               6	//
#endif

#define OSH_DEFAULT             0x00000000	//!<	Default mode.
#define OSH_OPTIONTITLE			0x00000001	//!<	Prepend "Options for" to option sheet caption.
#define OSH_USEHICON            0x00000002	//!<	Use the hIcon member of the m_psh attribute.
#define OSH_PROPSHEETPAGE       0x00000008	//!<	Use option sheet mode.
#define OSH_WIZARDHASFINISH     0x00000010	//!<	"Finish" button is always displayed on the wizard.
#define OSH_WIZARD              0x00000020	//!<	Use wizard mode.
#define OSH_WIZARDFULL			0x00000040	//!<	Use full page wizard mode.
#define OSH_NOAPPLYNOW          0x00000080	//!<	Option sheet has apply button.
#define OSH_HASHELP             0x00000200	//!<	Option sheet has help button.
#define OSH_WIZARDCONTEXTHELP   0x00001000	//!<	Uses context help.



#define OSWIZB_BACK				0x00000001	//!<	Back button.
#define OSWIZB_NEXT   			0x00000002	//!<	Next button.
#define OSWIZB_FINISH   		0x00000004	//!<	Finish button.
#define OSWIZB_DISABLEDFINISH  	0x00000008	//!<	Disable the finish button.

#define BUTTON_WIDTH	75		//!<	Default button width.
#define BUTTON_SPACING	10		//!<	Default button spacing.
#define PAGE_SPACING	12		//!<	Default layout spacing.
#define FULLPAGE_SPACING	0	//!<	Default layout spacing.
#define PAGE_SPACING2	(PAGE_SPACING / 2)	//!<	Default layout spacing for half steps.
#define IDC_PAGELIST	5000	//!<	ID of the control used to list all available option pages.
#define OSN_SELCHANGE	1		//!<	Notification code to alert the option sheet a selection in the child list has changed.
#define OSN_SELCHANGING	2		//!<	Notification code to alert the option sheet a selection in the child list is changing.


/*!
	Objects of class <b>COptionSheet</b> represent option sheets, otherwise
	known as embedded dialog boxes. An option sheet consists of a COptionSheet
	object and one or more COptionPage objects. A basic option sheet is
	displayed by the framework as a window with a default or user specified
	control containing the list of option pages, with which the user selects
	a page, and an area for the currently selected page. 

	Since COptionSheet is derived from CDialog, managing a COptionSheet object
	is similar to managing a CDialog object. For example, creation of an option
	sheet requires two-part construction: call the constructor, and then call
	DoModal for a modal option sheet or Create for a modeless option sheet.
	COptionSheet has two types of constructors:	COptionSheet::Construct and
	COptionSheet::COptionSheet.

	Exchanging data between a COptionSheet object and some external object is
	similar to exchanging data with a CDialog object. The important difference
	is that the settings of an option sheet are normally member variables of
	the COptionPage objects rather than of the COptionSheet object itself.

	Using COptionSheet, you can also create a dialog box called a wizard,
	which consists of an option sheet with a sequence of option pages that
	guide the user through the steps of an operation, such as setting up a
	device or creating a newsletter. In a wizard-type dialog box, the list of
	option pages are not displayed in a control and only one option	page is
	visible at a time. Also, instead of having OK and Apply buttons, a
	wizard-type dialog box has a Back button, a Next button, a Finish button,
	a Cancel button, and optionally a Help button.

	To create a wizard-type dialog box, follow the same steps you would follow
	to create a standard option sheet, but call SetWizardMode before you call
	DoModal. To enable the wizard buttons, call SetWizardButtons, using flags
	to customize their function and appearance. To enable the Finish button,
	call SetFinishText after the user has taken action on the last page of the
	wizard. 
*/
class COptionSheet : public CDialog
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_DYNAMIC(COptionSheet);
#endif

public:

	/*!
		\brief	Abstract interface describing a list construct

		The COptionList class is an interface used to provide a layer of abstraction
		between the COptionSheet and the control is uses for displaying the list
		of pages in non-wizard mode.
	*/
	class COptionList {
	public:
		/*!
			\brief Default Constructor
		*/
		COptionList();

		/*!
			\brief Destructor
		*/
		virtual ~COptionList();

		/*!
			\brief	Create the control [window]

			The create method is called by COptionSheet in order to create
			the child list control.

			This method MUST be implemented by the derived control class.

			\param	pParentWnd	The parent window of the control. Cannot be NULL.
			\param	nID			The control ID.

			\return			The state of the operation.
			\retval	TRUE	The control was created.
			\retval FALSE	Creation of the control failed.
		*/
		virtual BOOL Create(COptionSheet *pParentWnd, UINT nID) = 0;

		/*!
			\brief Adds a group page to the control

			This member is called by COptionSheet to add a group to the list.

			This method MUST be implemented by the derived control class.

			\param	pPage	Pointer to a COptionPage object used as the group.

			\return			The state of the operation.
			\retval	TRUE	The page was added as a group.
			\retval FALSE	The page was not added as a group.
		*/
		virtual BOOL AddGroup(COptionPage *pPage) = 0;

		/*!
			\brief Adds a page to the control

			This member is called by COptionSheet to add a page to the list.

			This method MUST be implemented by the derived control class.

			\param	pPage		Pointer to a COptionPage object to add.
			\param	pGroupPage	Pointer to the group this page belongs to.
								If NULL, the page is added at the root level.

			\return			The state of the operation.
			\retval	TRUE	The page was added as a group.
			\retval FALSE	The page was not added as a group.
		*/
		virtual BOOL AddPage(COptionPage *pPage, COptionPage *pGroupPage) = 0;

		/*!
			\brief Selects a page as being active/

			This member is called by COptionSheet to show that the supplied
			page is active.

			This method MUST be implemented by the derived control class.

			\param	pPage		Pointer to a COptionPage object to show as active.

			\return			The state of the operation.
			\retval	TRUE	The page was added as a group.
			\retval FALSE	The page was not added as a group.
		*/
		virtual BOOL SetSelection(COptionPage *page) = 0;

		/*!
			\brief	Retrieves the active (highlighted) page 

			Retrieves the active (highlighted) page. If no page is selected,
			this method returns NULL.

			This method MUST be implemented by the derived control class.

			\return		Pointer to the active page or NULL if no page is active.
		*/
		virtual COptionPage *GetSelection() = 0;

		/*!
			\brief	Retrieves the CWnd derived object used for the control.

			This method MUST be implemented by the derived control class.

			\return	A pointer to the CWnd object of the control. This cannot be NULL.
		*/
		virtual CWnd *GetControlWnd() = 0;

	protected:
		/*!
			\brief	Sends a notification message to the parent window.

			This method sends a notification message to the parent window to
			indicate that the selection has changed. This method should be called
			by the derived class anytime the selection has changed.

			If the parent returns FALSE, the selection change should be aborted.

			\return	State of the change in the parent window.
			\retval	TRUE	The parent window changed the active page.
			\retval	FALSE	The parent window did not change the active page.
		*/
		BOOL NotifyParentOfSelChange();

		/*!
			\brief	Sends a notification message to the parent window.

			This method sends a notification message to the parent window to
			indicate that the selection is about to change. This method should
			be called by the derived class prioer to a selection change.

			If the parent returns FALSE, the selection change should be aborted.

			\return	State of the change in the parent window.
			\retval	TRUE	The parent window can change the active page.
			\retval	FALSE	The parent window cannot change the active page.
		*/
		BOOL NotifyParentOfSelChanging();

	};


	/*!
		Option Sheet initializtion parameters. 
		
	*/
	struct OPTIONSHEETCFG {
		DWORD	dwFlags;	//!<	Option sheet initialization flags
		HICON	hIcon;		//!<	Handle to the window icon (optional);
	};

private:
	void		*m_DlgTemplate;		//!<	Pointer to the dialog template
	int			m_StartPageIndex;	//!<	Index of the starting page
	int			m_PageIndex;		//!<	Index of the currently active option page
	DWORD		m_WizButFlags;		//!<	Button flags used in wizard mode
	BOOL		m_Modified;			//!<	TRUE if a page is modified or FALSE if no pages have been modified
	int			m_ButtonYLocOffset;	//!<	Y location offset from the bottom of the page for button placement
	int			m_ButtonWidth;		//!<	Width of a button
	int			m_ButtonHeight;		//!<	Height of a button
	int			m_AllButtonsWidth;	//!<	Width of all buttons and spacing
	int			m_YBorder;			//!<	Horizontal Padding used between items
	int			m_XBorder;			//!<	Vertical Padding used between items
	int			m_ButtonSpacing;	//!<	Spacing between buttons
	CSize		m_MaxChildSize;		//!<	Maximum size of an option page

	CFont		m_Font;				//!<	Font used in buttons
	COptionPage	*m_CurrentPage;		//!<	Pointer to the currently active option page

	int			m_ListWidth;		//!<	Width of the control containing a list of the option pages
	COptionList	*m_ListWindow;		//!<	Control containing the list of option pages

	CString		m_strCaption;		//!<	Caption used on the option sheet dialog

	struct PageInfo {
		COptionPage	*m_Page;		//!<	The page (NULL if this is a group).
		COptionPage	*m_GroupPage;	//!<	The group this page belongs to.
		BOOL		m_IsGroup;		//!<	TRUE if this is a group or FALSE if a page.
	};

protected:
	CArray<PageInfo*, PageInfo*>	m_Pages;	//!<	Array of option pages
	CList<CButton*, CButton*>		m_Buttons;	//!<	List of buttons used by the page

private:
	/*!
		\brief Message handler for selection changed notifications.

		This method is called when an OSN_SELCHANGE notification message
		is recieved from the child list control.

		\param	pNMHDR	Pointer to a notification message object.
		\param	pResult	On return, contains the result of the operation.
	*/
	virtual void ListSelectionChanged(NMHDR *pNMHDR, LRESULT *pResult);

	/*!
		\brief Message handler for selection changing notifications.

		This method is called when an OSN_SELCHANGING notification message
		is recieved from the child list control.

		\param	pNMHDR	Pointer to a notification message object.
		\param	pResult	On return, contains the result of the operation.
	*/
	virtual void ListSelectionChanging(NMHDR *pNMHDR, LRESULT *pResult);

// Construction
private:
	/*!
		\brief	Common functionality to construction of the options sheet.

		Common functionality to construction of the options sheet.

		\param	pParentWnd	Parent window.
		\param	iSelectPage	Select page when the option sheet is first displayed.
	*/
	void CommonConstruct(CWnd *pParentWnd, UINT iSelectPage);

public:
	/*!
		\brief	The OPTIONSHEETCFG structure.

		The OPTIONSHEETCFG structure. Provides access to basic option sheet
		parameters. Change elements of this structure prior to calling DoModal()
		or Create() in order to change the operation parameters used to
		initialize and run an option sheet.
	*/
	OPTIONSHEETCFG	m_psh;

	/*!
		\brief	Construct a COptionSheet object

		Use this member function to construct a COptionSheet object. To
		display the option sheet, call DoModal. 
	*/
	COptionSheet();


	/*!
		\brief	Construct a COptionSheet object

		\param	nIDCaption	ID of the caption to be used for the option sheet.
		\param	pParentWnd	Points to the parent or owner window object (of
							type CWnd) to which the dialog object belongs.
							If it is NULL, the dialog object’s parent window
							is set to the main application window.
		\param	iSelectPage	The index of the page that will initially be on
							top. Default is the first page added to the sheet.

		Use this member function to construct a COptionSheet object. To
		display the option sheet, call DoModal. 
	*/
	COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	/**
		\brief	Construct a COptionSheet object

		\param	pszCaption	Points to a string containing the caption to be
							used for the option sheet. Cannot be NULL.
		\param	pParentWnd	Points to the parent or owner window object (of
							type CWnd) to which the dialog object belongs.
							If it is NULL, the dialog object’s parent window
							is set to the main application window.
		\param	iSelectPage	The index of the page that will initially be on
							top. Default is the first page added to the sheet.

		Use this member function to construct a COptionSheet object. To
		display the option sheet, call DoModal. 
	*/
	COptionSheet(UINT nIDTemplate, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);


	/*!
		\brief	Construct a modeless COptionSheet object

		\param	nIDCaption	ID of the caption to be used for the option
							sheet.
		\param	pParentWnd	Pointer to the parent window of the option
							sheet. If NULL, the parent window will be the
							main window of the application.
		\param	iSelectPage	The index of the page that will initially be on
							top. Default is the first page added to the sheet.

		Call this member function to construct a COptionSheet object. Call
		this member function if one of the class constructors has not already
		been called. For example, call Construct when you declare or allocate
		arrays of COptionSheet objects. In the case of  arrays, you must call
		Construct for each member in the array.

		To display the option sheet, call DoModal or Create. The string
		contained in the first parameter will be placed in the caption bar
		for the option sheet.

		Example:

		The following example demonstrates under what circumstances you would
		call Construct.

<code><pre>
	int i;
	COptionSheet   grpropsheet[4];
	COptionSheet   someSheet;   // no need to call Construct for this one

	UINT rgID[4] = {IDD_SHEET1, IDD_SHEET2, IDD_SHEET3, IDD_SHEET4};

	for (i = 0; i < 4; i++) {
		grpropsheet[i].Construct(rgID[i]);
	}
</pre></code>
	*/
	void Construct(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);


	/*!
		\brief	Construct a modeless COptionSheet object

		\param	pszCaption	Pointer to a string containing the caption to be
							used for the option sheet. Cannot be NULL.
		\param	pParentWnd	Pointer to the parent window of the option
							sheet. If NULL, the parent window will be the
							main window of the application.
		\param	iSelectPage	The index of the page that will initially be on
							top. Default is the first page added to the sheet.

		Call this member function to construct a COptionSheet object. Call
		this member function if one of the class constructors has not already
		been called. For example, call Construct when you declare or allocate
		arrays of COptionSheet objects. In the case of  arrays, you must call
		Construct for each member in the array.

		To display the option sheet, call DoModal or Create. The string
		contained in the first parameter will be placed in the caption bar
		for the option sheet.

		Example:

		The following example demonstrates under what circumstances you would
		call Construct.

<code><pre>
	int i;
	COptionSheet   grpropsheet[4];
	COptionSheet   someSheet;   // no need to call Construct for this one

	UINT rgID[4] = {IDD_SHEET1, IDD_SHEET2, IDD_SHEET3, IDD_SHEET4};

	for (i = 0; i < 4; i++) {
		grpropsheet[i].Construct(rgID[i]);
	}
</pre></code>
	*/
	void Construct(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	/*!
		\brief	COptionSheet constructor
	*/
	virtual ~COptionSheet();

	/////////// ATTRIBUTES ///////////

	/*!

		\brief	Retrieves the index of the active page of the option sheet.

		Call this member function to get the index number of the option sheet
		window’s active page, then use the returned index number as the parameter
		for GetPage. 

		\return	The index number of the active page.
	*/
	int GetActiveIndex() const;


	/*!
		\brief	Retrieves the index of the specified page of the option sheet.

		Use this member function to retrieve the index number of the specified
		page in the option sheet. For example, you would use GetPageIndex to
		get the page index in order to use SetActivePage or GetPage.

		\param	pPage	Points to the page with the index to be found. Cannot be NULL.

		\return	The index number of a page.

	*/
	int GetPageIndex(COptionPage *page) const;
	

	/*!
		\brief	Retrieves the number of pages in the option sheet.
		
		Call this member function to determine the number of pages currently
		in the option sheet.

		\return	The number of pages in the option sheet.
	*/
	int GetPageCount();
	
	
	/*!
		\brief	Retrieves a pointer to the specified page.

		\param	nPage	Index of the desired page, starting at 0. Must be
						between 0 and one less than the number of pages in the
						option sheet, inclusive.

		This member function returns a pointer to the specified page in this option sheet.

		\return	The pointer to the page corresponding to the nPage parameter.
	*/
	COptionPage *GetPage(int nPage) const;

	/*!
		\brief	Retrieves a pointer to the group that owns the specified pages.

		\param	nPage	Index of the desired page, starting at 0. Must be
						between 0 and one less than the number of pages in the
						option sheet, inclusive.

		\return	The pointer to the group corresponding to the "nPage" page parameter.
	*/
	COptionPage *GetPageGroup(int nPage) const;
	

	/*!
		\brief	Returns the active page object.

		Call this member function to retrieve the option sheet window’s
		active page. Use this member function to perform some action on the
		active page.

		\return	The pointer to the active page.
	*/
	COptionPage* GetActivePage() const;
	
	
	/*!
		\brief	Programmatically sets the active page object.

		\param	pPage	Points to the page to set in the option sheet. It
						cannot be NULL.

		\return	Nonzero if the option sheet is activated successfully;
				otherwise 0.
	*/
	BOOL SetActivePage(COptionPage *page);
	
	
	/*!
		\brief	Programmatically sets the active page object.

		\param	nPage	Index of the page to set. It must be between 0 and one
						less than the number of pages in the option sheet,
						inclusive.

		\return	Nonzero if the option sheet is activated successfully;
				otherwise 0.
	*/
	BOOL SetActivePage(int nPage);


	/*!
		\brief	Call this member function to specify the option sheet's caption.

		\param	lpszText	Points to the text to be used as the caption in the
							title bar of the option sheet.

		\param	nStyle		Specifies the style of the option sheet title.
							The style must be specified at 0 or as OSH_PROPTITLE.
							If the style is set as OSH_PROPTITLE, the words
							"Options for" appear before the text specified as
							the caption.

		Call this member function to specify the option sheet's caption (the
		text displayed in the title bar of a frame window). 

		By default, an option sheet uses the caption in the option sheet's
		dialog resource.
	*/
	void SetTitle( LPCTSTR lpszText, UINT nStyle = 0 );

	/*!
		\brief	Sets the caption of the option sheet.

		\param	nID			Resource ID of the caption string.

		\param	nStyle		Specifies the style of the option sheet title.
							The style must be specified at 0 or as OSH_PROPTITLE.
							If the style is set as OSH_PROPTITLE, the words
							"Options for" appear before the text specified as
							the caption.

		Call this member function to specify the option sheet's caption (the
		text displayed in the title bar of a frame window). 

		By default, an option sheet uses the caption in the option sheet's
		dialog resource.
	*/
	void SetTitle(UINT nID, UINT nStyle = 0);

	
	/*!
		\brief	Sets the text for the Finish button.

		Call this member function to set the text in the Finish command button.
		Call SetFinishText to display the text on the Finish command button and
		hide the Next and Back buttons after the user completes action on the
		last page of the wizard.

		\param	lpszText	Points to the text to be displayed on the Finish
							command button.
	*/
	void SetFinishText(LPCSTR lpszText);

	
	/*!
		\brief	Enables the wizard buttons.

		\param	dwFlags	A set of flags that customize the function and
				appearance of the wizard buttons. This parameter can be
				a combination of the following values:
				<br><ul>
					<li><b>OSWIZB_BACK</b> - Back button</li>
					<li><b>OSWIZB_NEXT</b> - Next button</li>
					<li><b>OSWIZB_FINISH</b> - Finish button</li>
					<li><b>OSWIZB_DISABLEDFINISH</b> - Disabled Finish button</li>
				</ul><br>

		Call this member function to enable or disable the Back, Next, or
		Finish button in a wizard option sheet. Call SetWizardButtons only
		after the dialog is open; you can’t call SetWizardButtons before you
		call DoModal. Typically, you should call SetWizardButtons from
		COptionPage::OnSetActive.

		If you want to change the text on the Finish button or hide the Next
		and Back buttons once the user has completed the wizard, call
		SetFinishText. Note that the same button is shared for Finish and Next.
		You can display a Finish or a Next button at one time, but not both.
	*/
	void SetWizardButtons(DWORD dwFlags);


	/*!
		\brief	Enables the wizard mode.

		Call this member function to establish a option page as a wizard. A
		key characteristic of a wizard option page is that the user navigates
		using Next or Finish, Back, and Cancel buttons instead of tabs. 

		Call SetWizardMode before calling DoModal. After you call SetWizardMode,
		DoModal will return either ID_WIZFINISH (if the user closes with the
		Finish button) or IDCANCEL.
	*/
	void	SetWizardMode(BOOL bFullPage = FALSE);


	/*!
		\brief	Checks if any of the option pages have been modified.

		\retval	TRUE	An option page has been modified.
		\retval	FALSE	No option pages have been modified.
	*/
	BOOL	IsModified();


	/*!
		\brief	Call this member function to enable or disable the Apply Now button.

		\param	bChanged	TRUE to indicate that the option page settings
							have been modified since the last time they were
							applied; FALSE to indicate that the option page
							settings have been applied, or should be ignored.

		Call this member function to enable or disable the Apply Now button,
		based on whether the settings in the option pages should be applied
		to the appropriate external object. 
	*/
	void	SetModified(BOOL bModified = TRUE);


	/*!
		\brief	Displays a modal option sheet.

		Call this member function to display a modal option sheet. The return
		value corresponds to the ID of the control that closed the option
		sheet. After this function returns, the windows corresponding to the
		option sheet and all the pages will have been destroyed. The objects
		themselves will still exist. Typically, you will retrieve data from the
		COptionPage objects after DoModal returns IDOK.

		To display a modeless option sheet, call Create instead.

		<b>Note:</b> The first time an option page is created from its
		corresponding dialog resource, it may cause a first-chance exception.
		This is a result of the option page changing the style of the dialog
		resource to the required style prior to creating the page. Because
		resources are generally read-only, this causes an exception. The exception
		is handled by the system, and a copy of the modified resource is made
		automatically by the system. The first-chance exception can thus be
		ignored.

		Since this exception must be handled by the operating system, do not
		wrap calls to COptionSheet::DoModal with a C++ try/catch block in
		which the catch handles all exceptions, for example, catch (...). This
		will handle the exception intended for the operating system, causing
		unpredictable behavior. Using C++ exception handling with specific
		exception types or using structured exception handling where the Access
		Violation exception is passed through to the operating system is safe,
		however.


		\return	IDOK or IDCANCEL if the function was successful; otherwise 0
				or -1. If the option sheet has been established as a wizard
				(see SetWizardMode), DoModal returns either ID_WIZFINISH or
				IDCANCEL

		\retval	IDOK			Function was successful.
		\retval	ID_WIZFINISH	If the option sheet has been established as
								a wizard (see SetWizardMode), DoModal returns
								ID_WIZFINISH if the function was successful.
		\retval	IDCANCEL		Function was successful, but the user canceled
								the operation.
		\retval	0				Function failed.
		\retval -1				Function failed.
	*/
	virtual int DoModal();


	/*!
		\brief	Displays a modal option sheet.

		Call this member function to display a modal option sheet. The return
		value corresponds to the ID of the control that closed the option
		sheet. After this function returns, the windows corresponding to the
		option sheet and all the pages will have been destroyed. The objects
		themselves will still exist. Typically, you will retrieve data from the
		COptionPage objects after DoModal returns IDOK.

		To display a modeless option sheet, call Create instead.

		<b>Note:</b> The first time a option page is created from its
		corresponding dialog resource, it may cause a first-chance exception.
		This is a result of the option page changing the style of the dialog
		resource to the required style prior to creating the page. Because
		resources are generally read-only, this causes an exception. The
		exception is handled by the system, and a copy of the modified resource
		is made automatically by the system. The first-chance exception can
		thus be ignored.

		Since this exception must be handled by the operating system, do not
		wrap calls to COptionSheet::DoModal with a C++ try/catch block in
		which the catch handles all exceptions, for example, catch (...). This
		will handle the exception intended for the operating system, causing
		unpredictable behavior. Using C++ exception handling with specific
		exception types or using structured 


		\retval	IDOK			Function was successful.
		\retval	ID_WIZFINISH	If the option sheet has been established as
								a wizard (see SetWizardMode), DoModal returns
								ID_WIZFINISH if the function was successful.
		\retval	IDCANCEL		Function was successful, but the user canceled
								the operation.
		\retval	0				Function failed.
		\retval -1				Function failed.
	*/


	/*!
		\brief	Displays a modeless option sheet.

		\bug	Currently this mode does not work correctly

		\param	pParentWnd	Points to parent window. If NULL, parent is the
							desktop.
		\param	dwStyle		Window styles for option sheet. For a complete
							list of available styles, see Window Styles.
		\param	dwExStyle	Extended window styles for option sheet. For a
							complete list of available styles, see Extended
							Window Styles

		Call this member function to display a modeless option sheet. The
		call to Create can be inside the constructor, or you can call it after
		the constructor is invoked. 

		The default style, expressed by passing -1 as dwStyle, is actually
		<b>WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXT_HELP | WS_VISIBLE</b>.
		The default extended window style, expressed by passing 0 as dwExStyle,
		is actually <b>WS_EX_DLGMODALFRAME</b>.

		The Create member function returns immediately after creating the
		option sheet. To destroy the option sheet, call
		CWnd::DestroyWindow.

		Modeless option sheets displayed with a call to Create do not have
		OK, Cancel, Apply Now, and Help buttons as modal option sheets do.
		Desired buttons must be created by the user.

		To display a modal option sheet, call DoModal instead.

		\return	Nonzero if the option sheet is created successfully;
				otherwise 0.
	*/
	BOOL Create( CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0 );


	/*!
		\brief	Adds a page to the option sheet.
		
		\param	pPage	Points to the page to be added to the option sheet.
						Cannot be <b>NULL</b>.
		\param	pParent	Points to the group parent.

		This member function adds the supplied page to the end of the page
		list. In non-wizard mode, the page name is added to the end of the
		list in the tree control or if a parent name is supplied the page
		is added in the groups sub-list.

		<b>AddPage</b> adds the COptionPage object to the COptionSheet object’s
		list of pages but does not actually create the window for the page. The
		framework postpones creation of the window for the page until the user
		selects that page.

		When you add an option page using <b>AddPage</b>, the <b>COptionSheet</b>
		is the parent of the <b>COptionPage</b>. To gain access to the option
		sheet from the option page, call <i>CWnd::GetParent.</i>

		It is not necessary to wait until creation of the option sheet window
		to call <b>AddPage</b>. Typically, you will call <b>AddPage</b> before
		calling DoModal or Create.

		If you call <b>AddPage</b> after displaying the option page, the child
		list control will reflect the newly added page.
	*/

	void AddPage(COptionPage *pPage, COptionPage *pGroupPage = NULL);


	/*!
		\brief	Adds a group to the option sheet.
		
		\param	pPage	Points to the group to be added to the option sheet.
						Cannot be <b>NULL</b>.

		This member function adds the supplied page to the page list as a as a
		group. In non-wizard mode, the group's name is added to the end of the
		list in the child list control..

		<b>AddGroup</b> adds the COptionPage object to the COptionSheet object’s
		list of groups/pages but does not actually create the window for the
		page. The framework postpones creation of the window for the page until
		the user selects that page.

		It is not necessary to wait until creation of the option sheet window
		to call <b>AddGroup</b>. Typically, you will call <b>AddGroup</b> before
		calling DoModal or Create.

		If you call <b>AddGroup</b> after displaying the option page, the child
		list control will reflect the newly added page.
	*/
	void AddGroup(COptionPage *pGroup);


	/*!
		\brief	Removes a page from the option sheet.

		\param	nPage	Index of the page to be removed. Must be between 0 and
						one less than the number of pages in the option
						sheet, inclusive.

		This member function removes a page from the option sheet and
		destroys the associated window. The COptionPage object itself
		is not destroyed until the owner of the COptionSheet window is
		closed.
	*/
	void RemovePage(int nPage);
	
	/*!
		\brief	Removes a page from the option sheet.

		\param	pPage	Points to the page to be removed from the option
						sheet. Cannot be NULL.

		This member function removes a page from the option sheet and
		destroys the associated window. The COptionPage object itself
		is not destroyed until the owner of the COptionSheet window is
		closed.
	*/
	void RemovePage(COptionPage *pPage);

	/*!
		\brief	Simulates the choice of the specified button in a option sheet.

		\param	nButton	Identifies the button to be pressed. This parameter
						can be one of the following values:
						<br><ul>
							<li><b>OSBTN_BACK</b></li> - Chooses the Back button.
							<li><b>OSBTN_NEXT</b></li> - Chooses the Next button.
							<li><b>OSBTN_FINISH</b></li> - Chooses the Finish button.
							<li><b>OSBTN_OK</b></li> - Chooses the OK button.
							<li><b>OSBTN_APPLYNOW</b></li> - Chooses the Apply Now button.
							<li><b>OSBTN_CANCEL</b></li> - Chooses the Cancel button.
							<li><b>OSBTN_HELP</b></li> - Chooses the Help button. 
						</ul><br>

		Call this member function to simulate the choice of the specified button
		in a option sheet.

		\return	Nonzero if successful; otherwise zero.
	*/
	BOOL PressButton(int nButton);
	
	/*!
		\brief	Terminates the option sheet.

		\param	nEndID	Identifier to be used as return value of the option
						sheet.

		Use this member function to terminate the option sheet. This member
		function is called by the framework when the OK, Cancel, or Close
		button is pressed. Call this member function if an event occurs that
		should close the option sheet.
	*/
	void EndDialog(int nEndID);

protected:
	/*!
		\brief	Called by the framework when the user chooses the OK button. 
	*/
	virtual void OnOK(void);

	
	/*!
		\brief	Called by the framework when the user chooses the Apply button. 
	*/
	virtual void OnApply(void);

	
	/*!
		\brief	Called by the framework when the user chooses the Cancel button. 
	*/
	virtual void OnCancel(void);

	
	/*!
		\brief	Called by the framework when the user chooses the Next button. 
	*/
	virtual void OnNext(void);

	
	/*!
		\brief	Called by the framework when the user chooses the Back button. 
	*/
	virtual void OnBack(void);

	
	/*!
		\brief	Called by the framework when the user chooses the Finish button. 
	*/
	virtual void OnFinish();

private:

	/*!
		\brief	Creates a dialog template for the COptionSheet dialog window.
	*/
	DLGTEMPLATE* CreateDlgTemplate(DWORD dwStyle = DWORD(-1), DWORD dwExStyle = (DWORD)0);

	/*!
		\brief	Clears the option flags.
		
		\param	flags	The flags to be cleared.
	*/
	void ClearFlags(DWORD flags);

	/*!
		\brief	Sets the option flags.

		This method sets the flags provided in the 'flags' parameter.
		All flags previously set when calling SetFlags() are preserved.

		\param	flags	The flags to be set.
	*/
	void SetFlags(DWORD flags);

	/*!
		\brief	Returns TRUE if in Wizard more, otherwize FALSE.
	*/
	BOOL IsWizardMode();

	/*!
		\brief	Updates all button states.
	*/
	void UpdateAllButtonsState();

	/*!
		\brief	Calculates the area that the child list control will reside in.

		\param	rect	The dimensions of the child list control.
	*/
	void CalcTreeRect(CRect &rect);

	/*!
		\brief	Adds a button to the dialog.

		\param	txt		Text displayed in the button.
		\param	x		the X location of the button.
		\param	w		The width of the button.
		\param	id		The control ID of the button.
		\param	showBut	If TRUE, shows the button when created.
	*/
	void AddButton(char *txt, int x, int w, UINT id, BOOL showBut = TRUE);

	/*!
		\brief	Adds standard buttons to the window.

	*/
	int AddButtons(BOOL addBut = TRUE);

	/*!
		\brief	Builds the list of windows in the child list control.
	*/
	void BuildList();

	/*!
		\brief	Calculates the spacing required for the option sheet dialog.
	*/
	BOOL CalcSpacing(CFont &buttonFont);

	/*!
		\brief	Creates the window for a COptionPsge object.
	*/
	void CreatePage(COptionPage *page);

	/*!
		\brief	Checks to see if the page is a group.
	*/
	BOOL IsPageAGroup(int nPage);


	/*!
		\brief	Retrieves the width of the button specific by 'id'.
	*/
	int GetButtonWidth(UINT id);

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetListWidth(int nWidth);
	void SetListControl(COptionList *listCtrl);
	CWnd *GetListControlWnd();
	// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHEET_H__05EA0917_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_)
