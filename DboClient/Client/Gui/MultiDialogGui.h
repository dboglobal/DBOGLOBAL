////////////////////////////////////////////////////////////////////////////////
// File: MultiDialogGui.h
// Desc: MultiDialogGui
//		 대화와 여러 선택을 할수 있다.
//
// 2006.10.11 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MULTI_DIALOG_GUI_H__
#define __MULTI_DIALOG_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GuiLineTree.h"
#include "NtlSLEvent.h"
#include "DboTSCoreDefine.h"
#include "Windowby3.h"

class CCommandList;
class CCommandNode;
class CNtlSobNpc;
class CQuestGui;

struct sMultiDialogData
{
	enum BALLOON_TYPE { TYPE_NONE, TYPE_JUST_NPC_BALLOON, TYPE_JUST_QUEST_BALLOON, TYPE_NPC_BALLOON, TYPE_QUEST_BALLOON, TYPE_JUST_MSG };

	sMultiDialogData(VOID) : eBalloonType( TYPE_NONE ), bTimeProc( TRUE ), uiQuestID( 0xFFFFFFFF ) {}
	VOID Clear(VOID) { eBalloonType = TYPE_NONE; bTimeProc = TRUE; uiQuestID = 0xFFFFFFFF; wstrText.clear(); }
	
	RwInt32			eBalloonType;
	RwBool			bTimeProc;
	
	// Quest Balloon 용 데이터
	RwUInt32		uiQuestID;
	// Quest Balloon, NPC Balloon 용 데이터
	std::wstring	wstrText;	
};

class CMultiDialogGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum DLGTYPE { TYPE_NONE, TYPE_NPCTALK, TYPE_QUESTCAT, TYPE_USERSELECT, TYPE_NPCQUESTTALK, TYPE_OBJQUESTTALK, TYPE_SERVERSELECT, TYPE_GAMBLE };

	//! Constructor & Destructor
	CMultiDialogGui(VOID);
	CMultiDialogGui( const RwChar* pName );
	~CMultiDialogGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);
	
	RwInt32	SwitchDialog( bool bOpen );
	
	VOID		SetDialogType( DLGTYPE eType ) { m_eDlgType = eType; }
	DLGTYPE		GetDialogType(VOID) { return m_eDlgType; }
	VOID		SetTargetSerial( RwUInt32 uiSerial ) { m_hTargetSerialID = uiSerial; }
	RwUInt32	GetTargetSerial(VOID) { return m_hTargetSerialID; }

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SendTriggerAction( vecdef_QUEST_INFO_LIST* pTriggerList, RwUInt8 eEventGenType, RwUInt32 uiOwnerID );
	VOID	ReserveDialog( std::wstring& wstrText, RwUInt32 uiQuestID, RwInt32 eType, RwBool bTimeProc );
	VOID	ReserveDialog( std::wstring& wstrText, RwInt32 eType, RwBool bTimeProc );

	VOID	SetMsgBox( std::wstring& wstrText );
	
	//! Callback
	//gui::CSlot		m_slotClickExitButton;
	gui::CSlot		m_slotCaptureMouseDown;
	gui::CSlot		m_slotCaptureWheelMove;
	
	gui::CSlot		m_slotMsgResize;
	gui::CSlot		m_slotMsgMove;
	gui::CSlot		m_slotPaint;

	//VOID	OnClickExitButton( gui::CComponent* pComponent );
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	VOID	OnMsgResize( RwInt32 nOldCX, RwInt32 nOldCY );
	VOID	OnMsgMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);

	//! Variables
	CCommandList*	m_pCommandList;
	gui::CStaticBox* m_pstbMsg;					// 타겟이 지정되지 않은 경우 메세지 출력.
	CWindowby3		m_MsgBackImage;	
		
	RwUInt32		m_hTargetSerialID;
	CQuestGui*		m_pQuestGui;

	DLGTYPE			m_eDlgType;

	sMultiDialogData m_stMultiDialogData;
};

class CCommandList : public CGuiLineTree
{
public:
	//! enum
	enum COMMAND_ID { MERCHANT_COMMAND, SKILLTRAINER_COMMAND, BANKER_COMMAND, BUY_BANK_COMMAND, QUEST_COMMAND, UPGRADE_COMMAND, GUILD_COMMAND, TMQ_COMMAND,
					UD_COMMAND,
					  GUILD_MAKE, GUILD_DISSOLVE, GUILD_CANCEL_DISSOLVE, GUILD_POINTUSE, GUILD_CHANGE_NAME,
					  GUILD_CONTRIBUTION_ZENNY, GUILD_BANK_COMMAND, BUDOHSI_MERCHANT_COMMAND,

