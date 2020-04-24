/*****************************************************************************
*
* File			: NtlSobActionSkillIcon.h
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 30	
* Abstract		: Simulation Actionskill icon object
*****************************************************************************
* Desc          : SkillIcon에서 분리
*
*****************************************************************************/

#ifndef __NTL_SOB_ACTIONSKILL_ICON_H__
#define __NTL_SOB_ACTIONSKILL_ICON_H__

#include "NtlSobIcon.h"

class CNtlSobActionSkill;

class CNtlSobActionSkillIcon : public CNtlSobIcon
{
	DECLEAR_MEMORY_POOL(CNtlSobActionSkillIcon, NTL_DEFAULT_MEMORY_POOL)

public:

	CNtlSobActionSkillIcon();
	virtual ~CNtlSobActionSkillIcon();

	virtual void HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool IsDragAndDropPossible(void);
	virtual RwBool IsUsePossible(void);
	
	virtual RwBool Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 uiRPBonusType = 0xFF, RwUInt32 hTarget = INVALID_SERIAL_ID, bool pStatus = false);
};

#endif
