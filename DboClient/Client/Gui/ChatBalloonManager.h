////////////////////////////////////////////////////////////////////////////////
// File: ChatBalloonManager.h
// Desc: ChatBalloon 
//
// 2006.08.22 Peessi@hitel.net   
// 2007.08.28 Peessi@hitel.net 
//    - 오브젝트에 레이어가 서로 다른 복수개의 풍선도움말이 달릴 수 있다. 
//	  - 타입에 따라 랜더되는 우선순위를 정하고 랜더 여부를 정할 수 있다. 
//	  - 복수개의 풍선도움말의 위치지정이 매우 하드하게 코딩되어있다. -_-
////////////////////////////////////////////////////////////////////////////////

#ifndef __CHAT_BALLOON_MANAGER_H__
#define __CHAT_BALLOON_MANAGER_H__

#include "NtlPLGui.h"
#include "NtlSob.h"
#include "ceventhandler.h"
#include "Windowby3.h"
#include "NumberGui.h"

#define BALLOON_CHAT_DISPTIME	6.0f
#define BALLOON_NPC_DISPTIME	10.0f

////////////////////////////////////////////////////////////////////////////////
// Data

class CBalloonGui
{
public:
	enum TYPE { FIRST_TYPE_LAYER = 0x1000, 
		TYPE_SERVER = 0x1001,		 
		TYPE_QUEST = 0x1002,
		TYPE_NPC_MARK = 0x1004,
		TYPE_NPC = 0x1008, 
		TYPE_DICE = 0x1010,
		TYPE_AVATAR_CHAT = 0x1020,			
		TYPE_PLAYER_CHAT = 0x1040,
		SECOND_TYPE_LAYER = 0x2000,				
		TYPE_PRIVATESHOP = 0x2001,
		TYPE_BATTLEGAUGE = 0x2002,				
		ALL = 0xFFFF };

	enum STATE { FADE_IN, SHOW, FADE_OUT, HIDE };

	enum PRIORITY { MORE_PRIORITY, EQUAL_PRIORITY, LESS_PRIORITY,	// 같은 레이어인 경우
		UPPER_LAYER, LOWER_LAYER };						// 다른 레이어인 경우

	CBalloonGui( CNtlSob* pOwnerObject, RwReal fLifeTime, CPos& posOffsetDelta, RwInt32 eType ) 
		:m_fCurrentTime( 0.0f ), m_pOwnerSobObject( pOwnerObject ), m_hSerialID( pOwnerObject->GetSerialID() ), m_fLifeTime( fLifeTime ), 
		m_eType( eType ), m_eState( FADE_IN ), m_posOffsetDelta( posOffsetDelta ), m_bVisible( TRUE ) {}
	virtual ~CBalloonGui(VOID) {}

	virtual VOID	SetMessage( RwUInt32 uiNumber ) {}
	virtual VOID	SetMessage( const WCHAR* pMessage ) {}
	virtual VOID	SetMessage( const WCHAR* pMessage, RwBool bTimeProc ) {}
	virtual VOID	SetMessage( std::wstring& wstrData ) {}
	virtual VOID	SetMessage( std::wstring& wstrData, RwBool bTimeProc, RwUInt32 uiID ) {}
	virtual VOID	SetMessage( const WCHAR* pMessage, RwBool bReady, RwBool bTimeProc ) {}
	virtual VOID	FadeoutMessage(VOID) {}
	virtual RwInt32	Update( RwReal fElapsedTime ) = 0;
	virtual VOID	Paint(VOID) = 0;

	VOID		SetState( RwInt32 eState );
	VOID		SetVisible( RwBool bVisible ) { m_bVisible = bVisible; }
	VOID		SetOffsetDelta( CPos& posOffsetDelta ) { m_posOffsetDelta = posOffsetDelta; }

	CNtlSob*	GetOwnerObject(VOID) { return m_pOwnerSobObject; }
	RwInt32		GetType(VOID) { return m_eType; }
	RwInt32		GetState(VOID) { return m_eState; }
	RwBool		GetVisible(VOID) { return m_bVisible; }
	CPos&		GetOffset(VOID) { return m_posOffset; }
	CPos&		GetOffsetDelta(VOID) { return m_posOffsetDelta; }

	RwInt32		PriorityCheck( RwUInt32 eType );	// return value : PRIORITY  객체가 매개변수의 타입에 비해 어떠한가.

	VOID		Show3DName( RwBool bShow );

protected:

	RwReal		m_fCurrentTime;
	RwReal		m_fLifeTime;
	CNtlSob*	m_pOwnerSobObject;	
	RwUInt32	m_hSerialID;

	RwBool		m_bVisible;
	RwInt32		m_eType;
	RwInt32		m_eState;
	CPos		m_posOffsetDelta;
	CPos		m_posOffset;
};

