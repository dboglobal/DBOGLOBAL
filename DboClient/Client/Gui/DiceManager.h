/*****************************************************************************
* File			: DiceManager.h
* Author		: Hong Sungbock
* Copyright		: (주)NTL
* Date			: 2008. 5. 26
* Abstract		: Dbo dice manager
*****************************************************************************
* Desc         : 
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

class CDiceGui;

enum eDiceWorkID
{
	DICE_WORKD_ID_ACTION,
	DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER,
	DICE_WORKD_ID_PARTY_INVEN_DICE,

	NUM_DICE_WORK_ID,

	INVALID_DICE_WORK_ID
};

#define dDICE_FUNCTION_NONE						(0x00)
#define dDICE_FUNCTION_DISABLE_CLOSE_BUTTON		(0x01)
#define dDICE_FUNCTION_DISABLE_CLICK_DICE		(0x02)


class CDiceManager : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef std::map<RwInt32, CDiceGui*>			MAP_DICEGUI;

	static CDiceManager* GetInstance();

	static RwBool CreateInstance();
	static VOID   DeleteInstance();
	
	CDiceManager(const RwChar* pName);
	virtual ~CDiceManager();

	VOID			Close(RwUInt8 byWorkID);
	
	VOID			Cancel(RwUInt8 byWorkID);
	VOID			CancelAll();

	VOID			LineUpDice();			///< 현재 화면에 보이는 주사위의 위치를 정렬한다

	RwInt32			SwitchDialog(bool bOpen);

	RwBool			IsActiveWorkID(RwUInt8 byWorkID);

protected:
	CDiceManager();
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	static CDiceManager* m_pInstance;

	gui::CSlot			m_slotMove;

	MAP_DICEGUI			m_mapDiceGui;
};

static CDiceManager* GetDiceManager()
{
	return CDiceManager::GetInstance();
}