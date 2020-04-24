#pragma once

#include <afxcmn.h>

enum FTD_FLAG
{
	FTD_FLAG_INVALID	= 0x00000000,

	FTD_FLAG_LV0		= 0x00000001,
	FTD_FLAG_LV1		= 0x00000002,
	FTD_FLAG_LV2		= 0x00000003,
	FTD_FLAG_LV3		= 0x00000004,
	FTD_FLAG_DIRECTROY	= 0x0FFFFFF0,
};


class CFileTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFileTreeCtrl)

public:
	CFileTreeCtrl();
	virtual ~CFileTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// UserCtrl
	void			Init();


	HTREEITEM		InsertDirectory(const char* pcPath, HTREEITEM hParent = NULL);

	CString			GetPathByItem(HTREEITEM hItem);
	HTREEITEM		GetItemByPath(CString strPath);

	void			SetItemLevel(HTREEITEM hItem, unsigned int uiLevel);
	unsigned int	GetItemLevel(HTREEITEM hItem);
	void			AllItems(HTREEITEM hItem, void (*_CALLBACK)(HTREEITEM,CTreeCtrl*,void*), void* pUserData);

protected:
	CImageList m_imgIcon;
};