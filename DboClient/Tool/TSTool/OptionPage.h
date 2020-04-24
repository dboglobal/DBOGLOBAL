/*!
	\file
*/
#if !defined(AFX_OPTIONPAGE_H__05EA0916_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_OPTIONPAGE_H__05EA0916_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_
#endif
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionPage dialog
/*!
	\brief COptionPage

	Objects of class COptionPage represent individual pages of a option
	sheet, otherwise known as a tab dialog box. As with standard dialog boxes,
	you derive a class from COptionPage for each page in your option
	sheet. To use COptionPage-derived objects, first create a COptionSheet
	object, and then create an object for each page that goes in the option
	sheet. Call COptionSheet::AddPage for each page in the sheet, and then
	display the option sheet by calling COptionSheet::DoModal for a modal
	option sheet, or COptionSheet::Create for a modeless option sheet.

	You can create a type of tab dialog box called a wizard, which consists of
	a option sheet with a sequence of option pages that guide the user
	through the steps of an operation, such as setting up a device or creating
	a newsletter. In a wizard-type tab dialog box, the option pages do not
	have tabs, and only one option page is visible at a time. Also, instead
	of having OK and Apply Now buttons, a wizard-type tab dialog box has a
	Back button, a Next or Finish button, and a Cancel button. 
*/
class COptionPage : public CDialog
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_DYNAMIC(COptionPage);
#endif

private:
	CString	m_Caption;	//!<	Caption of the option page

	/*!
		\brief	Constructs a COptionPage object.
	*/
	COptionPage();

	/*!
		\brief	Create() - Private!
	*/
	BOOL Create(LPCTSTR lpszTemplateName,
				CWnd* pParentWnd = NULL);

	/*!
		\brief	Create() - Private!
	*/
	BOOL Create(UINT nIDTemplate,
				CWnd* pParentWnd = NULL);

	/*!
		\brief	CreateIndirect() - Private!
	*/
	BOOL CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate,
					    CWnd* pParentWnd = NULL,
						void* lpDialogInit = NULL);

	/*!
		\brief	CreateIndirect() - Private!
	*/
	BOOL CreateIndirect(HGLOBAL hDialogTemplate,
						CWnd* pParentWnd = NULL);

