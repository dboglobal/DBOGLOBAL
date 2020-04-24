/*****************************************************************************
*
* File			: NtlSobHTBSkillIcon.h
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 1	
* Abstract		: Simulation HTBskill icon object
*****************************************************************************
* Desc          : SkillIcon에서 분리
*
*****************************************************************************/

#ifndef __NTL_SOB_HTBSKILL_ICON_H__
#define __NTL_SOB_HTBSKILL_ICON_H__

#include "NtlSobIcon.h"

class CNtlSobHTBSkill;

class CNtlSobHTBSkillIcon : public CNtlSobIcon
{
	DECLEAR_MEMORY_POOL(CNtlSobHTBSkillIcon, NTL_DEFAULT_MEMORY_POOL)

public:

	CNtlSobHTBSkillIcon();
	virtual ~CNtlSobHTBSkillIcon();

	virtual void HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool	IsDragAndDropPossible(void);
	virtual RwBool	IsUsePossible(void);

	virtual RwBool	Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 uiRPBonusType = 0xFF, RwUInt32 hTarget = INVALID_SERIAL_ID, bool pStatus = false);

	virtual	void	SetIconState(RwUInt8 byIconState, RwReal fCoolTIme = 0.0f);	
};

#endif
