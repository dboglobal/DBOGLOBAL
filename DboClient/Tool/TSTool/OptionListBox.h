/*!
	\file
*/
#if !defined(AFX_OPTIONLISTBOX_H__ED7E3015_515F_11D6_9C1E_00B0D0452EEC__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_OPTIONLISTBOX_H__ED7E3015_515F_11D6_9C1E_00B0D0452EEC__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionListBox window
#include "OptionSheet.h"

/*!
	\brief	Child list window used by COptionSheet
*/
class COptionListBox : public CListBox, public COptionSheet::COptionList
{
// Construction
public:
	/*!
		\brief Default Constructor
	*/
	COptionListBox();

	/*!
		\brief	Create the control [window]

		The create method is called by COptionSheet in order to create
		the child list control.

		\param	pParentWnd	The parent window of the control. Cannot be NULL.
		\param	nID			The control ID.

		\return			The state of the operation.
		\retval	TRUE	The control was created.
		\retval FALSE	Creation of the control failed.
	*/
	virtual BOOL Create(COptionSheet *pParentWnd, UINT nID);

	/*!
		\brief Adds a group page to the control

		This member is called by COptionSheet to add a group to the list.

		\param	pPage	Pointer to a COptionPage object used as the group.

		\return			The state of the operation.
		\retval	TRUE	The page was added as a group.
		\retval FALSE	The page was not added as a group.
	*/
	virtual BOOL AddGroup(COptionPage *pPage);

	/*!
		\brief Adds a page to the control

		This member is called by COptionSheet to add a page to the list.

		\param	pPage		Pointer to a COptionPage object to add.
		\param	pGroupPage	Pointer to the group this page belongs to.
							If NULL, the page is added at the root level.

		\return			The state of the operation.
		\retval	TRUE	The page was added as a group.
		\retval FALSE	The page was not added as a group.
	*/
	virtual BOOL AddPage(COptionPage *page, COptionPage *pGroupPage);

	/*!
		\brief Selects a page as being active/

		This member is called by COptionSheet to show that the supplied
		page is active.

		\param	pPage		Pointer to a COptionPage object to show as active.

		\return			The state of the operation.
		\retval	TRUE	The page was added as a group.
		\retval FALSE	The page was not added as a group.
	*/
	virtual BOOL SetSelection(COptionPage *page);

	/*!
		\brief	Retrieves the active (highlighted) page 

		Retrieves the active (highlighted) page. If no page is selected,
		this method returns NULL.

		\return		Pointer to the active page or NULL if no page is active.
	*/
	virtual COptionPage *GetSelection();

	/*!
		\brief	Retrieves the CWnd derived object used for the control.

		\return	A pointer to the CWnd object of the control. This cannot be NULL.
	*/
	virtual CWnd *GetControlWnd();

// Attributes
public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionListBox)
	public:
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionListBox)
	afx_msg void OnSelectionChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CImageList	m_ImageList;
	COptionPage* GetItemPage(int nIndex);
	COptionPage* GetItemGroup(int nIndex);
	BOOL AddItem(COptionPage *pPage, COptionPage *pGroup = NULL, int nIndex = -1, BOOL bGroup = FALSE);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONLISTBOX_H__ED7E3015_515F_11D6_9C1E_00B0D0452EEC__INCLUDED_)
