////////////////////////////////////////////////////////////////////////////////
// File: TBCommonDirectionGui.h
// Desc: 천하제일 무도회 공통 연출 Gui
//
// 2008.09.17 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_COMMON_DIRECTION_GUI_H__
#define __TB_COMMON_DIRECTION_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlParty.h"

#define TB_DIRECT_BASIS_WIDTH	1024
#define TB_DIRECT_BASIS_HEIGHT	768

//! Struct
struct stCOMMONINFO
{
	stCOMMONINFO(VOID) : bValid( FALSE ) {}

	RwBool				bValid;

	VOID	Valid( RwBool valid ) { bValid = valid; }
	RwBool	IsValid(VOID) { return bValid; }

	VOID	SizePosCalculation( gui::CComponent* pComponent, CRectangle rtOriginal, RwInt32 nScreenWidth, RwInt32 nScreenHeight, RwBool bResize = FALSE );
};

struct stSCROLLINFO : public stCOMMONINFO
{
	stSCROLLINFO(VOID) : pflaScroll( NULL ) {}

	gui::CFlash*		pflaScroll;
	gui::CHtmlBox*		phtmlText;
	CRectangle			rtOriginalScroll;

	gui::CSlot			slotScrollMovieEnd;
	gui::CSlot			slotScrollFSCallBack;
	gui::CSlot			slotHtmlPageDone;
		
	VOID Create( gui::CFlash* pScroll, gui::CHtmlBox* pText );
	VOID Destroy(VOID);
	VOID Update( RwReal fElapsedTime );
	VOID StartMovie(VOID);
	VOID EndMovie(VOID);
	VOID SetHtmlText( const RwChar* szHtmlFileName );
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	VOID OnScrollMovieEnd( gui::CComponent* pComponent );	
	VOID OnScrollFSCallBack( const RwChar* szCommand, const RwChar* szArgs );
	VOID OnScrollPageDone( RwInt32 nPage );
};

struct stONELINE : public stCOMMONINFO
{
	stONELINE(VOID) : pflaFirst( NULL ), byObjectID( 0xFF ) {}

	gui::CFlash*		pflaFirst;
	CRectangle			rtOriginalFirst;
	RwUInt8				byObjectID;

	gui::CSlot			slotFirstMovieEnd;
	
	VOID Create( gui::CFlash* pFirst );
	VOID Destroy(VOID);
	VOID LoadFlash( const RwChar* szFirst, RwUInt8 byID );
	VOID Update( RwReal fElapsedTime );
	VOID StartMovie(VOID);	
	VOID EndMovie(VOID);
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	VOID OnFirstMovieEnd( gui::CComponent* pComponent );
};

struct stTWOLINE : public stCOMMONINFO
{
	stTWOLINE(VOID) : pflaFirst( NULL ), pflaSecond( NULL ), byObjectID( 0xFF ), bAutoFoward( FALSE ) {}

	gui::CFlash*		pflaFirst;
	gui::CFlash*		pflaSecond;
	CRectangle			rtOriginalFirst;
	CRectangle			rtOriginalSecond;
	RwUInt8				byObjectID;
	RwBool				bAutoFoward;

	gui::CSlot			slotFirstMovieEnd;
	gui::CSlot			slotSecondMovieEnd;

	VOID Create( gui::CFlash* pFirst, gui::CFlash* pSecond );
	VOID Destroy(VOID);
	VOID LoadFlash( const RwChar* szFirst, const RwChar* szSecond, RwUInt8 byID, RwBool autoFoward );
	VOID Update( RwReal fElapsedTime );
	VOID StartFirstMovie(VOID);
	VOID StartSecondMovie(VOID);
	VOID EndMovie(VOID);
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	VOID OnFirstMovieEnd( gui::CComponent* pComponent );
	VOID OnSecondMovieEnd( gui::CComponent* pComponent );
};

class CTBCommonDirectionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	//! Constructor and Destructor
	CTBCommonDirectionGui( const RwChar* pName );
	~CTBCommonDirectionGui(VOID);

	//! Operation
	virtual VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	virtual VOID	HandleEvents( RWS::CMsg& msg );