class CChatBalloonGui : public CBalloonGui
{
public:
	CChatBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pMessage, RwInt32 eType, RwBool bPartyMsg = FALSE );
	virtual ~CChatBalloonGui(VOID);

	virtual VOID	SetMessage( const WCHAR* pMessage );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwInt32	Update( RwReal fElapseTime );
	virtual VOID	Paint(VOID);

protected:
	RwBool PositionUpdate(VOID);
	VOID   FadeEffect(VOID);

	gui::CBalloon*		m_pGuiBalloon;
	gui::CStaticBox*	m_pName;
	RwBool				m_bIsPartyMessage;
};

class CNPCBalloonGui : public CBalloonGui
{
public: 
	CNPCBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pMessage, RwReal fLifeTime, RwInt32 eType, RwUInt8 byBalloonType, RwBool bTimeProc );
	virtual ~CNPCBalloonGui(VOID);

	virtual VOID	SetMessage( const WCHAR* pMessage, RwBool bTimeProc );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwBool	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);

	std::wstring	GetText(VOID);
	RwUInt8			GetBalloonType(VOID) { return m_byBalloonType; }

protected:
	RwBool			PositionUpdate(VOID);
	VOID			FadeEffect(VOID);

	gui::CFlash*		m_pFlash;
	RwBool				m_bTimeProc;
	RwUInt8				m_byBalloonType;
	RwBool				m_bStandardSize;

	std::wstring		m_wstrString;	// 원본 메세지 저장.
};

class CPrivateShopBalloonGui : public CBalloonGui
{
public:
	CPrivateShopBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, RwBool bReady, const WCHAR* pMessage );
	virtual ~CPrivateShopBalloonGui(VOID);

	virtual VOID	SetMessage( const WCHAR* pMessage, RwBool bReady );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwBool	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);

	gui::CSurface	GetSurface( RwBool bReady );
	VOID			SetTextBkColor( RwBool bReady );
	VOID			SetTextColor( RwBool bReady );
	VOID			SetBackground( RwBool bReady );

	VOID			OnMovePanel(RwInt32 param1, RwInt32 param2);
	VOID			OnPaintPanel();

	VOID			ResizeBackground();

protected:
	RwBool	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);

	CWindowby3			m_wbBackground;

	gui::CSlot			m_slotMovePanel;
	gui::CSlot			m_slotPaintPanel;

	gui::CPanel*		m_pPanel;
	gui::CStaticBox*	m_pText;
};

class CQuestBalloonGui : public CBalloonGui
{
public:
	CQuestBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, std::wstring& wstrHtmlData, RwUInt32 uiID, RwBool bTimeProc );
	virtual ~CQuestBalloonGui(VOID);

	virtual VOID	SetMessage( std::wstring& wstrHtmlData, RwBool bTimeProc, RwUInt32 uiID );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwInt32	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);

	RwUInt32 GetTextID(VOID) { return m_uiID; }

protected:
	RwBool	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);

	gui::CBalloon*		m_pBalloon;
	gui::CPanel*		m_pMark;	
	gui::CStaticBox*	m_pName;	

	RwUInt32			m_uiID;
	RwBool				m_bTimeProc;
};

class CMarkBalloonGui : public CBalloonGui
{
public:
	CMarkBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, std::wstring& wstrHtmlData );
	virtual ~CMarkBalloonGui(VOID);

	virtual VOID	SetMessage( std::wstring& strHtmlData );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwInt32	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);	

protected:
	RwBool	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);

	gui::CBalloon*		m_pBalloon;
	gui::CPanel*		m_pMark;	
	gui::CStaticBox*	m_pName;
};

class CDiceBalloonGui : public CBalloonGui
{
public:
	CDiceBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, RwUInt32 uiNumber );
	virtual ~CDiceBalloonGui(VOID);

	virtual VOID	SetMessage( RwUInt32 uiNumber );
	virtual VOID	FadeoutMessage(VOID);
	virtual RwInt32	Update( RwReal fElapsedTime );
	virtual VOID	Paint(VOID);	

protected:
	RwBool	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);

	gui::CPanel*		m_pPanel;
	CNumberGui			m_NumberGui;
};

class CBattleGaugeGui : public CBalloonGui
{
public:
	CBattleGaugeGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pName, RwUInt32 uiMaxLp, RwUInt32 uiLp, RwBool bMyTeam );
	virtual ~CBattleGaugeGui(VOID);

	VOID	SetData( RwUInt32 uiMaxLp, RwUInt32 uiLp );
	VOID	FadeoutMessage(VOID);
	RwInt32	Update( RwReal fElapsedTime );
	VOID	Paint(VOID);

protected:
	RwBool	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);

	gui::CPanel*		m_pBack;
	gui::CProgressBar*	m_pLPGauge;
	gui::CStaticBox*	m_pName;
};

