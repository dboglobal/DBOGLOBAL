/*****************************************************************************
 *
 * File			: NtlSobItemIcon.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 3. 07	
 * Abstract		: Simulation item icon object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_ITEM_ICON_H__
#define __NTL_SOB_ITEM_ICON_H__

#include "NtlSobIcon.h"

class CNtlSobItem;

class CNtlSobItemIcon : public CNtlSobIcon
{
	DECLEAR_MEMORY_POOL(CNtlSobItemIcon, NTL_DEFAULT_MEMORY_POOL)
		
public:

	CNtlSobItemIcon();
	virtual ~CNtlSobItemIcon();

	virtual void Destroy(void);
	virtual void Update(RwReal fElapsed);
	virtual void HandleEvents(RWS::CMsg &pMsg);
	
	virtual RwBool IsDragAndDropPossible(void);
	virtual RwBool IsUsePossible(void);
	virtual RwBool IsStackable(void);
	
	virtual RwBool Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 param1 = 0xFF, RwUInt32 param2 = INVALID_SERIAL_ID, bool pStatus = false);

	virtual void SetIconState(RwUInt8 byIconState, RwReal fCoolTIme = 0.0f);
	virtual void SetIconState(RwUInt8 byIconState, RwReal fMaxCoolTime, RwReal fCoolTime);
};

#endif
