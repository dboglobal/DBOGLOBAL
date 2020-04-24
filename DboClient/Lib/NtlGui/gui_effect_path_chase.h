////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CPathChaseEffect
// Desc: Effects that move along a given path
//			 
// 2007.02.01 Peessi@hitel.net   
//
// To Be Desired: 
//	1. I did not think about the whole structure because of lack of time.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_EFFECT_PATH_CHASE_H__
#define __GUI_EFFECT_PATH_CHASE_H__

/*
START_GUI

class CPathParticle;

///////////////////////////////////////////////////////////////////////////////////////////
// CPathInfo

class CPathInfo
{
public:
	CPathInfo( FLOAT fPathTime ) { m_fPathTime = fPathTime; }
	virtual ~CPathInfo(VOID) {}

	virtual VOID Update( CPathParticle* pParticle ) = 0;

	FLOAT	m_fPathTime;
};

class CPathInfo_Pos : public CPathInfo
{
	GUI_DECLEAR_MEMORY_POOL( CPathInfo_Pos, 100 )
public:
//! Constructor & Destructor
	CPathInfo_Pos( CPos& srcCenter, CPos& destCenter, FLOAT fPathTime ) : CPathInfo( fPathTime ) { m_posSrcCenter = srcCenter; m_posDestCenter = destCenter; }
	~CPathInfo_Pos(VOID) {}

//! Operation
	VOID	Update( CPathParticle* pParticle );

public:
//! Variables
	CPos	m_posSrcCenter;
	CPos	m_posDestCenter;
};

class CPathInfo_PosAlpha : public CPathInfo
{
	GUI_DECLEAR_MEMORY_POOL( CPathInfo_PosAlpha, 100 )
public:
	//! Constructor & Destructor
	CPathInfo_PosAlpha( CPos& srcCenter, CPos& destCenter, BYTE ucSrcAlpha, BYTE ucDestAlpha, FLOAT fPathTime ) : CPathInfo( fPathTime ) { m_posSrcCenter = srcCenter; m_posDestCenter = destCenter; m_ucSrcAlpha = ucSrcAlpha; m_ucDestAlpha = ucDestAlpha; }
	~CPathInfo_PosAlpha(VOID) {}

	//! Operation
	VOID	Update( CPathParticle* pParticle );

public:
	//! Variables
	CPos	m_posSrcCenter;
	CPos	m_posDestCenter;
	BYTE	m_ucSrcAlpha;
	BYTE	m_ucDestAlpha;
};

///////////////////////////////////////////////////////////////////////////////////////////
// CPathParticle

class CPathParticle : public CSurface
{
	GUI_DECLEAR_MEMORY_POOL( CPathParticle, 300 )

public:
//! Constructor & Destructor
	CPathParticle( CSurface& surface, std::vector<CPathInfo*>* pvecPath, CPos& posParent );
	~CPathParticle(VOID) {}

//! Attribute
	FLOAT	GetTime(VOID) { return m_fCurrentTime; }
	CPos&	GetPos(VOID) { return m_posParent; }

//! Operation
	VOID	SetParentPos( CPos& posParent ) { m_posParent = posParent; }
	VOID	Reset(VOID);

	BOOL	Update( FLOAT fElapsedTIme, BOOL bRepeat );
	VOID	Render(VOID);

private:
//! Variables
	FLOAT	m_fCurrentTime;			
	INT		m_nPathIndex;					// 현재 Path Index
	
	CPos&	m_posParent;					// Emitter의 스크린좌표.

	std::vector<CPathInfo*>* m_pvecPath;
};

///////////////////////////////////////////////////////////////////////////////////////////
// CPathEffect

class CPathChaseEffect
{
public:
//! Enum	
	typedef struct _stEMITSIZER
	{
		BOOL	bSizer;
		INT		nWidth;
		INT		nHeight;
	}stEMITSIZER;
	
//! Constructor & Destructor
	CPathChaseEffect(VOID);
	~CPathChaseEffect(VOID);
	
//! Attribute
	BOOL	IsWork(VOID) { return m_bRun; }

	VOID	SetSurface( const CSurface& surface ) { m_vecSurface.push_back( surface ); }
	VOID	SetTime( FLOAT fLifeTime, FLOAT fEmitTime, INT nEmitCount ) { m_fLifeTime = fLifeTime; m_fEmitTime = fEmitTime; m_nEmitCount = nEmitCount; }
	
	VOID	SetCenterPos( INT nPosX, INT nPosY );
	VOID	AddPathInfo( CPathInfo* pPathInfo );
	VOID	SetEmitSizer( INT nWidth, INT nHeight );

//! Operation
	VOID	StartProc( BOOL bRepeat = FALSE ) { m_bRun = TRUE; m_bRepeat = bRepeat; Reset(); }
	VOID	EndProc(VOID) { m_bRun = FALSE; ClearParticle(); }

	VOID	Reset(VOID);
	
	VOID	Update( FLOAT fElapsedTime );
	VOID	Render(VOID);

private:
//! Implementation
	VOID	Init(VOID);
	VOID	ClearParticle(VOID);
	VOID	ClearPath(VOID);
	VOID	EmitParticle(VOID);

//! Variables
	std::vector<CSurface>		m_vecSurface;
	std::vector<CPathInfo*>		m_vecPathInfo;
	std::list<CPathParticle*>	m_listParticles;	
	
	stEMITSIZER	m_EmitSizer;

	CPos	m_posCenter;

	FLOAT	m_fLifeTime;
	FLOAT	m_fCurrentTime;
	FLOAT	m_fEmitTime;
	FLOAT	m_fCurrentEmitTime;

	INT		m_nEmitCount;

	BOOL	m_bRun;
	BOOL	m_bRepeat;	
	BOOL	m_bFinishEmit;
};

END_GUI

*/

#endif//__GUI_EFFECT_PATH_CHASE_H__