#ifndef AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
#define AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_

// DirTreeCtrl.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CDirTreeCtrl 

class CDirTreeCtrl : public CTreeCtrl
{
// Konstruktion
public:
	CDirTreeCtrl();

// Attribute
public:

// Operationen
public:

// ?erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	//{{AFX_VIRTUAL(CDirTreeCtrl)
	//}}AFX_VIRTUAL

// Implementierung
public:
	BOOL SetSelPath( LPCTSTR strPath );
	CString GetFullPath( HTREEITEM hItem );
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL DisplayTree( LPCTSTR strRoot, BOOL bFiles = FALSE );
	virtual ~CDirTreeCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	BOOL IsValidPath( LPCTSTR strPath );
	void ExpandItem( HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText );
	BOOL FindSubDir( LPCTSTR strPath );
	HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strPath );
	void DisplayPath( HTREEITEM hParent, LPCTSTR strPath );
	BOOL DisplayDrives();
	BOOL m_bFiles;
	CString m_strError;
	BOOL GetSysImgList();
	CImageList m_imgList;
	CString m_strRoot;
	//{{AFX_MSG(CDirTreeCtrl)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f?t zus?zliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
