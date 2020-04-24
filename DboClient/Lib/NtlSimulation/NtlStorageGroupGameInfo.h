/*****************************************************************************
* File			: NtlStorageGroupGameInfo.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 2. 5	
* Abstract		: GameInfo 의 관련 Unit들
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_GAMEINFO_H__
#define __NTL_STORAGE_GROUP_GAMEINFO_H__

#pragma once

#include "NtlStorageGroup.h"

class CNtlStorageGroupGameInfo : public CNtlStorageGroup
{
public:
	CNtlStorageGroupGameInfo(void);
	virtual ~CNtlStorageGroupGameInfo(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_GAMEINFO_H__