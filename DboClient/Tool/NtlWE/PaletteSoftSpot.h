#pragma once


#include "colourpicker.h"

// CZ-SS
// CPaletteSoftSpot 대화 상자입니다.
// class CNtlPLSpot;
// 
// 
// class CPaletteSoftSpot : public CDialog
// {
// 	DECLARE_DYNAMIC(CPaletteSoftSpot)
// 
// public:
// 	CPaletteSoftSpot(CWnd* pParent = NULL);   // 표준 생성자입니다.
// 	virtual ~CPaletteSoftSpot();
// 
// // 대화 상자 데이터입니다.
// 	enum { IDD = IDD_SOFTSPOT };
// 
// protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
// 
// 	DECLARE_MESSAGE_MAP()
// 
// public:
// 	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 
// public:
// 	VOID InitializeWorldData();
// 
// public:
// 	CString m_SoftAngle;
// 	CString m_SoftRadius;
// 
// public:
// 	CString			m_SoftR;
// 	CString			m_SoftG;
// 	CString			m_SoftB;
// 	COLORREF		m_clrDiffuser;
// 	CColourPicker	m_SoftClrPicker;
// 
// public:
// 	VOID SetColor(int r, int g, int b);
// 	VOID SetCurSoftSpotProp();
// 	VOID RefreshProp(CNtlPLSpot* _pNtlPLSpot);
// 
// protected:
// 	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
// public:
// 	CString m_SoftPosX;
// public:
// 	CString m_SoftPosY;
// public:
// 	CString m_SoftPosZ;
// public:
// 	CString m_SoftRotX;
// public:
// 	CString m_SoftRotY;
// public:
// 	CString m_SoftRotZ;
// public:
// 	afx_msg void OnBnClickedSoftCreateBtn();
// };
