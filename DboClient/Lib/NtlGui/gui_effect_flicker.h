////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CFlickerEffect
// Desc: Flashing effect over time
//			 
// 2007.02.01 Peessi@hitel.net   
//
// To Be Desired: 
//	1. I did not think about the whole structure because of lack of time.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_EFFECT_FLICKER_H__
#define __GUI_EFFECT_FLICKER_H__

START_GUI

class CFlickerEffect
{
public:
//! Enum
	enum	TYPE_BITFLAG { TYPE_NONE = 0x00, TYPE_ALPHA = 0x01, TYPE_COLOR = 0x02, TYPE_SIZE = 0x04 };
	
//! Constructor & Destructor
	CFlickerEffect(VOID);
	~CFlickerEffect(VOID);

//! Attribute
	BOOL	IsWork(VOID) { return m_bRun; }
	
	VOID	SetCenterPos( INT nPosX, INT nPosY );

	VOID	SetSurface( const CSurface& surface );
	CSurface& GetSurface(VOID) { return m_surSurface; }

	VOID	SetTime( FLOAT fLifeTime, FLOAT fPulseTime ) { m_fLifeTime = fLifeTime;	m_fPulseTime = fPulseTime; }

	VOID	SetAlpha( BYTE ucBeginAlpha, BYTE ucEndAlpha ) { m_ucBeginAlpha = ucBeginAlpha; m_ucEndAlpha = ucEndAlpha; m_dwTypeFlag |= TYPE_ALPHA; }
	VOID	SetColor( DWORD uiBeginColor, DWORD uiEndColor ) { m_uiBeginColor = uiBeginColor; m_uiEndColor = uiEndColor; m_dwTypeFlag |= TYPE_COLOR; }
	VOID	SetSize( INT nBeginWidth, INT nEndWidth, INT nBeginHeight, INT nEndHeight ) { m_nBeginWidth = nBeginWidth; m_nEndWidth = nEndWidth; m_nBeginHeight = nBeginHeight; m_nEndHeight = nEndHeight; m_dwTypeFlag |= TYPE_SIZE; }

	VOID	SetCurrentPulseTime( FLOAT fCurrentPulseTime ) { m_fCurrentPulseTime = fCurrentPulseTime; }
	FLOAT	GetCurrentPulseTime(VOID) { return m_fCurrentPulseTime; }

	VOID	SetBeginState( BOOL bState ) { m_bBeginState = bState; }
	BOOL	GetState(VOID) { return m_bBeginState; }

	VOID	SetRenderTop( bool bRenderTop ) { m_bRenderTop = bRenderTop; }

//! Operation
	VOID	StartProc( BOOL bRepeat = FALSE ) { m_bRun = TRUE; m_bRepeat = bRepeat; Reset(); }	 
	VOID	EndProc(VOID) { m_bRun = FALSE; }

	VOID	Reset(VOID) { m_fCurrentTime = m_fCurrentPulseTime = 0.0f; m_bBeginState = TRUE; }						// 처음부터...
	VOID	ResetAttribute(VOID) { m_dwTypeFlag = 0; m_surSurface.m_SnapShot = m_surSurface.m_Original; }

	VOID	Update( FLOAT fElapsedTime );
	VOID	Render(VOID);

private:
//! Implementation
	VOID	Init(VOID);

	VOID	ApplyAlpha(VOID);
	VOID	ApplyColor(VOID);
	VOID	ApplySize(VOID);

//! Variables
	CSurface	m_surSurface;
	
	FLOAT		m_fLifeTime;			// 0.0f면 무한대
	FLOAT		m_fPulseTime;
	FLOAT		m_fCurrentTime;
	FLOAT		m_fCurrentPulseTime;

	bool		m_bRenderTop;

	BYTE		m_ucBeginAlpha;
	BYTE		m_ucEndAlpha;
	DWORD		m_uiBeginColor;
	DWORD		m_uiEndColor;
	INT			m_nBeginWidth;
	INT			m_nEndWidth;
	INT			m_nBeginHeight;
	INT			m_nEndHeight;
	
	INT			m_nInitPosX;
	INT			m_nInitPosY;
	INT			m_nVaryWidth;
	INT			m_nVaryHeight;

	DWORD		m_dwTypeFlag;

	BOOL		m_bRun;
	BOOL		m_bRepeat;
	BOOL		m_bBeginState;	
};

END_GUI

#endif//__GUI_EFFECT_FLICKER_H__