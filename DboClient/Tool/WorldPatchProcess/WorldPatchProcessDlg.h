#pragma once

#include "afxcmn.h"

#include "PerformanceChecker.h"

#define TIMER_PATCH (7)
#define WORLD_PROJECT_STATE_ERROR		(0)
#define WORLD_PROJECT_STATE_RAEDY		(1)
#define WORLD_PROJECT_STATE_FAILED		(2)
#define WORLD_PROJECT_STATE_SUCCEEDED	(3)

#define LIST_SUBITEM_EMPTY				(0)	 // 사용하지 않는다
#define LIST_SUBITEM_STATE				(1)  // 1부터 시작해야 한다.
#define LIST_SUBITEM_WORLD_VERSION		(2)
#define LIST_SUBITEM_WORLD_NAME			(3)
#define LIST_SUBITEM_WORLD_FULLPATH		(4)

class CNtlWPPApplication;

class CWorldPatchProcessDlg : public CDialog
{
public:
	CWorldPatchProcessDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_WORLDPATCHPROCESS_DIALOG };

public:
	void				ProgressWorldProject(int iPos, int iRange);
	void				ProgressWorldFields(int iPos, int iRange);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	virtual BOOL		OnInitDialog();
	afx_msg void		OnPaint();
	afx_msg void		OnDestroy();
	afx_msg HCURSOR		OnQueryDragIcon();
	
	afx_msg void		OnBnClickedOk();
	afx_msg void		OnBnClickedCancel();
	afx_msg void		OnDropFiles(HDROP hDropInfo);

	afx_msg void		OnBnClickedButtonClear();
	afx_msg void		OnBnClickedButtonAdd();
	afx_msg void		OnBnClickedButtonDel();
	afx_msg void		OnBnClickedButtonPatch();

	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	
	DECLARE_MESSAGE_MAP()

	void				CreateThread();
	void				DeleteThread();

	void				InitControlUI();
	void				UpdateControlUI();

	void				GetTimeString(CString& str, double fTime);

	void				SetEnableAll(BOOL bEnable);

	BOOL				AddWorldProject(char* pcPath);

	void				Patch();

	static unsigned int __stdcall PatchThread(void* pParam);

protected:
	HANDLE					m_hThread;
	unsigned int			m_uiThreadID;
	BOOL					m_bThreading;

	HICON					m_hIcon;

	CListCtrl				m_listWorldProject;

	CString					m_strWorldProject;
	CString					m_strWorldFields;
	CString					m_strTime;
	CProgressCtrl			m_progressWorldProject;
	CProgressCtrl			m_progressWorldFields;

	CPerformanceChecker		m_PerformanceChecker;

	CNtlWPPApplication*		m_pNtlWPPApplication;

	// Update Control Data
	std::string				m_strWroldProjectCurrent;

	double					m_fTimeStartPatch;
	double					m_fTimeStartCurrent;

	int						m_iWorldProjectCnt;	
	int						m_iWorldProjectNum;

	int						m_iFieldCntTotal;
	int						m_iFieldNumTotal;
	int						m_iFieldCntCurrent;
	int						m_iFieldNumCurrent;
};

// STDMETHODIMP CWorldPatchProcessDlg::ListCtrlDrop(LPDATAOBJECT pDataObject, DWORD dwKeyState, POINTL pt, LPDWORD pdwEffect)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());  // init MFC
// 
// 	COleDataObject dataobj;
// 	TCHAR          szItem[MAX_PATH];
// 	UINT           uNumFiles;
// 	HGLOBAL        hglobal;
// 	HDROP          hdrop;
// 	dataobj.Attach(pDataObject, FALSE);
// 
// 	// Read the list of items from the data object.  They're stored in HDROP
// 	// form, so just get the HDROP handle and then use the drag 'n' drop APIs
// 	// on it.
// 	hglobal = dataobj.GetGlobalData(CF_HDROP);
// 	if (NULL != hglobal)
// 	{
// 		hdrop = (HDROP)GlobalLock(hglobal);
// 		uNumFiles = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0); //넘어온 파일갯수
// 		for (UINT uFile = 0; uFile < uNumFiles; ++uFile) 
// 		{
// 			DragQueryFile(hdrop, uFile, szItem, MAX_PATH); // 넘어온 파일명 추출
// 		}
// 		GlobalUnlock (hglobal);
// 
// 	}
// 	dataobj.Detach();
// 	/*FORMATETC fmtetc = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
// 	STGMEDIUM stgMedium;
// 
// 
// 	HRESULT hr=pDataObject->GetData(&fmtetc , &stgMedium);
// 
// 	if( FAILED(hr) )
// 	return hr;
// 
// 
// 	char szFileName[MAX_PATH] = {0,};
// 	DragQueryFile( (HDROP)stgMedium.hGlobal, 0, szFileName, sizeof(szFileName));
// 	*/
// 	return S_OK;
// }