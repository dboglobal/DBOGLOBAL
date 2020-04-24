#pragma once

#include "NtlSharedDef.h"

//-----------------------------------------------------------------------------------
// DWC 2.12.2014
//-----------------------------------------------------------------------------------
const int		MAX_DWC_COUNT = 30;
const int		MAX_DWC_ADMISSION_CONDITION_COUNT = 10;
const int		MAX_DWC_REWARD_COUNT = 4;
const int		MAX_DWC_MISSION_COUNT_PER_SCENARIO = 5;



enum eDWC_REWARD_GRADE
{
	DWC_REWARD_GRADE_NORMAL,
	DWC_REWARD_GRADE_SUPERIOR,
	DWC_REWARD_GRADE_EXCELLENT,
	DWC_REWARD_GRADE_RARE,
	DWC_REWARD_GRADE_LEGENDARY,

	MAX_DWC_REWARD_GRADE_COUNT,
	INVALID_DWC_REWARD_GRADE = 0xff,
};

enum eDWC_REWARD_TYPE
{
	DWC_REWARD_TITLE,
	DWC_REWARD_ITEM,
	DWC_REWARD_TYPE_COUNT,

	INVALID_DWC_REWARD_TYPE = 0xff,
};


struct sDWC_MISSION_DATA
{
	BYTE		byRewardGrade;
	WORD		wClearCount;
};

struct sDWCMISSION_REWARD_ENTITY
{
	BYTE			byType; 
	TBLIDX			tblidx;	
	BYTE			byValue; 
};


struct sDWCMISSION_REWARD
{
	TBLIDX								cardNameTblidx;
	BYTE								byRequireCount;
	float								fAcquireRate;
	sDWCMISSION_REWARD_ENTITY			asBasicReward[MAX_DWC_REWARD_COUNT];
	sDWCMISSION_REWARD_ENTITY			asRepeatReward[MAX_DWC_REWARD_COUNT];
};


struct sDWC_SCENARIO_DATA
{
	TBLIDX						dwcTblidx;
	sDWC_MISSION_DATA			asMission[MAX_DWC_MISSION_COUNT_PER_SCENARIO];
};

struct sDWC_SCENARIO_INFO
{
	TBLIDX						dwcTblidx;
	sDWC_MISSION_DATA			asMission[MAX_DWC_MISSION_COUNT_PER_SCENARIO];
};

struct sDWC_WORLD_COUNT_INFO
{
	TBLIDX			tblidx;
	WORD			wUseCount;
};

