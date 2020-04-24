#pragma once


// CMeshListDialog 대화 상자입니다.

#include "DataTreeCtrl.h"

class CMeshListDialog : public CDialog
{
	DECLARE_DYNCREATE(CMeshListDialog)

public:
	CMeshListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshListDialog();

	void	SerializeList();

public:
	BOOL					m_Initialized;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MESH_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnTvnSelchangedMeshList(NMHDR *pNMHDR, LRESULT *pResult);

protected:
    void OnRefresh();                               ///< Mesh List를 다시 읽어 들인다.

protected:
    CDataTreeCtrl m_MeshList;
};