// Construction
public:

	/*!
		\brief	Retrieves the resource ID of the option page icon.
	*/
	UINT GetIconID();

	/*!
		\brief	Constructs a COptionPage object.

		Constructs a COptionPage object. The object is displayed after all of
		the following conditions are met: 

		<ul>
			<li>The page has been added to a option sheet using
				CPropertySheet::AddPage.
			<li>The option sheet’s DoModal or Create function has been called.
			<li>The user has selected (tabbed to) this page.
		</ul>

		\param	nIDTemplate	ID of the template used for this page.
		\param	nIDCaption	ID of the name to be placed in the tab for this
				page. If 0, the name will be taken from the dialog template
				for this page.
	*/
	COptionPage(UINT nIDTemplate, UINT nIDCaption = 0);

	/*!
		\brief	Constructs a COptionPage object.

		Constructs a COptionPage object. The object is displayed after all of
		the following conditions are met: 

		<ul>
			<li>The page has been added to a option sheet using
				CPropertySheet::AddPage.
			<li>The option sheet’s DoModal or Create function has been called.
			<li>The user has selected (tabbed to) this page.
		</ul>

		\param	lpszTemplateName	Points to a string containing the name 
									of the template for this page. Cannot be NULL.
		\param	nIDCaption			ID of the name to be placed in the tab for
									this page. If 0, the name will be taken from
									the dialog template for this page.
	*/
	COptionPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

	/*!
		\brief	Creates an option page.

		This method is called by COptionSheet in order to create the
		modaless option page component.

		\param	rect		The area to diaplay the option page in.
		\param	pParentWnd	The parent window of the option page.

		\return	The status of the operation.
		\retval	TRUE	The option page was created.
		\retval	FALSE	The option page was not created.
	*/
	BOOL Create(CRect &rect, CWnd *pParentWnd);

	/*!
		\brief	Retrieves the caption of the option page.

		Retrieves the caption of the option page.

		\param	str	Upon return, will contain the caption of the option page.
	*/
	void GetCaption(CString &str);

	/*!

		\brief	Call to activate or deactivate the Apply Now button.

		Call this member function to enable or disable the Apply Now button,
		based on whether the settings in the option page should be applied
		to the appropriate external object. 

		The framework keeps track of which pages are "dirty," that is,
		option pages for which you have called SetModified( TRUE ). The
		Apply Now button will always be enabled if you call SetModified( TRUE )
		for one of the pages. The Apply Now button will be disabled when you
		call SetModified( FALSE ) for one of the pages, but only if none of the
		other pages is "dirty."

		\param	bChanged	TRUE to indicate that the option page settings
							have been modified since the last time they were
							applied; FALSE to indicate that the option page
							settings have been applied, or should be ignored.
	*/
	virtual void SetModified(BOOL bChanged = TRUE);

	/*!
		\brief	Called by the framework when the Apply Now button is clicked.

		This member function is called by the framework when the user chooses
		the OK or the Apply Now button. When the framework calls this function,
		changes made on all option pages in the option sheet are accepted,
		the option sheet retains focus, and OnApply returns TRUE (the value 1).
		Before OnApply can be called by the framework, you must have called
		SetModified and set its parameter to TRUE. This will activate the Apply
		Now button as soon as the user makes a change on the option page.

		Override this member function to specify what action your program takes
		when the user clicks the Apply Now button. When overriding, the function
		should return TRUE to accept changes and FALSE to prevent changes from
		taking effect.

		The default implementation of OnApply calls OnOK.

		For more information about notification messages sent when the user
		presses the Apply Now or OK button in a option sheet, see OSN_APPLY.

		\return	TRUE if the changes are accepted; otherwise FALSE. 
	*/
	virtual BOOL OnApply();

	/*!
		\brief	Called by the framework when the Cancel button is clicked.

		This member function is called by the framework when the user chooses
		the Cancel button. When the framework calls this function, changes to
		all option pages that were made by the user previously choosing the
		Apply Now button are discarded, and the option sheet retains focus. 

		Override this member function to specify what action the program takes
		when the user clicks the Cancel button.

		The default implementation of OnReset does nothing.
	*/
	virtual void OnReset();

	/*!
		\brief	Called by the framework when the OK, Apply Now, or Close
				button is clicked.

		This member function is called by the framework when the user chooses
		either the OK or Apply Now button, immediately after the framework calls
		OnKillActive. Override this member function to implement additional behavior
		specific to the currently active page when user dismisses the entire
		option sheet.

		The default implementation of this member function marks the page as "clean"
		to reflect that the data was updated in the OnKillActive function.
	*/
	virtual void OnOK();

	/*!
		\brief	Called by the framework when the Cancel button is clicked, and
				before the cancel has taken place.

		This member function is called by the framework when the Cancel button
		is selected. Override this member function to perform Cancel button
		actions. The default negates any changes that have been made.
	*/
	virtual void OnCancel();

	/*!
		\brief	Called by the framework when the page is made the active page.

		This member function is called by the framework when the page is chosen
		by the user and becomes the active page. Override this member function
		to perform tasks when a page is activated. Your override of this member
		function should call the default version before any other processing is
		done.

		The default implementation creates the window for the page, if not
		previously created, and makes it the active page.
	*/
	virtual BOOL OnSetActive();

	/*!
		\brief	Called by the framework when the current page is no longer the
				active page. Perform data validation here.

		This member function is called by the framework when the page is no
		longer the active page. Override this member function to perform
		special data validation tasks.

		The default implementation of this member function copies settings from
		the controls in the option page to the member variables of the option
		page. If the data was not updated successfully due to a dialog data
		validation (DDV) error, the page retains focus.

		After this member function returns successfully, the framework will call
		the page’s OnOK function.
	*/
	virtual BOOL OnKillActive();

	/*!
		\brief	Called by the framework when the Cancel button is clicked, and
				before the cancel has taken place.

		This member function is called by the framework when the user clicks the
		Cancel button and before the cancel action has taken place. 

		Override this member function to specify an action the program takes
		when the user clicks the Cancel button.

		The default implementation of OnQueryCancel returns TRUE.
	*/
	virtual BOOL OnQueryCancel();


	/*!
		\brief	Called by the framework when the Back button is clicked while
				using a wizard-type option sheet.

		This member function is called by the framework when the user clicks
		on the Back button in a wizard.

		Override this member function to specify some action the user must
		take when the Back button is pressed. 

		For more information on how to make a wizard-type option sheet,
		see CPropertySheet::SetWizardMode.

		\retval	0	To automatically advance to the next page.
		\retval	-1	to prevent the page from changing. 
		\retval nId	An identifier of the dialog to be displayed. This is used
					To jump to a page other than the next one.
	*/
	virtual LRESULT OnWizardBack();

	/*!
		\brief	Called by the framework when the Next button is clicked while
				using a wizard-type option sheet.

		\retval	0	To automatically advance to the next page.
		\retval	-1	to prevent the page from changing. 
		\retval nId	An identifier of the dialog to be displayed. This is used
					To jump to a page other than the next one.

		This member function is called by the framework when the user clicks
		on the Next button in a wizard.

		Override this member function to specify some action the user must take
		when the Next button is pressed. 
	*/
	virtual LRESULT OnWizardNext();

	/*!
		\brief	Called by the framework when the Finish button is clicked while
				using a wizard-type option sheet.

		This member function is called by the framework when the user clicks on
		the Finish button in a wizard. When the framework calls this function,
		the option sheet is destroyed when the Finish button is pressed and
		OnWizardFinish returns TRUE ( a nonzero value).

		Override this member function to specify some action the user must take
		when the Finish button is pressed. When overriding this function, return
		FALSE to prevent the option sheet from being destroyed.

		For more information about notification messages sent when the user presses
		the Finish button in a wizard option sheet, see OSN_WIZFINISH.
	*/
	virtual BOOL OnWizardFinish();


	/*!
		\brief	Retrieves the option pages dialog template name
	*/
	LPCSTR GetTemplateName() {
		return m_lpszTemplateName;
	}


	/*!
		\brief	Retrieves the option pages dialog template name
	*/
	UINT GetTemplateID() {
		return (UINT)(m_lpszTemplateName);
	}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
private:


	/*!
		\brief	Common construction code
	*/
	void CommonConstruct(UINT nIDCaption);


// Dialog Data
	//{{AFX_DATA(COptionPage)
	//enum { IDD = IDR_MAINFRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPAGE_H__05EA0916_4E3E_11D6_9C1E_00B0D0452EEC__INCLUDED_)
