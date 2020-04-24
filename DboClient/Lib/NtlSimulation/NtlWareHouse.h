/*****************************************************************************
*
* File			: NtlWareHouse.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 11. 20	
* Abstract		: Simulation Warehouse class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_WARE_HOUSE_H__
#define __NTL_WARE_HOUSE_H__

#include "NtlItem.h"
#include "ceventhandler.h"
#include "NtlSLDef.h"

class CNtlSobItem;
struct SNtlEventSobItemAdd;

class CNtlWarehouse : public RWS::CEventHandler
{
public:
	CNtlWarehouse();
	virtual ~CNtlWarehouse();

	void			Destroy(void);

	SERIAL_HANDLE	GetSlotSerailID(RwUInt8 bySlot);		///< 해당 슬롯의 핸들을 반환한다
	RwUInt8			FindWarehouseSlot(SERIAL_HANDLE hSerial);///< 인자로 받은 핸들을 가지고 있는 슬롯을 찾는다
	RwUInt32		GetZenny();								///< 창고안의 제니를 반환한다

	RwBool			IsHaveCommonWarehouse();				///< 공유 창고를 가지고 있는지 알아본다	
	RwBool			IsRecieveData_from_Server();			///< 서버로 부터 창고 데이터를 모두 받았는지 여부

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

protected:	
	CNtlSobItem* CreateItem(void *pParentSobItem, sITEM_PROFILE* pSobItemCreate);
	CNtlSobItem* CreateItem(void *pParentSobItem, SNtlEventSobItemAdd *pSobItemAdd);

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void DeleteEventHandler(RWS::CMsg &pMsg);
	void ItemMoveEventHandler(RWS::CMsg &pMsg);
	void ItemStackMoveEventHandler(RWS::CMsg &msg);

protected:
	SERIAL_HANDLE	m_hWareHouseSlot[NTL_MAX_BANKSLOT_COUNT];///< 창고 슬롯
	RwUInt32		m_uiZenny;								///< 제니

	RwBool			m_bHaveCommonWarehouse;					///< 공유창고 소유여부
	RwBool			m_bRecieveData_from_Server;				///< 서버로 부터 창고 데이터를 모두 받았는지 여부
};

#endif