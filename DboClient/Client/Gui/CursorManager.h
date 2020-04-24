////////////////////////////////////////////////////////////////////////////////
// File: CursorManager.h
// Desc: Cursor
//
// 2006.07.13 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __CURSOR_MANAGER_H__
#define __CURSOR_MANAGER_H__

// presentation
#include "NtlPLPicking.h"

// simulation
#include "NtlSLDef.h"

#include "ceventhandler.h"

class CNtlSob;
struct sWorldPickInfo_for_Cursor;

class CCursorManager : public RWS::CEventHandler
{
public:
	//! Enumuration
	enum STATE { CS_LOBBY_CURSOR, CS_MOVE_NORMAL, CS_MOVE_DISABLE, CS_MOVE_INDUNENTER, 
				 CS_BATTLE_TARGET, CS_BATTLE_ATTACK, CS_BATTLE_BOUNDARY, CS_BATTLE_LOOTING,
				 CS_DEAL_VEHICLE, CS_DEAL_SHOP, CS_TRIGGER_OBJECT,
				 CS_OBJECT_DRAGONBALL, CS_OBJECT_MAILCHECK, CS_OBJECT_RECIPE, CS_OBJECT_INTERACTIVE,
				 CS_QUEST_DIALOG, CS_QUEST_ACCEPT, CS_QUEST_PROCEED, CS_QUEST_COMPLETE,
				 CS_SYSTEM_CAMROTATE, CS_SYSTEM_LOADING, CS_SYSTEM_HELP,
				 CS_ITEM_REPAIR, CS_ITEM_IDENTIFICATION, CS_ITEM_GET_ON_BUS, CS_ITEM_GET_OFF_BUS,
				 CS_CHAT_RESIZE_HORI, CS_CHAT_RESIZE_VERT, CS_CHAT_RESIZE_RIGHTUP, CS_CHAT_RESIZE_RIGHTDOWN,
				 CS_ITEM_CAN_NOT_GET_ON_BUS,
				 CS_GATHER_DISABLED,

				 CS_ITEM_USE_SKILL_RESET,
				 CS_SKILL_ATTACK,
				 CS_ITEM_USE_DOGIBALL,
				 CS_ITEM_DISASSEMBLE,
				 CS_NUM_CURSOR, CS_INVALID_CURSOR };

	//! Destructor
	~CCursorManager(VOID) {}

	//! Operation
	VOID CreateInstance(VOID);
	VOID DeleteInstance(VOID);

protected:

	virtual void			HandleEvents(RWS::CMsg &pMsg);

public:

	 ///< 일정한 시간마다 마우스 커서 조건을 검사, 변경한다
	VOID Update(RwReal fElapsed, sWorldPickInfo_for_Cursor* pWorldPickInfo_for_Cursor);

	VOID InitLobbyCursor();
	VOID InitGameCursor();
	
	VOID SetMouseCursorPosition(RwInt32 iXPos, RwInt32 iYPos);
	VOID SaveCurrentPosition();
	VOID RestorePosition();

	VOID SetRace( RwUInt8 eRace );	
	VOID ShowMouseCursor( RwBool bShow = TRUE );	

	VOID SetDesignatedCursor( STATE eCursorState );	// CS_INVALID_CURSOR 해제.

	RwUInt8 GetCursorState();
	
	RwBool IsShowCursor();

	int		GetOldPosX() { return m_iOldXPos; }
	int		GetOldPosY() { return m_iOldYPos; }

	//! Access
	static CCursorManager* GetInstance(VOID);

private:
	//! Implementation
	VOID GUICursorProc();
	VOID WorldCursorProc(CNtlSob* pSobObj);
	
	VOID SetMouseCursor( STATE eState );
	
	VOID SetHumanCursor(VOID);
	VOID SetNamekCursor(VOID);
	VOID SetMajinCUrsor(VOID);
	
	//! Variable
	HCURSOR m_hMouseCursor[CS_NUM_CURSOR];
	RwUInt8	m_eCurrentRace;
	RwBool  m_bShowCursor;
	SERIAL_HANDLE   m_hFocusSerialId;
	RwInt32 m_iOldXPos, m_iOldYPos;

	RwUInt8	m_eCurCursor;
	RwUInt8	m_eDesignatedCursor;		// 지정된 커서, INVALID이외의 값인 경우 항상 그 커서로 고정된다.

	bool	m_bSkillSelectMode;

	//! Prevented Constructor
	CCursorManager(VOID);
};

static CCursorManager* GetCursorManager(VOID)
{
	return CCursorManager::GetInstance();
}

#endif//__CURSOR_MANAGER_H__