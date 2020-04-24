
#include "StdAfx.h"

#include "KMSelectBox.h"


//@@---------------------------------------------------------------------------
void CKMSelectBox::InitMember()
{
	m_hWnd = NULL;
	m_pDC = NULL;

	//// point, rect
	m_ptCrossCur.x = m_ptCrossCur.y = 0;
	m_ptCrossOld.x = m_ptCrossOld.y = 0;
	
	SetRectEmpty( &m_rcSelectCur );
	SetRectEmpty( &m_rcSelectOld );
	
	m_czClient.cx = m_czClient.cy = 0;
	
	//// pen, brush
	m_crCross   = RGB(0,0,0);
	m_crSelect  = RGB(0,0,0);
	
	m_penCross  = NULL;
	m_penSelect = NULL;
	m_brSelect  = NULL;
	
	// is...
	m_bUseCrossMode = TRUE;
	m_bInBoxMode	= FALSE;
	m_bIsOneClick	= FALSE;
	
}// end of InitMember()


//@@---------------------------------------------------------------------------
void CKMSelectBox::Create( HWND hWnd, BOOL bUseCross )
{
	m_hWnd = hWnd;
	m_pDC = NULL;

	//// point, rect
	m_ptCrossCur.x = m_ptCrossCur.y = 0;
	m_ptCrossOld.x = m_ptCrossOld.y = 0;
	
	SetRectEmpty( &m_rcSelectCur );
	SetRectEmpty( &m_rcSelectOld );
	
	m_czClient.cx = m_czClient.cy = 0;
	
	//// pen, brush
	m_crCross   = RGB(0,0,0);
	m_crSelect  = RGB(0,0,0);
	
	m_penCross	= (HPEN)GetStockObject(WHITE_PEN);
	m_penSelect = (HPEN)GetStockObject(WHITE_PEN);
	m_brSelect  = (HBRUSH)GetStockObject(NULL_BRUSH);
	
	// is...
	m_bUseCrossMode = bUseCross;
	m_bInBoxMode	= FALSE;
	m_bIsOneClick	= FALSE;
	
}// end of Create()


//@@---------------------------------------------------------------------------
void CKMSelectBox::Delete()
{
	//
	InitMember();

}// end of Delete()


//##Cross
//@@---------------------------------------------------------------------------
void CKMSelectBox::DrawXORCross( POINT point )  
{
	if( m_bUseCrossMode == FALSE ) return;

		SelectObject( m_pDC->GetSafeHdc(), m_penCross );
		SetROP2( m_pDC->GetSafeHdc(), R2_XORPEN ) ;                    

		// h
		MoveToEx( m_pDC->GetSafeHdc(), 0,              point.y, NULL);
		LineTo  ( m_pDC->GetSafeHdc(), m_czClient.cx,  point.y);

		// v
		MoveToEx( m_pDC->GetSafeHdc(), point.x, 0,            NULL );
		LineTo  ( m_pDC->GetSafeHdc(), point.x, m_czClient.cy );
	
}// end of DrawXORCross()


//@@---------------------------------------------------------------------------
void CKMSelectBox::CrossDraw( POINT ptMove )  
{
	m_ptCrossCur.x = ptMove.x;
	m_ptCrossCur.y = ptMove.y;
	
	DrawXORCross( m_ptCrossOld );
	DrawXORCross( m_ptCrossCur );

	m_ptCrossOld.x = m_ptCrossCur.x;
	m_ptCrossOld.y = m_ptCrossCur.y;

}// end of CrossDraw()

	
//@@---------------------------------------------------------------------------
void CKMSelectBox::CrossErase()  
{
	DrawXORCross( m_ptCrossOld );
}// end of CrossErase()


//##Box
//@@---------------------------------------------------------------------------
void CKMSelectBox::DrawXORBox( RECT rect )
{
	SelectObject( m_pDC->GetSafeHdc(), m_brSelect );
	SelectObject( m_pDC->GetSafeHdc(), m_penSelect );
	SetROP2( m_pDC->GetSafeHdc(), R2_XORPEN );   
	
	Rectangle( m_pDC->GetSafeHdc(), 
			    rect.left,  rect.top,
				rect.right, rect.bottom );

}// end of DrawXORBox()