protected:
	//! Implementation
	
	virtual VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! Variables
	//stSCROLLINFO		m_stScroll;
	stONELINE			m_stOneLine;
	stTWOLINE			m_stTwoLine;
};

struct stCOMMONMEMBERINFO
{
	stCOMMONMEMBERINFO(VOID) : pDialog( NULL ) {}
	virtual ~stCOMMONMEMBERINFO(VOID);

	gui::CDialog*		pDialog;

	virtual VOID	Create( gui::CComponent* pParent ) = 0;
	virtual VOID	SetBluePos(VOID) = 0;
	virtual VOID	SetRedPos(VOID) = 0;
	VOID			Show( bool bShow );
};

struct stTBudokaiMember;

struct stINDIVIDUALINFO : public stCOMMONMEMBERINFO
{
	stINDIVIDUALINFO(VOID);
	~stINDIVIDUALINFO(VOID);

	gui::CStaticBox*	pstbName;
	gui::CStaticBox*	pstbLevel;
	gui::CStaticBox*	pstbLevelValue;
	gui::CStaticBox*	pstbBattleRecord;
	gui::CStaticBox*	pstbBattleRecordValue;
	gui::CStaticBox*	pstbGuildName;
	gui::CStaticBox*	pstbGuildNameValue;
	gui::CPanel*		ppnlClass;

	VOID	Create( gui::CComponent* pParent );
	VOID	SetBluePos(VOID);
	VOID	SetRedPos(VOID);
	VOID	SetData( stTBudokaiMember* pMember );	
};

struct stPARTYMEMBERITEM 
{
	stPARTYMEMBERITEM(VOID);
	
	gui::CDialog*		pItemDialog;
	gui::CPanel*		ppnlClass;
	gui::CStaticBox*	pstbName;
	gui::CStaticBox*	pstbLevel;
	gui::CStaticBox*	pstbRankPoint;

	VOID Create( gui::CComponent* pParent );
	VOID SetPos( RwUInt8 byIndex );
	VOID SetData( stTBudokaiMember* pMember );
	VOID Show( bool bShow );
};

struct stTBudokaiTeam;

struct stPARTYINFO : public stCOMMONMEMBERINFO
{
	stPARTYINFO(VOID);	
	~stPARTYINFO(VOID);

	stPARTYMEMBERITEM	aPartyMemberItem[NTL_MAX_MEMBER_IN_PARTY];

	VOID	Create( gui::CComponent* pParent );
	VOID	SetBluePos(VOID);
	VOID	SetRedPos(VOID);	
};

//! Struct
struct stMEMBERINFO : public stCOMMONINFO
{
	stMEMBERINFO(VOID) : pflaTeam( NULL ), pMemberInfo( NULL ), bParty( FALSE ) {}
	~stMEMBERINFO(VOID);

	gui::CFlash*		pflaTeam;				
	CRectangle			rtOriginalTeam;

	stCOMMONMEMBERINFO*	pMemberInfo;
	
	RwBool				bParty;

	gui::CSlot			slotMovieEnd;
	gui::CSlot			slotFSCallBack;

	VOID Create( gui::CFlash* pTeam );
	VOID Destroy(VOID);
	VOID LoadFlash( const RwChar* szFile );
	VOID SetMemberInfo( stCOMMONMEMBERINFO* pInfo );
	VOID Update( RwReal fElapsedTime );
	VOID StartMovie(VOID);
	VOID EndMovie(VOID);
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	VOID OnMovieEnd( gui::CComponent* pComponent );
	VOID OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs );
};

struct stSCOREINFO : public stCOMMONINFO
{
	stSCOREINFO(VOID) : pflaScore( NULL ) {}

	gui::CFlash*		pflaScore;
	CRectangle			rtOriginalScore;

	VOID Create( gui::CFlash* pScore );
	VOID Destroy(VOID);
	VOID LoadFlash( const RwChar* szFile );
	VOID Update( RwReal fElapsedTime );	
	VOID StartMovie(VOID);
	VOID EndMovie(VOID);	
	VOID ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID SetScore( RwInt32 nBlue, RwInt32 nRed );
};

class CTBMainCommonDirectionGui : public CTBCommonDirectionGui
{
public:
	//! Constructor and Destructor
	CTBMainCommonDirectionGui( const RwChar* pName );
	~CTBMainCommonDirectionGui(VOID);

