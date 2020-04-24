/*****************************************************************************
* File			: NtlStorageGroupCharacter.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 8. 16	
* Abstract		: 캐릭터 별로 묶을 수 있는 옵션들
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_CHARACTER_H__
#define __NTL_STORAGE_GROUP_CHARACTER_H__

#pragma once

#include "NtlStorageGroup.h"

class CNtlStorageGroupCharacter : public CNtlStorageGroup
{
public:
	CNtlStorageGroupCharacter(void);
	virtual ~CNtlStorageGroupCharacter(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_CHARACTER_H__