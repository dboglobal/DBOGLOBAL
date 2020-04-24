#pragma once


// CTextureListCtrl

#include "libgfl.h"
#include "rwcore.h"

class CTextureListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTextureListCtrl)


public:
	enum THUMBNAIL_SIZE
	{
		THUMBNAIL_WIDTH	= 70,
		THUMBNAIL_HEIGHT = 60
	};
	CImageList				m_ImageList;

public:
	CTextureListCtrl();
	virtual ~CTextureListCtrl();

	void	OnInitControl();

	void	BuildTextureList();

	void	InsertTexture(const RwChar* strFileName, const RwChar* strPathName);
	RwBool	InsertTextureImage(RwInt32 index, const RwChar* strPathName);

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//	afx_msg void OnInsertTexture();
//	afx_msg void OnDeleteTexture();
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


