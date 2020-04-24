#pragma once

#include "NtlSharedDef.h"

//-----------------------------------------------------------------------------------
// NtlMascot 3.12.2014
//-----------------------------------------------------------------------------------

const DWORD		DBO_MAX_LENGTH_MASCOT_NAME_TEXT = 32;
const DWORD		DBO_MASCOT_MAX_SP				= 1000;
const int		GMT_MAX_PC_HAVE_MASCOT			= 40;
const int		DBO_MASCOT_MAX_SKILL_COUNT		= 4;
const int		DBO_MASCOT_MAX_LOOT				= 20;

enum eMASCOT_MODEL_TYPE
{
	MASCOT_MODEL_TYPE_ANIMAL,
	MASCOT_MODEL_TYPE_HUMAN_ANIMAL,
	MASCOT_MODEL_TYPE_DINOSAUR,
	MASCOT_MODEL_TYPE_ALIEN,
	MASCOT_MODEL_TYPE_ANDROID,
	MASCOT_MODEL_TYPE_ROBOT,
	MASCOT_MODEL_TYPE_DRAGON,
	MASCOT_MODEL_TYPE_DEVIL,
	MASCOT_MODEL_TYPE_UNDEAD,
	MASCOT_MODEL_TYPE_PLANT,
	MASCOT_MODEL_TYPE_INSECT,
	MASCOT_MODEL_TYPE_HUMAN,
	MASCOT_MODEL_TYPE_NAMEC,
	MASCOT_MODEL_TYPE_MAJIN,
	MASCOT_MODEL_TYPE_NON_LIVING,
	MASCOT_MODEL_TYPE_EGG,

	MASCOT_MODEL_TYPE_COUNT,
	MASCOT_MODEL_TYPE_FIRST = MASCOT_MODEL_TYPE_ANIMAL,
	MASCOT_MODEL_TYPE_LAST = MASCOT_MODEL_TYPE_COUNT - 1,
};


#pragma pack(1)
struct sMASCOT_DATA
{
	TBLIDX			tblidx;
	DWORD			dwCurVP;
	BYTE			index;//the pet index like slotid(skillindex) for skill
};

struct sMASCOT_DATA_EX
{
	TBLIDX			tblidx;

	WORD			wunknown;
	WORD			wunknown2;

	TBLIDX			itemTblidx;

	DWORD			dwCurVP;
	DWORD			dwMaxVP;

	DWORD			dwCurExp;

	BYTE			byItemRank;

	TBLIDX			skillTblidx[DBO_MASCOT_MAX_SKILL_COUNT];

	BYTE			byIndex;

	BYTE			byUnknown;
};

#pragma pack()



