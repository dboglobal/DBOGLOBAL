#pragma once


#include "afxwin.h"
#include "NtlPLWorldDecal.h"
#include "colourpicker.h"


// CPaletteDecalDlg 대화 상자입니다.
class CNtlPLWorldDecal;


class CPaletteDecalDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteDecalDlg)

public:
	CPaletteDecalDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteDecalDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT_DECAL };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	sDECAL_PROP_PARAM	m_DecalPropParam;
	CString				m_StrAlphaValue;
	CString				m_StrTexAng;
	CString				m_StrTexScl;
	CString				m_StrDecalSizeX;
	CString				m_StrDecalSizeZ;
	CString				m_StrVisibilityDist;
	CString				m_UVAniSpeed;
	CString				m_UVAniOffset;
	CListBox			m_DecalTexList;
	CComboBox			m_UVAniPattern;
	CButton				m_UVAniCheck;
	CComboBox			m_DecalMode;
	CColourPicker		m_DecalColor;
	COLORREF			m_clrDiffuser;
	CString				m_DecalColorR;
	CString				m_DecalColorG;
	CString				m_DecalColorB;

public:
	virtual BOOL		OnInitDialog();
	afx_msg void		OnLbnSelchangeDecalListbox();
	afx_msg void		OnBnClickedApply();
	VOID				Refresh();
	VOID				Refresh(CNtlPLWorldDecal* _pEntity);
	void				SetColor(int r, int g, int b);
	sDECAL_PROP_PARAM&	GetDecalParam();
	VOID				InitializeWorldData();
};
