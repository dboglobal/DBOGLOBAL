//-----------------------------------------------------------------------------
// Name: class CCoordManager
// Desc: View의 확대 축소시 마우스 포인터 및 Draw될 영역을 조절한다. 
//
//			 
// 2006.01.20 Peessi@hitel.net  
//
//-----------------------------------------------------------------------------
#pragma once

#include <afxwin.h>

class CCoordManager
{
public:
	enum	MARGINRECT { CENTER = 0, LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN };

	CCoordManager(void);
	~CCoordManager(void);

	void	InitMember();	// 모든멤버의 초기화.
	void	Reset();		// 그림이 다시 로드될때.

	void    Create( CScrollView* pView, INT nMargin );// OnCreate에서 한번만 호출한다.
	void	Destroy() { InitMember(); }							 

	CPoint  FixMousePt( CPoint& point );
	CRect	GetSrcRect();
	CRect   GetDestRect();

	FLOAT	GetScaleFactor() { return m_fScaleFactor; }
	FLOAT	GetFixXScaleFactor() { return m_fFixXScaleFactor; }
	FLOAT	GetFixYScaleFactor() { return m_fFixYScaleFactor; }
	INT		GetMargin() { return m_nMargin; }
	CPoint	GetOffset() { return m_ptOffset; }
	CRect	GetViewRect() { return m_rcView; }
    
	void	SetMargin( INT nMargin );							// 마진을 변경할때 호출.
	void	SetMouseDelta( INT nDelta );						// WM_MOUSEWHEEL에서 호출

	void	SetClientRect( INT cx, INT cy );					// WM_SIZE에서 호출.
	void	SetImageRect( CRect& rcImage ) { m_rcImage = rcImage; }	// 그림 로드시 호출.

	void	ResetOffset() { m_ptOffset = CPoint( 0, 0 ); }
	void	SetOffset( CPoint& point );							// 옵셋의 위치를 point로 이동시킨다.
	void	MoveOffset( CPoint& point );						// 옵셋의 위치를 point'만큼' 이동시킨다.
	
	MARGINRECT PtInMargin( CPoint& point );
	void	MarginMove( MARGINRECT eMarginRect );

	void	SetPtToCenter( CPoint& point );

	BOOL	IsScrollNeed();
	BOOL	IsHScrollNeed();
	BOOL	IsVScrollNeed();
	void	FixOffset();

private:
	void	SetViewRect();										// 반드시 마진, Client영역 지정후에 사용.
	void	SetFixScaleFactor();
	
private:
	
	CRect	m_rcImage;				// 로드된 그림의 크기.
	CRect   m_rcView;				// 찍히는 영역의 크기.	( 윈도우 영역 - 여백 )
	CRect	m_rcClient;				// 윈도우 영역의 크기.
	CPoint	m_ptOffset;				// 그림의 출력 옵셋.( 오리지널이미지 기준 )
		
	INT		m_nMargin;				// 여백 ( 윈도우 픽셀 )
	INT		m_nMouseDelta;			
		
	FLOAT	m_fScaleFactor;			// 확대 축소 비율.
	FLOAT	m_fFixXScaleFactor;		// 수정된 X축 비율.
	FLOAT	m_fFixYScaleFactor;		// 수정된 Y축 비율.
	
	CScrollView* m_pView;
};
