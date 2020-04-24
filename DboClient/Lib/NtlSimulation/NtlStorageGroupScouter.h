/*****************************************************************************
* File			: NtlStorageGroupScouter.h
* Author		: HaeSung, Cho
* Copyright	: (주)NTL
* Date			: 2009. 2. 5	
* Abstract		: Scouter의 Storage Group
*****************************************************************************
* Desc         : 
*
* 스카우터 측정 정보를 구조체로 가지고 있는다.
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_SCOUTER_H__
#define __NTL_STORAGE_GROUP_SCOUTER_H__

#pragma once

typedef VOID (*UserScouterSaveFunc)();

#include "NtlStorageDefine.h"
#include "NtlStorageGroup.h"
#include <map>

class CNtlStorageGroupScouter : public CNtlStorageGroup
{
public:
	CNtlStorageGroupScouter(void);
	virtual ~CNtlStorageGroupScouter(void);


	virtual bool Load(CNtlStorageSerializer* pS);
	virtual bool Save(CNtlStorageSerializer* pS);

	void Clear();
	void LoadScouterInformation(CNtlSerializer* pS, RwBool bMonster);

	void					UseMonsterScouter(RwUInt32 uiGroupId, RwUInt32 uiPower);
	SMemoryScouterData*	IsUseMonsterScouter(RwUInt32 uiGroupId);						// return NULL == FALSE

	void					UsePlayerScouter(RwUInt32 uiCharId, RwUInt32 uiPower);
	SMemoryScouterData*	IsUsePlayerScouter(RwUInt32 uiCharId);

protected:
	typedef std::map<RwUInt32, SMemoryScouterData> MapMemoryScouter;

	MapMemoryScouter		m_mapMobScouter;
	MapMemoryScouter		m_mapPlayerScouter;
};

#endif