////////////////////////////////////////////////////////////////////////////////
// File: NtlPLCinematicGui.h
// Desc: NtlPLCinematicGui. DTEditor에서의 사용을 위해 Client에서 이주해 옴.
//
// 2007.12.21 Peessi@hitel.net   
//
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_PL_CINEMATIC_GUI_H__
#define __NTL_PL_CINEMATIC_GUI_H__

#include "ceventhandler.h"
#include "gui_precomp.h"
#include "NtlPLCinematicObject.h"

class CNtlPLLuaLinkedFlash : public CNtlPLCinematicFlash
{
public:
	enum STATE { BEGIN, PLAY, END, NONE };

	CNtlPLLuaLinkedFlash( CRectangle& rtRect, RwChar* pFilename, gui::CComponent* pParent, VOID* pEchoData, RwBool bUseTime, 
						  RwReal fLifeTime, RwReal fBeginAlphaTime = 0.0f, RwReal fEndAlphaTime = 0.0f, RwUInt32 flagPlayTimeType = 0 );
	~CNtlPLLuaLinkedFlash(VOID);

	RwBool Update( RwReal fElapsedTime );

private:
	VOID BeginStateUpdate( RwReal fElapsedTime );
	VOID PlayStateUpdate( RwReal fElapsedTime );
	VOID EndStateUpdate( RwReal fElapsedTime );		

	VOID OnEndMovie( gui::CComponent* pComponent );

	//gui::CSlot	m_slotMovieEnd;

	RwBool	m_bUseTime;
	RwReal	m_fLifeTime;				// m_bUseTime : True일때 유효
	RwReal	m_fFlashMovieTime;			// m_bUseTime : False일때 유효
	
	RwReal	m_fBeginAlphaTime;
	RwReal  m_fPlayTime;				
	RwReal	m_fEndAlphaTime;	
	
	RwReal	m_fCurrentTime;	
	
	RwUInt32 m_flagPlayTimeType;
	VOID*	m_pEchoData;

	STATE	m_eState;
};

class CNtlPLCinematicGui : public RWS::CEventHandler
{
public:
	enum CINEMATICSTATE { CINEMATIC_STATE_NONE, CINEMATIC_STATE_BEGIN, CINEMATIC_STATE_END, CINEMATIC_STATE_IDLE };
	enum BALLOONSTATE { BALLOON_STATE_NONE, BALLOON_STATE_BEGIN, BALLOON_STATE_END, BALLOON_STATE_TEXTAPPEAR, BALLOON_STATE_IDLE };
	enum OBJECT { CINEMATIC = 0x01, BALLOON = 0x02, FLASH = 0x04, ALL = CINEMATIC | BALLOON | FLASH };

	//! Constructor & Destructor
	CNtlPLCinematicGui(VOID);
	virtual ~CNtlPLCinematicGui(VOID);

	//! Operation
	virtual RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);	

	virtual INT		StateUpdate( RwUInt32 uiElapsedTime );

	//! Event
	virtual VOID	HandleEvents( RWS::CMsg& msg );		

protected:
	//! Implement
	virtual VOID	SetCinematicViewShow( RwBool bShow );
	virtual VOID	SetCinematicBalloon( void* pData );
	virtual VOID	SetCinematicFlash( void* pData );

	virtual VOID	SetCinematicState( CINEMATICSTATE eState );
	virtual VOID	SetBalloonState( BALLOONSTATE eState );


	virtual VOID	CinematicDirection(VOID);
	virtual VOID	BalloonDirection( RwReal fElapsedTime );	

	virtual RwBool	IsStateNone( RwUInt32 flagObject );
	virtual VOID	CloseCinematic(VOID);
	
	//! CallBack
	virtual VOID	OnMouseDown( const CKey& key );

protected:

	virtual void	SkipCinematics(UINT32 hPc) = 0;

protected:

	gui::CSlot	m_slotOnMouseDown;

	//! Variable
	CINEMATICSTATE		m_eCinematicState;
	BALLOONSTATE		m_eBalloonState;
	
	RwReal				m_fBalloonTime;

	CNtlPLCinematicObject*	m_pCinematicObject;	
};

#endif