// Custom Items.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2006 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


class CCustomItemIcon : public CXTPPropertyGridItem
{
public:
	CCustomItemIcon(CString strCaption, HICON hIcon = 0);

	~CCustomItemIcon(void);

protected:
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);
	virtual void OnInplaceButtonDown();

private:
	HICON m_hIcon;
};

class CCustomItemSpin;

class CCustomItemSpinInplaceButton : public CSpinButtonCtrl
{
	friend class CCustomItemSpin;
	CCustomItemSpin* m_pItem;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
};

class CCustomItemSpin : public CXTPPropertyGridItemNumber
{
	friend class CCustomItemSpinInplaceButton;
public:
	CCustomItemSpin(CString strCaption, int nLower = 0, int nUpper = 100);
    void SetRange(int nLower, int nUpper);

protected:
	virtual void OnDeselect();
	virtual void OnSelect();
	virtual CRect GetValueRect();


private:
	CCustomItemSpinInplaceButton m_wndSpin;
    int m_nLower;
    int m_nUpper;
        
};

//////////////////////////////////////////////////////////////////////////
class CCustomItemSpinDouble;

/// 실수를 사용하기 위한 CustomItemSpinDouble을 구성하는 SpinButton 컨트롤
class CCustomItemSpinInplaceButtonDouble : public CSpinButtonCtrl
{
    friend class CCustomItemSpinDouble;
    CCustomItemSpinDouble* m_pItem;
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT* pResult);
};

/// 실수를 사용하기 위한 CustomItemSpinDouble
class CCustomItemSpinDouble : public CXTPPropertyGridItemDouble
{
    friend class CCustomItemSpinInplaceButtonDouble;
public:
    CCustomItemSpinDouble(CString strCaption, CString strFormat = _T("%.2f"), double fLower = -100.0f, double fUpper = 100.0f);

protected:
    virtual void OnDeselect();
    virtual void OnSelect();
    virtual CRect GetValueRect();

private:
    CCustomItemSpinInplaceButtonDouble m_wndSpin;
    double m_fLower;
    double m_fUpper;
};

//////////////////////////////////////////////////////////////////////////

class CCustomItemEdit : public CXTPPropertyGridItem
{
public:
	CCustomItemEdit(CString strCaption, CString strValue)
		: CXTPPropertyGridItem(strCaption, strValue)
	{
		m_nFlags = xtpGridItemHasComboButton|xtpGridItemHasEdit;
		GetConstraints()->AddConstraint(_T("<Edit...>"));
	}

protected:
	virtual void OnValueChanged(CString strValue) 
	{
		if (strValue == _T("<Edit...>"))
		{
			AfxMessageBox(_T("Show Edit Dialog Box"));
		}
		else
		{
			CXTPPropertyGridItem::OnValueChanged(strValue);

		}
	}

};

class CCustomItemFileBox : public CXTPPropertyGridItem
{
public:    
	CCustomItemFileBox(CString strCaption, CString strFilter = _T("All Files (*.*)|*.*||"));

protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

    CString m_strFilter;
};



class CCustomItemCheckBox;

class CInplaceCheckBox : public CButton
{
public:
	afx_msg LRESULT OnCheck(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

	DECLARE_MESSAGE_MAP()
protected:
	CCustomItemCheckBox* m_pItem;
	COLORREF m_clrBack;
	CBrush m_brBack;

	friend class CCustomItemCheckBox;
};

class CCustomItemCheckBox : public CXTPPropertyGridItem
{
protected:

public:
	CCustomItemCheckBox(CString strCaption);

	BOOL GetBool();
	void SetBool(BOOL bValue);

protected:
	virtual void OnDeselect();
	virtual void OnSelect();
	virtual CRect GetValueRect();
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	virtual BOOL IsValueChanged();




private:
	CInplaceCheckBox m_wndCheckBox;
	BOOL m_bValue;

	friend class CInplaceCheckBox;
};



class CCustomItemChilds : public CXTPPropertyGridItem
{
	class CCustomItemChildsAll;
	class CCustomItemChildsPad;

	friend class CCustomItemChildsAll;
	friend class CCustomItemChildsPad;

public:
	CCustomItemChilds(CString strCaption, CRect rcValue);

protected:
	virtual void OnAddChildItem();
	virtual void SetValue(CString strValue);

private:
	void UpdateChilds();
	CString RectToString(CRect rc);


private:
	CCustomItemChildsAll* m_itemAll;
	CCustomItemChildsPad* m_itemLeft;
	CCustomItemChildsPad* m_itemTop;
	CCustomItemChildsPad* m_itemRight;
	CCustomItemChildsPad* m_itemBottom;

