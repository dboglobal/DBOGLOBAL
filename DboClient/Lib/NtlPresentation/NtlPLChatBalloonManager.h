////////////////////////////////////////////////////////////////////////////////
// File: NtlPLChatBalloonManager.h
// Desc: PL Layer ChatBalloonManager
//
// 2008.08.07 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_PL_CHAT_BALLOON_MANAGER_H__
#define __NTL_PL_CHAT_BALLOON_MANAGER_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

////////////////////////////////////////////////////////////////////////////////
// Data

class CNtlPLBalloonGui
{
public:
	enum TYPE { FIRST_TYPE_LAYER = 0x1000, 
				TYPE_SERVER = 0x1001,		 
				TYPE_QUEST = 0x1002,
				TYPE_NPC_MARK = 0x1004,
				TYPE_NPC = 0x1008, 
				TYPE_CHAT = 0x1010,				
				SECOND_TYPE_LAYER = 0x2000,
				TYPE_PRIVATESHOP = 0x2001,
				TYPE_BATTLEGAUGE = 0x2002,
				TYPE_SPECTATOR = 0x2004,
				ALL = 0xFFFF };				

	enum STATE { FADE_IN, SHOW, FADE_OUT, HIDE };

	enum PRIORITY { MORE_PRIORITY, EQUAL_PRIORITY, LESS_PRIORITY,	// 같은 레이어인 경우
					UPPER_LAYER, LOWER_LAYER };						// 다른 레이어인 경우

	CNtlPLBalloonGui( RwReal fLifeTime, RwInt32 eType ) 
		:m_fCurrentTime( 0.0f ), m_fLifeTime( fLifeTime ), m_eType( eType ), m_eState( FADE_IN ) {}
	virtual ~CNtlPLBalloonGui(VOID) {}

	virtual VOID	SetMessage( const WCHAR* pMessage ) {}
	virtual VOID	FadeoutMessage(VOID) {}
	virtual RwV3d*	GetPosition(VOID) = 0;
	virtual RwInt32	Update( RwReal fElapsedTime ) = 0;
	virtual VOID	Paint(VOID) = 0;

	VOID		SetState( RwInt32 eState );
	
	RwInt32		GetType(VOID) { return m_eType; }
	RwInt32		GetState(VOID) { return m_eState; }

	RwInt32		PriorityCheck( RwUInt32 eType );	// return value : PRIORITY  객체가 매개변수의 타입에 비해 어떠한가.	

protected:

	RwReal		m_fCurrentTime;
	RwReal		m_fLifeTime;
	RwInt32		m_eType;
	RwInt32		m_eState;
	CPos		m_posOffset;
};

class CSpectatorBalloonGui : public CNtlPLBalloonGui
{
public: 
	CSpectatorBalloonGui( RwV3d& vPos, const WCHAR* pMessage, RwReal fLifeTime, RwReal fFadeInTime, RwInt32 eType, RwUInt8 byBalloonType );
	virtual ~CSpectatorBalloonGui(VOID);

	virtual VOID	FadeoutMessage(VOID);
	virtual RwV3d*	GetPosition(VOID) { return &m_vPos; }
	virtual RwBool	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);

	RwUInt8			GetBalloonType(VOID) { return m_byBalloonType; }

protected:
	RwBool			PositionUpdate(VOID);
	VOID			FadeEffect(VOID);

	gui::CFlash*		m_pFlash;
	RwV3d				m_vPos;
	RwUInt8				m_byBalloonType;
	RwBool				m_bStandardSize;

	std::wstring		m_wstrString;	// 원본 메세지 저장.
	RwReal				m_fFadeInTime;
};

////////////////////////////////////////////////////////////////////////////////
// Manager

#define BALLOON_MAX_PRESENT		30

class CNtlPLBalloonManager : public CNtlPLGui, public RWS::CEventHandler
{
private:
	//! Constructor
	CNtlPLBalloonManager( const RwChar* pName );
public:
	//! Typedef
	typedef struct _stDISPLAYBALLOON
	{
		CNtlPLBalloonGui*	pBalloon;
		RwReal				fDist;
	}stDISPLAYBALLOON;

	enum RESULT { UR_SHOW, UR_NOTSHOW, UR_DELETE };

	//! Destructor
	~CNtlPLBalloonManager(VOID);

	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CNtlPLBalloonManager*	GetInstance(VOID);
	static gui::CStaticBox*			GetTestText(VOID);

	virtual VOID  Update( RwReal fElapsed );

	static RwInt32 Compare( const VOID* valLeft, const VOID* valRight );

	//! Operation
	VOID AddSpectatorBalloon( RwV3d& vPos, const WCHAR* pMessage, RwReal fLifeTime, RwReal fFadeInTime, RwInt32 eType, RwUInt8 byBalloonType );
	VOID FadeoutAllBalloon(VOID);
	VOID RemoveAllBalloon(VOID);

private:
	//! Implementation
	VOID AddBalloonData( CNtlPLBalloonGui* pBalloon );
	VOID RemoveBalloonData( CNtlPLBalloonGui* pBalloon );
	VOID RemoveAllBalloonData(VOID);

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

	//! CallBack
	VOID OnPaint(VOID);

	gui::CSlot m_slotPaint;

	//! Variable
	std::list<CNtlPLBalloonGui*>	m_listBalloon;

	stDISPLAYBALLOON	m_aDiaplsyBalloon[BALLOON_MAX_PRESENT];
	RwInt32				m_nDisplayCount;

	RwUInt32			m_uiVisibleBalloonType;
	
	static gui::CStaticBox* m_pTestText;
	static CNtlPLBalloonManager* m_pInstance;
};

static CNtlPLBalloonManager* GetNtlPLBalloonManager(VOID)
{
	return CNtlPLBalloonManager::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////

#endif//__NTL_PL_CHAT_BALLOON_MANAGER_H__
