#pragma once


// CEditDialog 대화 상자입니다.

#include "resource.h"

#include "rwcore.h"

class CEditDialog : public CDialog
{
	DECLARE_DYNAMIC(CEditDialog)

public:
	CEditDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditDialog();

public:
	CString m_strOldAbsoluteX;
	CString m_strOldAbsoluteY;
	CString m_strOldAbsoluteZ;

public:
	void	Clear();
	void	SetAbsolute();
	void	SetAbsolute(float x, float y, float z);

	void	AddEditPoint(RwV3d& vOffset);
	void	SetEditPoint(RwV3d& vAbsolute);
	void	SetEditPoint_X(float x);
	void	SetEditPoint_Y(float y);
	void	SetEditPoint_Z(float z);

	void	Reset();
	void	ResetInstanceEffect();
	void	ResetSplineControlPoint();

	static CEditDialog& GetInstance();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDIT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strAbsoluteX;
	CString m_strAbsoluteY;
	CString m_strAbsoluteZ;
	CString m_strOffsetX;
	CString m_strOffsetY;
	CString m_strOffsetZ;
	afx_msg void OnEnChangeAbsoluteX();
	afx_msg void OnEnChangeAbsoluteY();
	afx_msg void OnEnChangeAbsoluteZ();
	afx_msg void OnEnChangeOffsetX();
	afx_msg void OnEnChangeOffsetY();
	afx_msg void OnEnChangeOffsetZ();
	afx_msg void OnEnKillfocusAbsoluteX();
	afx_msg void OnEnKillfocusAbsoluteY();
	afx_msg void OnEnKillfocusAbsoluteZ();
	afx_msg void OnEnKillfocusOffsetX();
	afx_msg void OnEnKillfocusOffsetY();
	afx_msg void OnEnKillfocusOffsetZ();
};