						EVENT_ITEM_EXCHANGE_COMMAND,
						EVENT_REWARD_COMMAND,

					  // Dojo
					  GUILD_DOJO_ESTABLISHMENT,		GUILD_DOJO_INFO,		GUILD_DOJO_POINTUSE,
					  GUILD_DOJO_ACCOUNT,			GUILD_DOGI,

					  // guild contribution
					  GUILD_CONTRIBUTION_CONTRIBUTION_50000,
					  GUILD_CONTRIBUTION_CONTRIBUTION_100000, GUILD_CONTRIBUTION_CONTRIBUTION_500000,
					  GUILD_CONTRIBUTION_CONTRIBUTION_1000000, GUILD_CONTRIBUTION_CONTRIBUTION_10000000,

					  // guild function start
						GUILDFUNCTION_MAX_MEMBER_30, GUILDFUNCTION_MAX_SECOND_MASTER_1,
						GUILDFUNCTION_NOTICE_BOARD, GUILDFUNCTION_POST,
						GUILDFUNCTION_WAREHOUSE, GUILDFUNCTION_MAX_MEMBER_50,
						GUILDFUNCTION_MAX_SECOND_MASTER_2, GUILDFUNCTION_MAX_MEMBER_70,
						GUILDFUNCTION_MAX_MEMBER_90, GUILDFUNCTION_MAX_SECOND_MASTER_3,
						GUILDFUNCTION_CAN_SET_EMBLEM, GUILDFUNCTION_MAX_MEMBER_110,
						GUILDFUNCTION_MAX_MEMBER_130, GUILDFUNCTION_MAX_SECOND_MASTER_4,
						GUILDFUNCTION_CAN_HAVE_DOJO, GUILDFUNCTION_CAN_USE_DOJO_TELEPORT,
						GUILDFUNCTION_MAX_MEMBER_150, GUILDFUNCTION_DOJO_UPGRADE_1,
						GUILDFUNCTION_CAN_MAKE_DOGI, GUILDFUNCTION_CAN_CHANGE_EMBLEM,
						GUILDFUNCTION_DOJO_UPGRADE_2, GUILDFUNCTION_TENKAICHI_SEED,
						GUILDFUNCTION_MAX_MEMBER_170, GUILDFUNCTION_DOJO_UPGRADE_3,
						GUILDFUNCTION_DOJO_QUEST_1, GUILDFUNCTION_DOJO_MIX_RATE_UP_1,
						GUILDFUNCTION_MAX_MEMBER_190, GUILDFUNCTION_DOJO_UPGRADE_4,
						GUILDFUNCTION_DOJO_UPGRADE_5, GUILDFUNCTION_DOJO_QUEST_2,
						GUILDFUNCTION_DOJO_VEHICLE, GUILDFUNCTION_DOJO_UPGRADE_6,
						GUILDFUNCTION_CAN_HAVE_BOOK, GUILDFUNCTION_DOJO_UPGRADE_7,
						GUILDFUNCTION_DOJO_MIX_RATE_UP_2,
					  // guild function end

                      PORTAL_COMMAND, GAMBLE_BUY_COMMNAD, CCBD_COMMAND,
					  
					  // HoipoiMix Command
					  HOIPOIMIX_CREATE_COMMAND,

						DWC_COMMAND,

						SMITH_CHANGE_BATTLE_ATTRIBUTE_COMMAND,
						SMITH_TRANSFER_ITEM_GRADE,

					ITEM_SEAL_COMMAND,

					  BUY_PUBLIC_BAG = 0xFFFFFFFE, EXIT_COMMAND = 0xFFFFFFFF,
					  GUILDFUNCTION_START = GUILDFUNCTION_MAX_MEMBER_30};
	
	//! Constructor & Destructor
	CCommandList(VOID);
	~CCommandList(VOID);