	//! Operation
	virtual VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	virtual VOID	HandleEvents( RWS::CMsg& msg );

protected:
	//! Implementation
	virtual VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! Variables
	stMEMBERINFO		m_stBlueMember;
	stMEMBERINFO		m_stRedMember;	
	stSCOREINFO			m_stScore;
};

class CTBFlashName
{
public:
	//! Destructor
	~CTBFlashName(VOID) {}

	//! Access
	static CTBFlashName* GetInstance(VOID) { static CTBFlashName instance; return &instance; }

	//! Functions
	const RwChar*	GetWait(VOID) { return "TB_Pleasewait.swf"; }
	const RwChar*	GetDepth( RwUInt8 byDepth );
	const RwChar*	GetWin(VOID) { return "TB_Win.swf"; }
	const RwChar*	GetLose(VOID) { return "TB_Lose.swf"; }
	const RwChar*	GetDraw(VOID) { return "TB_Draw.swf"; }
	const RwChar*	GetScroll(VOID) { return "TB_Scroll.swf"; }
	const RwChar*	GetReady(VOID) { return "TB_Ready.swf"; }
	const RwChar*	GetStart(VOID) { return "TB_Start.swf"; }
	const RwChar*	GetTitle(VOID) { return "TB_Title.swf"; }

	const RwChar*	GetPreliminary(VOID) { return "TB_Preliminary.swf"; }
	const RwChar*	GetPreliminaryEnd(VOID) { return "TB_Preliminary_End.swf"; }
	const RwChar*	GetWinner(VOID) { return "TB_TheWinner.swf"; }

	const RwChar*	GetNormalRound( RwUInt8 byRound );
	const RwChar*	GetNormalRoundStart(VOID) { return "TB_Normal_Round_start.swf"; }
	const RwChar*	GetNormalBlueTitle(VOID) { return "TB_Normal_Title_Blue.swf"; }
	const RwChar*	GetNormalRedTitle(VOID) { return "TB_Normal_Title_Red.swf"; }
	const RwChar*	GetNormalCounter(VOID) { return "TB_Normal_Round_Counter.swf"; }

	const RwChar*	GetSpecialRound( RwUInt8 byRound );
	const RwChar*	GetSpecialRoundStart(VOID) { return "TB_Special_Round_start.swf"; }
	const RwChar*	GetSpecialBlueTitle(VOID) { return "TB_Special_Title_Blue.swf"; }
	const RwChar*	GetSpecialRedTitle(VOID) { return "TB_Special_Title_Red.swf"; }
	const RwChar*	GetSpecialPartyKameTitle(VOID) { return "TB_Special_PartyInfo_Blue.swf"; }
	const RwChar*	GetSpecialPartyTsuruTitle(VOID) { return "TB_Special_PartyInfo_Red.swf"; }
	const RwChar*	GetSpecialCounter(VOID) { return "TB_Special_Round_Counter.swf"; }

	const RwChar*	GetFirstPrize(VOID) { return "TB_First_Prize.swf"; }
	const RwChar*	GetSecondPrize(VOID) { return "TB_Second_Prize.swf"; }
	const RwChar*	Get34Prize(VOID) { return "TB_34_Prize.swf"; }

	const RwChar*	GetLuaPreFinal(VOID) { return "Tenkaichi_preFinal()"; }
	const RwChar*	GetLuaFinal(VOID) { return "Tenkaichi_Final()"; }
	const RwChar*	GetLuaAward(VOID) { return "Tenkaichi_AwardingPrizes()"; }

	const RwChar*	GetLuaBlueTeamInfoCallBack(VOID) { return "TurtleTeamInfoGui"; }
	const RwChar*	GetLuaRedTeamInfoCallBack(VOID) { return "CraneTeamInfoGui"; }
	const RwChar*	GetLuaAwardInfoCallBack(VOID) { return "AwardInfoGui"; }

private:
	CTBFlashName(VOID) {}
};

static CTBFlashName* GetTBFlashName(VOID)
{
	return CTBFlashName::GetInstance();
}

#endif //__TB_MAJOR_DIRECTION_GUI_H__