////////////////////////////////////////////////////////////////////////////////
// Manager

#define BALLOON_MAX_PRESENT		30

class CBalloonManager : public CNtlPLGui, public RWS::CEventHandler
{
private:
	//! Constructor
	CBalloonManager( const RwChar* pName );
public:
	//! Typedef
	typedef std::multimap<CNtlSob*,CBalloonGui*>					MMAP_OBJ_BALLOON;
	typedef std::multimap<CNtlSob*,CBalloonGui*>::iterator			MMAP_OBJ_BALLOON_IT;
	typedef std::pair<MMAP_OBJ_BALLOON_IT,MMAP_OBJ_BALLOON_IT>		MMAP_OBJ_IT_PAIR;
	typedef std::multimap<RwInt32,CBalloonGui*>						MMAP_TYPE_BALLOON;
	typedef std::multimap<RwInt32,CBalloonGui*>::iterator			MMAP_TYPE_BALLOON_IT;
	typedef std::pair<MMAP_TYPE_BALLOON_IT,MMAP_TYPE_BALLOON_IT>	MMAP_TYPE_IT_PAIR;

	typedef struct _stDISPLAYBALLOON
	{
		CBalloonGui* pBalloon;
		RwReal       fDist;
	}stDISPLAYBALLOON;

	enum RESULT { UR_SHOW, UR_NOTSHOW, UR_DELETE };

	//! Destructor
	~CBalloonManager(VOID);

	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CBalloonManager* GetInstance(VOID);
	static gui::CStaticBox* GetTestText(VOID);

	virtual VOID  Update( RwReal fElapsed );

	static RwInt32 Compare( const VOID* valLeft, const VOID* valRight );

	//! Operation
	VOID AddAvatarBalloon( const WCHAR* pMessage, RwBool bPartyMsg = FALSE);
	VOID AddPlayerBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwBool bPartyMsg = FALSE);
	VOID AddNPCBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwReal fLifeTime, RwBool bServer = FALSE, RwBool bTimeProc = TRUE, RwUInt8 byBalloonType = 6 );
	VOID AddPrivateShopBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwBool bReady );
	VOID AddQuestBalloon( CNtlSob* pSobObject, std::wstring& wstrHtmlData, RwUInt32 uiID, RwBool bTimeProc = TRUE );
	VOID AddMarkBalloon( CNtlSob* pSobObject, std::wstring& wstrHtmlData );
	VOID AddDiceBalloon( CNtlSob* pSobObject, RwUInt32 uiNumber );
	VOID AddBattleGauge( CNtlSob* pSobObject, const WCHAR* pName, RwUInt32 uiMaxLp, RwUInt32 uiLp, RwBool bMyTeam );

	VOID FadeOutBalloon( CNtlSob* pSobObject, RwUInt32 eBalloonType );

	VOID SetBattleGaugeData( CNtlSob* pSobObject, RwUInt32 uiMaxLp, RwUInt32 uiLp );
	VOID VisibleBalloon( CNtlSob* pSobObject, RwBool bVisible, RwInt32 flagExceptType );

	RwBool HasNPCBalloon( CNtlSob* pSobObject );
	RwBool HasQuestBalloon( CNtlSob* pSobObject );

	VOID SetVisibleBalloonType( RwUInt32 eBalloonType ) { m_uiVisibleBalloonType = eBalloonType; }

	VOID RemoveAllBalloonData(VOID);

private:
	//! Implementation
	CBalloonManager::MMAP_OBJ_BALLOON::size_type AlreadyExist( CNtlSob* pSobObject );

	VOID AddBalloonData( CBalloonGui* pGui );
	VOID RemoveBalloonData( CBalloonGui* pGui, RwBool bDeleteFromTypeMap = TRUE );
	VOID RemoveBalloonData( CNtlSob* pObj );	
	VOID RemoveBalloonDataFromRenderQueue( CBalloonGui* pBalloon );

	RwBool IsEnableToCreate( RwInt32 eBalloonType );

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

	//! CallBack
	VOID OnPaint(VOID);

	gui::CSlot m_slotPaint;

	//! Variable
	MMAP_OBJ_BALLOON	m_mmapObjBalloon;
	MMAP_TYPE_BALLOON	m_mmapTypeBalloon;

	stDISPLAYBALLOON	m_aDiaplsyBalloon[BALLOON_MAX_PRESENT];
	RwInt32				m_nDisplayCount;

	RwUInt32			m_uiVisibleBalloonType;
	RwBool				m_bAvatarDie;

	static gui::CStaticBox* m_pTestText;

	static CBalloonManager* m_pInstance;
};

static CBalloonManager* GetBalloonManager(VOID)
{
	return CBalloonManager::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////

#endif//__CHAT_BALLOON_MANAGER_H__
