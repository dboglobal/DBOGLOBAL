/*****************************************************************************
 *
 * File			: NtlSLVisualLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 21	
 * Abstract		: simulation logic.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLVISUALLOGIC_H__
#define __NTL_SLVISUALLOGIC_H__

#include "NtlSLDef.h"

class CNtlSobActor;

void Logic_SkillUseVisualEffect(CNtlSobActor *pActor, SERIAL_HANDLE hSkillSerialId);

void Logic_VisualAttackSelect(SERIAL_HANDLE hSerialId);

#endif
