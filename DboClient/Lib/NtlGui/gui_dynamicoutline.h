
////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CDynamicOutline
// Desc: Create dynamically resized borders.
//		 Must follow certain rules.
//			 
// 2006.03.6 Peessi@hitel.net   
//
// To Be Desired: 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_DYNAMIC_OUTLINE__
#define __GUI_DYNAMIC_OUTLINE__

START_GUI

class CDynamicOutline
{
public:
//! Constructor & Destructor:
	CDynamicOutline( CComponent* pComponent, BOOL bBalloon );
	~CDynamicOutline(VOID);

//! Defines
	enum SURFACEPART { CENTER = 0, LTC, RTC, LBC, RBC, LL, TL, RL, BL1, BL2, TAIL, NUM_SURFACE };
	
//! Attributes:
	CRectangle GetOutRect() { return m_rcOutRect; }

	INT		GetLeftCX(VOID) { return m_nLeftCX; }
	INT		GetRightCX(VOID) { return m_nRightCX; }
	INT		GetTopCY(VOID) { return m_nTopCY; }
	INT		GetBottomCY(VOID) { return m_nBottomCY; }

	INT		GetLeftCornerCX(VOID); 
	INT		GetRightCornerCX(VOID);
	
	VOID	SetLeftCX( INT nCX ) { m_nLeftCX = nCX; }
	VOID	SetRightCX( INT nCX ) { m_nRightCX = nCX; }
	VOID	SetTopCY( INT nCY ) { m_nTopCY = nCY; }
	VOID	SetBottomCY( INT nCY ) { m_nBottomCY = nCY; }

	INT		GetTailWidth() { return m_cxTail; }
	INT		GetTailHeight() { return m_cyTail; }

//! Operations:
	VOID	SetSurfaces( const CComponentOptions& options );
	VOID	Paint( bool bRenderTop );

	CRectangle	SetInScreenRect( CRectangle& InRect, INT nTailX = 0 );
	VOID		SetOutScreenRect( CRectangle& OutRect, INT nTailX = 0 );
	VOID		SetMove( INT nScreenX, INT nScreenY );

	VOID	AddSurface( SURFACEPART ePart, CSurface& surface );
	VOID	AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
						 CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL, CSurface& surTail );

	VOID	SetSurfaceAlpha( BYTE ucAlpha );
	VOID	SetSurfaceColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
		
private:
//! Variables:
	BOOL			m_bInitialized;
	BOOL			m_bBalloon;
	INT				m_nTailX;
	CRectangle		m_rcOutRect;
		
	CSurface		m_Surfaces[NUM_SURFACE];

	INT				m_nLeftCX;
	INT				m_nRightCX;
	INT				m_nTopCY;
	INT				m_nBottomCY;

	INT				m_cxTail;
	INT				m_cyTail;

	CComponent*		m_pComponent;	
};

END_GUI

#endif//__GUI_DYNAMIC_OUTLINE__