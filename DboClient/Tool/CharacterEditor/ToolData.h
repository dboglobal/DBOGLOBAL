#ifndef _TOOL_DATA_
#define _TOOL_DATA_

#include "NtlPLCharacterAnimTable.h"

struct SToolAnimData
{
	SAnimData	*pAnimData;
	unsigned int uiAnimKey;
	SToolAnimData()
	{
		pAnimData = NULL;
		uiAnimKey = 0;
	}
};

#endif
