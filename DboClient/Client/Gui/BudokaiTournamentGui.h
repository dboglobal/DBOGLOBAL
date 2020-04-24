////////////////////////////////////////////////////////////////////////////////
// File: BudokaiTournamentGui.h
// Desc: 천하제일무도회 대전표
//
// 2008.06.04 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BUDOKAI_TOURNAMENT_GUI_H__
#define __BUDOKAI_TOURNAMENT_GUI_H__

#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "ceventhandler.h"
#include "NtlBudokai.h"
#include "NtlTimeQuest.h"

// 참가자 
class CBudokaiTournamentEntry
{
public:
	//! Constructor & Destructor
	CBudokaiTournamentEntry( gui::CComponent* pParent, RwUInt8 byPos,
							 RwInt32 nMark1Left, RwInt32 nMark1Top, RwInt32 nWidth, RwInt32 nHeight, RwInt32 nOffset );
	~CBudokaiTournamentEntry(VOID);

	//! Operation
	// 1. 참가자 패킷 받았을때 전부 채워준다.
	VOID	SetEntry( const WCHAR* pName, RwUInt16 wJoinID );

private:
	//! Callback
	// 1. Mouse Enter&Leave시 InfoWnd 띄우기
	gui::CSlot	m_slotMouseEnter;
	gui::CSlot	m_slotMouseLeave;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;

	VOID	OnMouseEnter( gui::CComponent* pComponent );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);

	//! Variable
	// 0. 본선 참가 번호
	RwUInt8			m_byPos;
	RwUInt16		m_wJoinID;
	// 1. 참가자 이름.
	std::wstring	m_wstrName;
	// 2. 참가번호 CPanel.
	gui::CPanel*	m_ppnlMark;
	// 3. 포커스 Surface 
	CSurfaceGui		m_surFocus;	
};

// 대전정보
class CBudokaiTournamentFightInfo
{
public:
	//! Enumuration
	enum ANIMATIONSTATE { NONE, INIT_VERTICAL, HORIZONTAL, VERTICAL, END };
	enum CHILDSTATE { ENDOFCHILDANIMATION = 2 };

	struct stENTRYINFO
	{
		stENTRYINFO::stENTRYINFO(VOID)
		{
			bEntry = FALSE;
			wJoinID = INVALID_WORD;
			byScore = INVALID_BYTE;
		}

		RwBool		bEntry;
		RwUInt16	wJoinID;
		RwUInt8		byScore;
	};

	//! Constructor & Destructor
	CBudokaiTournamentFightInfo( CBudokaiTournamentFightInfo* pAboveNode, gui::CComponent* pParent, RwUInt8 byMatchNumber, RwUInt8 byMatchLevel, RwUInt8 byServerMatchNumber, RwUInt8 byServerMatchLevel, RwBool bParty,
								 RwInt32 nOriginLeft, RwInt32 nOriginTop , RwInt32 nWidth, RwInt32 nHeight, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nLineEndY );
	~CBudokaiTournamentFightInfo(VOID);

	//! Operation
	// 0. 업데이트
	VOID	Update( RwReal fElapsedTime );		
	VOID	UpdateInitVertical( RwReal fElapsedTime );
	VOID	UpdateHorizontal( RwReal fElapsedTime );
	VOID	UpdateVertical( RwReal fElapsedTime );

	// 1. 대전 결과 받았을때 Line데이터 채우기, 한번 들어온 결과는 플래그를 켜준다.	애니메이션 플래그도 온~!!
	VOID	SetFightResult( RwUInt16 wWinnerJoinID, RwUInt16 wJoinID1, RwUInt16 wJoinID2, RwUInt8 byScore1, RwUInt8 byScore2 );

	// 2. 각 진출했을때, 밑에서 올라온 데이터로 채워준다. 
	VOID	SetEntryData( RwUInt8 byIdx, RwUInt16 wJoinID );
		