	RwBool	Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, CMultiDialogGui* pOwnerGui );
	VOID	RegistCommand( RwInt32 nID, RwUInt8 ucJob = 0xFF );			// NPC Table에서 읽어올 Command
	VOID	RegistQuestCommand( RwUInt32 nTextID, RwUInt32 nTCIDz );	// Quest에서 날라올 Command	
	VOID	RegistServerSelectCommand( RwInt32 nID, WCHAR* pText, RwUInt16 wCurrentUserCount );	// Server Select Command
	VOID	RegistExitCommand( RwInt32 eNodeType );						// 어떤 곳에서든 쓰일 Exit Command	

	//! Attribute
	RwUInt32 GetID(VOID);
	RwUInt8	 GetInputType(VOID);
	sTS_KEY& GetTSKey(VOID);
	CNtlTCUnit*	GetTCUnit(VOID);
	CMultiDialogGui* GetOwnerGui(VOID) { return m_pOwnerGui; }

	VOID	SetID( RwUInt32 uiId );
	VOID	SetInputType( RwUInt8 byInputType );
	VOID	SetTSKey( sTS_KEY& TSKey );
	VOID	SetTCUnit( CNtlTCUnit* pTCUnit );

	//! Operation
	VOID	SelfClearNodes(VOID);
	VOID	SendQuest(VOID);
	void	TriggerObjectSendQuest();
	VOID	SendGuild(VOID);
	VOID	SendGuildPoint(VOID);
	VOID	SendGuildContribution(VOID);
	VOID	SendBuyBankMenu(VOID);

	VOID	SendUserSelect( RwInt32 nSelectID, bool bResult );

private:
	//! Implementation
	VOID	ClearState_No(VOID);
	VOID	ClearState_Just(VOID);
	VOID	ClearState_ClearAndMakeQuest(VOID);
	VOID	ClearState_ClearAndMakeGuildMenu(VOID);
	VOID	ClearState_ClearAndMakeGuildPointMenu(VOID);
	VOID	ClearState_ClearAndMakeGuildContributionMenu(VOID);
	VOID	ClearState_ClearAndMakeBuyBankMenu(VOID);
	

	//! Callback
	VOID	OnClear(VOID);			// Clear를 위함.

	gui::CSlot	m_slotPaint;		

	VOID		(CCommandList::*ClearProc)();	
	
	RwUInt32	m_uiId;				// Resources to pass to Quest.
	RwUInt8		m_byInputType;

	sTS_KEY		m_TSKey;			
	CNtlTCUnit*	m_pTCUnit;

	CMultiDialogGui* m_pOwnerGui;
};

class CCommandNode : public CGuiLineTreeNode
{
public:
	//! typedef
	enum NODE_TYPE { MULTIDIALOG_NODE, QUESTCATALOG_NODE, QUESTDIALOG_NODE, BRANCHDIALOG_NODE, GUILD_MODE, GUILDPOINT_MODE,
					 GUILDCONTRIBUTION_MODE, BUYBANK_MODE, SERVERSELECT_MODE };
	//! Constructor & Destructor
	CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, std::string wstrText, NODE_TYPE eType );
	CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, std::wstring wstrText, NODE_TYPE eType );
	CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, RwInt32 nTextID );
	CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, sQUEST_INFO sData );
	~CCommandNode(VOID);

protected:
	//! Override
	VOID	ShowProc(VOID);
	VOID	HideProc(VOID);

	//! Callback
	VOID	OnClickCommand( gui::CComponent* pComponent );
	VOID	OnPressedCommand( gui::CComponent* pComponent );
	VOID	OnReleasedCommand( gui::CComponent* pComponent );
	
	gui::CSlot		m_slotClickCommand;
	gui::CSlot		m_slotPressedCommand;
	gui::CSlot		m_slotReleasedCommand;
	
	//! Variable
	gui::CButton*	m_pbtnCommand;
	gui::CPanel*	m_ppnlIcon;
	
	CPos			m_posIcon;

	NODE_TYPE		m_eNodeType;

	sQUEST_INFO		m_sData;
};

class CServerSelectNode : public CGuiLineTreeNode
{
public:
	CServerSelectNode( CGuiLineTree* pTree, RwInt32 nID, WCHAR* pText, RwInt32 nCurrentUserCount );
	~CServerSelectNode(VOID);

protected:
	//! Override
	VOID	ShowProc(VOID);
	VOID	HideProc(VOID);

	//! Callback
	VOID	OnClickCommand( gui::CComponent* pComponent );
	VOID	OnPressedCommand( gui::CComponent* pComponent );
	VOID	OnReleasedCommand( gui::CComponent* pComponent );

	gui::CSlot		m_slotClickCommand;
	gui::CSlot		m_slotPressedCommand;
	gui::CSlot		m_slotReleasedCommand;

	//! Variable
	gui::CButton*		m_pbtnCommand;
	gui::CPanel*		m_ppnlIcon;
	gui::CPanel*		m_ppnlUserCountBack;
	gui::CProgressBar*	m_ppgbUserCount;

	CPos				m_posIcon;
	CPos				m_posUserCount;
};

#endif//__MULTI_DIALOG_GUI_H__