/*****************************************************************************
 *
 * File			: NtlSobQuestItemIcon.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 11. 08	
 * Abstract		: Simulation quest item icon object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_QUESTITEM_ICON_H__
#define __NTL_SOB_QUESTITEM_ICON_H__

#include "NtlSobIcon.h"

class CNtlSobQuestItem;

class CNtlSobQuestItemIcon : public CNtlSobIcon
{
	DECLEAR_MEMORY_POOL(CNtlSobQuestItemIcon, NTL_DEFAULT_MEMORY_POOL)
		
public:

	CNtlSobQuestItemIcon();
	virtual ~CNtlSobQuestItemIcon();

	virtual void HandleEvents(RWS::CMsg &pMsg);
	
	virtual RwBool IsDragAndDropPossible(void);
	virtual RwBool IsUsePossible(void);
	
	virtual RwBool Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 param1 = 0xFF, RwUInt32 param2 = INVALID_SERIAL_ID, bool pStatus = false);
};

#endif