//@@---------------------------------------------------------------------------
void CKMSelectBox::BoxDraw ( POINT ptMoveBR )  
{
		// out of Client Area
		if( ptMoveBR.x > m_czClient.cx ) ptMoveBR.x =  m_czClient.cx;
		if( ptMoveBR.y > m_czClient.cy ) ptMoveBR.y =  m_czClient.cy;

		if( ptMoveBR.x < 0 ) ptMoveBR.x = 0;
		if( ptMoveBR.y < 0 ) ptMoveBR.y = 0;
	
	m_rcSelectCur.right  = ptMoveBR.x;
	m_rcSelectCur.bottom = ptMoveBR.y;

	DrawXORBox( m_rcSelectOld );
	DrawXORBox( m_rcSelectCur );

	CopyRect( &m_rcSelectOld, &m_rcSelectCur );

}// end of BoxDraw()


//@@---------------------------------------------------------------------------
void CKMSelectBox::BoxErase()  
{
	DrawXORBox( m_rcSelectOld );
}// end of BoxErase()


//@@---------------------------------------------------------------------------
void CKMSelectBox::NormalizeRect ( RECT& rect )
{
	LONG temp = 0;
	
	if( rect.left > rect.right ) {
		temp		 = rect.right;
		rect.right   = rect.left;
		rect.left	 = temp;
	}
	
	if( rect.top  > rect.bottom ) {
		temp		 = rect.bottom;
		rect.bottom  = rect.top;
		rect.top	 = temp;
	}
	
}// end of NormalizeRect()


//@@---------------------------------------------------------------------------
void CKMSelectBox::UpdateClientSize( int nWidth, int nHeight )
{
	m_czClient.cx = nWidth;
	m_czClient.cy = nHeight;
	
}// end of UpdateClientSize()


//@@---------------------------------------------------------------------------
void CKMSelectBox::Draw( POINT ptCur ) 
{
	if( m_bInBoxMode == FALSE ) 
	{
		CrossDraw( ptCur );
	}
	else 
	{
		BoxDraw( ptCur );
	}

	
}// end of Draw()


//@@---------------------------------------------------------------------------
void CKMSelectBox::Erase( )
{
	CrossErase();
	BoxErase();
}// end of Update()


//@@---------------------------------------------------------------------------
void CKMSelectBox::BoxBegin( POINT ptStart )
{
	if( m_bInBoxMode == TRUE ) return;
	
	CrossErase();
	
	if( m_czClient.cx < ptStart.x )
		m_rcSelectCur.left = m_czClient.cx;
	else
		m_rcSelectCur.left = ptStart.x;
	if( m_czClient.cy < ptStart.y )
		m_rcSelectCur.top = m_czClient.cy;
	else
		m_rcSelectCur.top  = ptStart.y;
		
	m_bInBoxMode = TRUE;
	
	SetCapture( m_hWnd );

}// end of BoxBegin()


//@@---------------------------------------------------------------------------
void CKMSelectBox::BoxEnd()
{
	if( m_bInBoxMode == FALSE ) return;

	CrossErase();
	
	m_bInBoxMode = FALSE;

	ReleaseCapture();
	
	// re-arrange rect structure
	NormalizeRect( m_rcSelectCur ); 

}// end of BoxEnd()


//@@---------------------------------------------------------------------------
// Append by Peessi 
void CKMSelectBox::ClearRects()
{
	m_rcSelectCur.bottom	= 0;
	m_rcSelectCur.right		= 0;
	m_rcSelectCur.left		= 0;
	m_rcSelectCur.top		= 0;

	m_rcSelectOld.bottom	= 0;
	m_rcSelectOld.right		= 0;
	m_rcSelectOld.left		= 0;
	m_rcSelectOld.top		= 0;
}