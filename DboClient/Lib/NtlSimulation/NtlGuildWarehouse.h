/*****************************************************************************
*
* File			: NtlGuildWarehouse.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 2. 27
* Abstract		: Simulation Guild Warehouse class
*****************************************************************************
* Desc          : 길드 창고는 길드장과 부길드장이 이용할 수 있다.
*				  길드 창고는 한번에 한명만이 이용할 수 있다.
*				  
*				  길드 창고에 있는 아이템은 단순 데이터로만 존재하며
*				  가방, 스카우터, 장비 슬롯 등에 옮겨야먄 Sob 객체가 된다
*****************************************************************************/

#ifndef __NTL_GUILD_WAREHOUSE_H__
#define __NTL_GUILD_WAREHOUSE_H__

// core
#include "ceventhandler.h"

// shared
#include "NtlItem.h"

// simulation
#include "NtlSLDef.h"

class CNtlSobItem;
struct sITEM_TBLDAT;

struct sGuildWarehouseSlot
{
	SERIAL_HANDLE		hHandle;
	sITEM_TBLDAT*		pITEM_TBLDAT;
	RwUInt8				byServerPlace;
	RwUInt8				byPos;
	RwUInt8				byStackcount;
	RwUInt8				byRank;
	RwUInt8				byCurDur;
	RwBool				bNeedToIdentify;
	RwUInt8				byGrade;
	RwUInt8				byBattleAttribute;		// 진기맹여락 cf) NtlBattle.h eBATTLE_ATTRIBUTE 
	RwUInt8				byRestrictState;
	sITEM_OPTION_SET	sOptionSet;
};

class CNtlGuildWarehouse : public RWS::CEventHandler
{
public:
	CNtlGuildWarehouse();
	virtual ~CNtlGuildWarehouse();

	RwBool			Create();
	void			Destroy();

	RwUInt32		GetZenny();
	SERIAL_HANDLE	GetNPCHandle();
	sGuildWarehouseSlot* GetItem(RwUInt8 byIndex, RwUInt8 byPos);
	sGuildWarehouseSlot* GetItem(SERIAL_HANDLE hItem);

	RwBool			CanUseGuileWarehouse();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

protected:
	void			Clear();
	CNtlSobItem*	CreateSobItem(CNtlSobItem* pParentItem, SERIAL_HANDLE hOwner, sGuildWarehouseSlot* pItemData,
								  RwUInt8 byPlace, RwUInt8 byPos);

	void			CreateGuildWarehouseItem(RwUInt8 byIndex, RwUInt8 byPos, SERIAL_HANDLE hSrcItem);
	void			DeleteGuildWarehouseItem(RwUInt8 byIndex, RwUInt8 byPos);
	void			SetGuildWarehouseItem(SERIAL_HANDLE hHandle, RwUInt8 byIndex, RwUInt8 byPos, sGuildWarehouseSlot& rItemData);

	void			CreateEventHandler(RWS::CMsg &pMsg);
	void			DeleteEventHandler(RWS::CMsg &pMsg);
	void			ItemMoveEventHandler(RWS::CMsg &pMsg);
	void			ItemStackMoveEventHandler(RWS::CMsg &msg);

protected:
	SERIAL_HANDLE	m_hNPC;									///< 상대하고 있는 NPC
	RwUInt32		m_uiZenny;								///< 제니

	sGuildWarehouseSlot	aWarehouseSlot[NTL_MAX_GUILD_BANK_COUNT][NTL_MAX_GUILD_ITEM_SLOT];
};

#endif