	//! Attribute
	RwUInt8		GetServerMatchNumber(VOID) { return m_byServerMatchNumber; }
	RwUInt8		GetServerMatchLevel(VOID) { return m_byServerMatchLevel; }
	RwUInt16	GetWinnerID(VOID) { return m_wWinnerTeamJoinID; }
	RwUInt16	GetLoserID(VOID) { return m_wLoserTeamJoinID; }
	RwUInt16	GetLeftEntryID(VOID) { return m_stEntry[0].wJoinID; }
	RwUInt16	GetRightEntryID(VOID) { return m_stEntry[1].wJoinID; }
	RwBool		IsPartyData(VOID) { return m_bParty; }
		
private:
	//! Implements
	VOID	SetAnimationState( ANIMATIONSTATE eAnimationState );

	RwInt32	GetHorizontalOrigin(VOID);
	RwInt32	GetHorizontalOffset(VOID);
	RwBool	IsLeftNode(VOID);
	
	//! Callback
	// 1. 버튼 클릭시 대전정보 요청패킷 보내기. 	
	gui::CSlot	m_slotClickInfoBtn;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;

	VOID	OnClickInfoBtn( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);
	
	//! Variable
	// 1. CButton
	gui::CButton*	m_pbtnBattleInfo;
	// 2. Line Surface 3개, 말단인 경우 5개, 그 만큼의 Rect정보, 말단인 경우
	CSurfaceGui		m_surLeftInitVerticalLine;
	CSurfaceGui		m_surRightInitVerticalLine;
	CSurfaceGui		m_surLeftHorizontalLine;
	CSurfaceGui		m_surRightHorizontalLine;
	CSurfaceGui		m_surCenterVerticalLine;

	CRectangle		m_rtLeftInitVerticalLine;
	CRectangle		m_rtRightInitVerticalLine;
	CRectangle		m_rtLeftHorizontalLine;
	CRectangle		m_rtRightHorizontalLine;
	CRectangle		m_rtCenterVerticalLine;

	RwInt32			m_nOriginLeft;
	RwInt32			m_nHorizontalOffset;
	
	// 각 라인의 이동 시간( 거리(픽셀) / 속도(픽셀/시간) )
	RwReal			m_fMoveTimeInitVertical;
	RwReal			m_fMoveTimeHorizontal;
	RwReal			m_fMoveTimeVertical;

	// 현재 시간
	RwReal			m_fCurrentTime;

	// 3. 위치 정보( 몇강 몇번째 경기 )
	RwUInt8			m_byMatchNumber;		// 왼쪽부터 0번
	RwUInt8			m_byMatchLevel;			// 아래부터 0번
	RwUInt8			m_byServerMatchNumber;	// 왼쪽부터 0번, 서버넘버
	RwUInt8			m_byServerMatchLevel;	// 결승부터 0번, 서버넘버
	// 4. 애니메이션되야하는 플래그( 하위 노드 2개로 부터 종료 신호를 받아야함. 즉 2가 되어야 함
	RwInt32			m_nEndOfBelowAnimation;	
	// 5. 일정 시간이 지날때까지 패킷을 못보내도록 시간저장.
	RwUInt32		m_uiWaitTime;			
	// 6. 각 대전자의 참가 ID
	stENTRYINFO		m_stEntry[2];			
	// 7. 대전 결과
	RwUInt16		m_wWinnerTeamJoinID;
	RwUInt16		m_wLoserTeamJoinID;
	// 8. 애니메이션 상태
	ANIMATIONSTATE	m_eAnimationState;		// 결과 받았을때 0이외의 값이 들어옴.
	// 9. 상위 노드 
	CBudokaiTournamentFightInfo* m_pAboveFightInfo;
	// 10. 부모 컴포넌트
	gui::CComponent*	m_pdlgParent;
	// 11. 파티 모드인가
	RwBool			m_bParty;	
};

class CBudokaiTournamentEntryGroup
{
public:
	CBudokaiTournamentEntryGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nOffset, gui::CDialog* pDialog );
	~CBudokaiTournamentEntryGroup(VOID);

	VOID	Show( bool bShow );
	VOID	SetData( sVARIABLE_DATA* pData, WORD wEntryTeam );

private:
	//! Implementation
	
	//! Variables
	gui::CDialog* m_pDialog;
	CBudokaiTournamentEntry** m_pEntry;
	RwInt32	m_nEntryCount;
	RwInt32 m_nPosOffset;				// ID Offset
};

