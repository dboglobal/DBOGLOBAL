#pragma once


// CDataTreeCtrl

#include <vector>
#include <string>

class CDataTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDataTreeCtrl)

public:
	CImageList				m_ImageList;

	typedef std::vector<std::string>	svdef_String;
	svdef_String		m_svExt;

public:
	CDataTreeCtrl();
	virtual ~CDataTreeCtrl();

	BOOL	InitTreeCtrl();
	void	AddExt(const char* strExt);
	void	SerializeList(CString strPath);
	void	SerializeList(HTREEITEM hParentItem);
	BOOL	GetIconIndex(int& iIcon, int& iIconSel, LPCTSTR strPath);
	BOOL	IsExt(const char* strFileName);

protected:
	DECLARE_MESSAGE_MAP()
public:
};


