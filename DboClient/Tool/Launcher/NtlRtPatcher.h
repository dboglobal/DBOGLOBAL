/*****************************************************************************
 *
 * File			: NtlRtPatcher.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch Data를 Patch를 한다.(RtPatch 전용)
 *
 *****************************************************************************/
#pragma once
#include "patchwin.h"
#include "NtlPatcher.h"
#include "NtlEventPatcher.h"

class CNtlRtPatcher : public CNtlPatcher
{
protected:
	CNtlRtPatcher();
	CNtlRtPatcher(const CNtlRtPatcher& rhs) {}


public:
	virtual ~CNtlRtPatcher();

	BOOL DoAction(const char *pszDestPatchPath, const char *pszCurFullPatchFileName);
	
	static CNtlPatcher *GetInstance()
	{ 
		if(s_pInstance == NULL)
			s_pInstance = new CNtlRtPatcher;

		ASSERT(s_pInstance != NULL);
		return s_pInstance;
	}
};
