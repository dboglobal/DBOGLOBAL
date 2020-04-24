#pragma once


// CMeshListCtrl
#include "VenusGridCtrl.h"

class CMeshListCtrl : public CVenusGridCtrl
{
	DECLARE_DYNAMIC(CMeshListCtrl)

public:
	CImageList m_image;//Must provide an imagelist

public:
	CMeshListCtrl();
	virtual ~CMeshListCtrl();

	void	InitializeGrid();

	void	RebuildMeshList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


