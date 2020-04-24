#pragma once


// CColorButton

class CColorButton : public CButton
{
	DECLARE_DYNAMIC(CColorButton)

public:
	COLORREF	m_Color;

public:
	void	SetColor(int r, int g, int b);

public:
	CColorButton();
	virtual ~CColorButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


