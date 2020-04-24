



#pragma once


class CBitmapDC : public CDC
{
	//public:
	CBitmap bitmap;
	CBitmap *OldBitmap;
	CDC* m_pDC;
	int m_cx,m_cy,x,y;
public:
	CBitmapDC(){};
	CBitmapDC( CDC* pDC ,int cx, int cy, COLORREF bkColor , int x=0, int y=0)
	{
		m_pDC=pDC;
		m_cx=cx,m_cy=cy;
		this->x=x,this->y=y;
		CreateCompatibleDC(pDC);
		if(bitmap.GetSafeHandle()==NULL){
			bitmap.CreateCompatibleBitmap(pDC,cx,cy);
		}
		OldBitmap=SelectObject(&bitmap);
		CBrush brush(bkColor);
		FillRect( CRect(0,0,cx,cy),&brush);
		brush.DeleteObject();
	};
	void Create( CDC* pDC ,int cx, int cy, COLORREF bkColor , int x=0, int y=0)
	{
		m_pDC=pDC;
		m_cx=cx,m_cy=cy;
		this->x=x,this->y=y;
		CreateCompatibleDC(pDC);
		if(bitmap.GetSafeHandle()==NULL){
			bitmap.CreateCompatibleBitmap(pDC,cx,cy);
		}
		OldBitmap=SelectObject(&bitmap);
		CBrush brush(bkColor);
		FillRect( CRect(0,0,cx,cy),&brush);
		brush.DeleteObject();
	};
	void Release(){
		SelectObject(OldBitmap);
		bitmap.DeleteObject();
		DeleteDC();
	};
	CBitmapDC* operator->() 
	{
		return this;
	}	
	
	// Allow usage as a pointer	
	operator CBitmapDC*() 
	{
		return this;
	}
	
	void MemToDC()
	{
		// Copy the offscreen bitmap onto the screen.
		m_pDC->BitBlt(x,y, m_cx,m_cy,
			this, 0, 0, SRCCOPY);		
	}
	~CBitmapDC()	
	{		
		// Copy the offscreen bitmap onto the screen.
		m_pDC->BitBlt(x,y, m_cx,m_cy,
			this, 0, 0, SRCCOPY);			
		
		//Swap back the original bitmap.
		SelectObject(OldBitmap);	
	}
};

// N3SlierFrame

class N3SlierFrame : public CStatic
{
	DECLARE_DYNAMIC(N3SlierFrame)

protected:
	COLORREF m_clrColor;

public:
	N3SlierFrame();
	virtual ~N3SlierFrame();

public:
	void SetColor(COLORREF color);
	COLORREF GetColor(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};



// N3Slider

class N3Slider : public CWnd
{
	DECLARE_DYNAMIC(N3Slider)

// Attributes
public:
	int m_iMin;
	int m_iMax;
	int m_iPos;
	COLORREF m_clrFrom;
	COLORREF m_clrTo;

public:
	N3Slider();
	virtual ~N3Slider();

public:
	
	void SetRange(int iMin, int iMax);
	void SetColors(COLORREF From, COLORREF To);
	int GetRangeMin() { return m_iMin; };
	int GetRangeMax() { return m_iMax; };
	int GetPos()	  { return m_iPos; };
	int GetRangeSize(){ return GetRangeMax()-GetRangeMin(); };	
	void SetPos(int nPos);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