	CRect m_rcValue;
};


// Offset을 위한 Custom Item (x,y,z 세가지 좌표를 가진다.)
class CCustomItemOffset : public CXTPPropertyGridItem
{
    class CCustomItemOffsetChild;

    friend class CCustomItemOffsetChild;

public:
    enum EType      // Offset Type
    {
        TYPE_XYZ,       // x,y,z
        TYPE_XY,        // x,y,
    };

public:
    CCustomItemOffset(CString strCaption, EType eType = TYPE_XYZ);

    virtual void SetOffset(float x, float y, float z = 0.0f);    
    double GetValueX();
    double GetValueY();
    double GetValueZ();

protected:
    virtual void OnAddChildItem();    
    void UpdateChilds();

    CCustomItemOffsetChild* m_pItemX;
    CCustomItemOffsetChild* m_pItemY;
    CCustomItemOffsetChild* m_pItemZ;
    EType                   m_eType;
};

class CCustomItemColor: public CXTPPropertyGridItemColor
{
public:
	CCustomItemColor(CString strCaption,  COLORREF clr = 0);

protected:
	virtual void OnInplaceButtonDown();

#ifndef _DECIMAL_VALUE
public:
	COLORREF StringToRGB(CString str)
	{
		TCHAR *stopstring;
		int nValue = _tcstoul((LPCTSTR)str, &stopstring, 16);
		return RGB(GetBValue(nValue), GetGValue(nValue), GetRValue(nValue));
	}

	CString RGBToString(COLORREF clr)
	{
		CString str;
		str.Format(_T("%2X%2X%2X"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	#if _MSC_VER < 1200 // MFC 5.0
		for (int i = 0; i < str.GetLength(); i++) if (str[i] == _T(' ')) str.SetAt(i, _T('0'));
	#else
		str.Replace(_T(" "), _T("0"));
	#endif
		return str;
	}

	void SetValue(CString strValue)
	{
		SetColor(StringToRGB(strValue));
	}

	void SetColor(COLORREF clr)
	{
		m_clrValue = clr;
		CXTPPropertyGridItem::SetValue(RGBToString(clr));
	}
#endif
};




//////////////////////////////////////////////////////////////////////////


class CInplaceUpperCase : public CXTPPropertyGridInplaceEdit
{
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_DYNAMIC(CInplaceUpperCase);
	DECLARE_MESSAGE_MAP()
};


class CCustomItemUpperCase : public CXTPPropertyGridItem
{
public:
	CCustomItemUpperCase::CCustomItemUpperCase(CString strCaption)
		: CXTPPropertyGridItem(strCaption)
	{
	}

protected:
	virtual CXTPPropertyGridInplaceEdit& GetInplaceEdit()
	{
		return m_wndEdit;
	}

private:
	CInplaceUpperCase m_wndEdit;

};






class CCustomItemSlider;

class CInplaceSlider : public CSliderCtrl
{
public:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
protected:
	CCustomItemSlider* m_pItem;
	COLORREF m_clrBack;
	CBrush m_brBack;
	int m_nValue;

	friend class CCustomItemSlider;
};

class CCustomItemSlider : public CXTPPropertyGridItemNumber
{
protected:

public:
	CCustomItemSlider(CString strCaption);

protected:
	virtual void OnDeselect();
	virtual void OnSelect();

private:
	CInplaceSlider m_wndSlider;
	int m_nWidth;

	friend class CInplaceSlider;
};


class CCustomItemButton;

class CInplaceButton : public CXTButton
{
public:
	DECLARE_MESSAGE_MAP()
protected:
	CCustomItemButton* m_pItem;
	void OnClicked();

	friend class CCustomItemButton;
};

class CCustomItemButton : public CXTPPropertyGridItem
{
protected:

public:
	CCustomItemButton(CString strCaption, BOOL bFullRowButton, BOOL bValue);

	BOOL GetBool();
	void SetBool(BOOL bValue);

protected:
	virtual BOOL IsValueChanged();
	virtual void SetVisible(BOOL bVisible);
	BOOL OnDrawItemValue(CDC& dc, CRect rcValue);
	virtual void OnIndexChanged();
	void CreateButton();


private:
	CInplaceButton m_wndButton;
	BOOL m_bValue;
	CFont m_wndFont;
	CString m_strButtonText;
	BOOL m_bFullRowButton;

	friend class CInplaceButton;
};


