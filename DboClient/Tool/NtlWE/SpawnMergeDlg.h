#pragma once


// CSpawnMergeDlg 대화 상자입니다.

#include "NtlSpawnMergeManager.h"
#include "afxcmn.h"

class CSpawnMergeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpawnMergeDlg)

public:
	CSpawnMergeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpawnMergeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_MERGE };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual	BOOL	PreTranslateMessage(MSG* pMsg);
	virtual BOOL	OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void	OnBnClickedCancel();
	afx_msg void	OnBnClickedOk();

	void			UpdatePathListCtrl(CNtlPathListMergeContainer* pPathListMergeContainer, CListCtrl* pListCtrl, RwBool bUseCheckBox);

	BOOL			TestSpawnMergeReady();
	BOOL			TestSpawnMergeFileExist(CString& strFolderName, CString& strNPCSpawnFileName, CString& strMOBSpawnFileName, CString& strPath4ToolFileName);

	BOOL			GetMergeArea(RwBBox* pbboxMergeDst, RwBBox* pbboxMergeSrc);

protected:
	CString			m_strNPCSpawnFileNameDst;
	CString			m_strMOBSpawnFileNameDst;
	CString			m_strPath4ToolFileNameDst;

	CString			m_strNPCSpawnFileNameSrc;
	CString			m_strMOBSpawnFileNameSrc;
	CString			m_strPath4ToolFileNameSrc;

	CNtlSpawnMergeContainer		m_SpawnMergeContainerNPCDst;
	CNtlSpawnMergeContainer		m_SpawnMergeContainerMOBDst;
	CNtlPathListMergeContainer	m_PathListMergeContainerDst;

	CNtlSpawnMergeContainer		m_SpawnMergeContainerNPCSrc;
	CNtlSpawnMergeContainer		m_SpawnMergeContainerMOBSrc;
	CNtlPathListMergeContainer	m_PathListMergeContainerSrc;

	RwBBox						m_bboxMergeDst;
	RwBBox						m_bboxMergeSrc;
	
	CListCtrl					m_ListCtrlDst;
	CListCtrl					m_ListCtrlSrc;

	RwBool						m_bMergeSpawn;
	RwBool						m_bMergeSpawnPath;
};