class CBudokaiTournamentFightInfoGroup
{
public:
	CBudokaiTournamentFightInfoGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nEndLineY, gui::CDialog* pDialog );
	~CBudokaiTournamentFightInfoGroup(VOID);

	VOID	CreateData( RwUInt8 byIndex, CBudokaiTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwBool bParty );	// peessitemp : 귀찮다 -_-
	//RwBool	SetEntryData( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2 );
	//RwBool	SetResultData( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwUInt16 wWinnerJoinID );

	CBudokaiTournamentFightInfo*	FindFightInfo( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber );
	
	VOID	Update( RwReal fElapsed );
	VOID	Show( bool bShow );	
	
private:
	//! Variables
	gui::CDialog* m_pDialog;
	CBudokaiTournamentFightInfo** m_ppFightInfo;
	RwInt32 m_nFightInfoCount;
	RwInt32 m_nPosOffset;
	
	CPos	m_posOrigin;
	RwInt32	m_nVerticalOffset;
	RwInt32	m_nHorizontalOffset;
	RwInt32 m_nEndLineY;
};

class CBudokaiTournamentGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum ePage { TOURNAMENT_INDIVIDUAL_A, TOURNAMENT_INDIVIDUAL_B, TOURNAMENT_PARTY, NUM_PAGE, INVALID_PAGE = 0xFFFFFFFF };

	//! Constructor & Destructor
	CBudokaiTournamentGui( const RwChar* pName );
	~CBudokaiTournamentGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32	SwitchDialog( bool bOpen );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );	
private:	
	//! Implements
	VOID	CreateDataGroup(VOID);
	VOID	SetPage( ePage ePageType );

	//! Callback
	gui::CSlot		m_slotClickClose;
	gui::CSlot		m_slotClickRightArrow;
	gui::CSlot		m_slotClickLeftArrow;
	gui::CSlot		m_slotClickPartyBtn;
	gui::CSlot		m_slotClickIndividualBtn;

	VOID	OnClickClose( gui::CComponent* pComponent );
	VOID	OnClickRightArrow( gui::CComponent* pComponent );
	VOID	OnClickLeftArrow( gui::CComponent* pComponent );
	VOID	OnToggledPartyBtn( gui::CComponent* pComponent, bool bToggleDown );
	VOID	OnToggledIndividualBtn( gui::CComponent* pComponent, bool bToggleDown );

	//! Variable
	gui::CDialog*						m_pPageDialog[NUM_PAGE];
	
	CBudokaiTournamentEntryGroup*		m_pEntryGroup[NUM_PAGE];
	CBudokaiTournamentFightInfoGroup*	m_pFightInfoGroup[NUM_PAGE];
	
	RwUInt8							m_byMaxEntry;			// 최대 몇강. 
	ePage							m_eCurrenctPage;		// 현재 보이는 페이지.
	RwUInt8							m_byIndividualFightInfoInputDoneCount;	// 현재 입력된 FightInfo 갯수.(서버 리퀘스트 용) 개인전
	RwUInt8							m_byTeamFightInfoInputDoneCount;		// 현재 입력된 FightInfo 갯수.(서버 리퀘스트 용) 파티전		

	gui::CButton*					m_pbtnRightArrow;
	gui::CButton*					m_pbtnLeftArrow;
	gui::CStaticBox*				m_pstbIndividualGroupType;

	gui::CButton*					m_pbtnParty;			// Toggle형식 버튼. 파티시 위의 세 컴포넌트가 사라지거나, 비활성화
	gui::CButton*					m_pbtnIndividual;
	
	gui::CPanel*					m_ppnlLineBack;			// 라인.
	gui::CPanel*					m_ppnlQuestionMark;		// -_-;;
	
	gui::CStaticBox*				m_pstbTitle;			
	gui::CStaticBox*				m_pstbInfomation;		// 설명.	

	gui::CButton*					m_pbtnClose;

public:
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA> m_mapIndividualEntryData;
	static std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA> m_mapTeamEntryData;

	static sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* FindIndividualEntryData( RwUInt16 wJoinID );
	static sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* FindTeamEntryData( RwUInt16 wJoinID );
};


#endif//__BUDOKAI_TOURNAMENT_GUI_H__