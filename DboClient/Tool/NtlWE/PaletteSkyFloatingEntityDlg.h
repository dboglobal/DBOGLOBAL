#pragma once
#include "afxwin.h"


// CPaletteSkyFloatingEntityDlg 대화 상자입니다.

class CPaletteSkyFloatingEntityDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSkyFloatingEntityDlg)

public:
	CPaletteSkyFloatingEntityDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSkyFloatingEntityDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKY_FLOATING_ENTITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	CButton m_Entity00Create;
	CButton m_Entity01Create;
	CButton m_Entity02Create;
	CString m_Entity00TexName;
	CString m_Entity01TexName;
	CString m_Entity02TexName;
	CString m_Entity00PosX;
	CString m_Entity01PosX;
	CString m_Entity02PosX;
	CString m_Entity00PosY;
	CString m_Entity01PosY;
	CString m_Entity02PosY;
	CString m_Entity00PosZ;
	CString m_Entity01PosZ;
	CString m_Entity02PosZ;
	CString m_Entity00Scale;
	CString m_Entity01Scale;
	CString m_Entity02Scale;
	CButton m_Entity00LensFlare;
	CButton m_Entity01LensFlare;
	CButton m_Entity02LensFlare;

	CButton m_SpecularCreate;
	CButton m_SpecularReflash;
	CString m_SpecularScaleX;
	CString m_SpecularScaleY;
	CString m_SpecularPower;
	CString m_SpecularPosY;

protected:
	afx_msg void OnBnClickedEntity00TextureLoad();
	afx_msg void OnBnClickedEntity01TextureLoad();
	afx_msg void OnBnClickedEntity02TextureLoad();
	afx_msg void OnBnClickedEntity00Create();
	afx_msg void OnBnClickedEntity01Create();
	afx_msg void OnBnClickedEntity02Create();
	afx_msg void OnBnClickedEntity00Lensflare();
	afx_msg void OnBnClickedEntity01Lensflare();
	afx_msg void OnBnClickedEntity02Lensflare();
	afx_msg void OnBnClickedSetprop();
	afx_msg void OnBnClickedGetprop();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedSpecularCreate();
	afx_msg void OnBnClickedSpecularReflash();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	VOID Reset();
	VOID Refresh(RwInt32 iIndex = -1);
	VOID FillData(sNTL_FIELD_PROP* _pNtlFieldProp);
	VOID ApplyData(sNTL_FIELD_PROP* _pNtlFieldProp);
};
